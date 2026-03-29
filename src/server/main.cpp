#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include "client.hpp"
#include "server.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

void active_ansi()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    if (!SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        printf("ANSI not supported\n");
    }
    return ;
}

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    active_ansi();

    // permet de choisir la version
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested,&wsaData);
    if (err != 0)
    {
        printf("Error: WSAStartup failed\n");
        return (0);
    }
    printf("Succes: Winsock initialized\n");
    
    int af = AF_INET; // IPV4
    int type = SOCK_STREAM; // TCP
    int protocol = 0; // Auto mais ducoup sa choisir TCP
    SOCKET sock;
    sock = socket(af, type, protocol);
    if (sock == INVALID_SOCKET)
    {
        printf("Error: socket failed\n");
        return (1);
    }
    printf("Succes: Socket initialized\n");

    struct sockaddr_in saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;
    saServer.sin_port = htons(4444); // port HTTPS pour bypass firewall

    if (bind(sock, (struct sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR) //attache le socket a un port
    {
        printf("Error: bind failed\n");
        return (1);
    }
    printf("Succes: Bind initialized\n");

    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) // mais le socket en mode ecoute avec SOMAXCONN qui dit pas de limit de machine
    {
        printf("Error: listen failed\n");
        WSACleanup();
        return (1);
    }
    printf("Succes: Listen initialized\n");

    // prepare la sockaddr_in du client
    struct sockaddr_in saClient;
    int saClient_size = sizeof(saClient);
    SOCKET client_socket;

    printf("Succes: Loop initialized\n");
    Server server;
    server.socket = sock;

    fd_set readfds;
    printf("Waiting for connection...\n");
    while (true)
    {
        FD_ZERO(&readfds);

        // socket serveur
        FD_SET(server.socket, &readfds);
        // sockets clients
        for (int i = 0; i < (int)server.clients.size(); i++)
            FD_SET(server.clients[i].socket, &readfds);

        int activity = select(0, &readfds, NULL, NULL, NULL); //on regarde si ya une activiter dans les socket sinon on reregarde
        if (activity < 0)
            continue;

        if (FD_ISSET(server.socket, &readfds)) // vue que le server est en ecouter si ya une activiter sa veut dire que ya un mec qui veut se co
        {
            saClient_size = sizeof(saClient);
            client_socket = accept(server.socket, (struct sockaddr*)&saClient, &saClient_size); // attend une connextion est accept
            if (client_socket == INVALID_SOCKET)
            {
                printf("Error: accept failed\n");
                WSACleanup();
                return (1);
            }

            Client new_client(client_socket, saClient, saClient_size);
            server.clients.push_back(new_client);
            printf(GREEN "Client connected! %d\n" RESET, (int)server.clients.size());
            printf("Waiting for connection...\n");
        }

        for (int i = 0; i < (int)server.clients.size(); i++)
        {
            if (FD_ISSET(server.clients[i].socket, &readfds))
            {
                char buffer[1024];
                int byte = recv(server.clients[i].socket, buffer, 1024, 0);
                if (byte <= 0)
                {
                    closesocket(server.clients[i].socket);
                    server.clients.erase(server.clients.begin() + i);
                    i--;
                }
                else
                {
                    buffer[byte] = '\0';
                    printf(YELLOW "Client %d: %s\n" RESET, i, buffer);
                }
            }         
        }
    }
    WSACleanup();
    return (0);
}