#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

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

    struct sockaddr_in saClient;
    int saClient_size = sizeof(saClient);
    SOCKET client_socket;
    printf("Waiting for connection...\n");

    client_socket = accept(sock, (struct sockaddr*)&saClient, &saClient_size);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Error: accept failed\n");
        WSACleanup();
        return (1);
    }
    printf(GREEN "Client connected!\n" RESET);

    printf("Succes: Loop initialized\n");
    while (true)
    {
        Sleep(1000);
    }
    WSACleanup();
    return (0);
}