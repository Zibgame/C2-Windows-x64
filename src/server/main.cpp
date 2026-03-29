#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // permet de choisir la version
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested,&wsaData);
    if (err != 0)
    {
        printf("Error: WSAStartup failed\n");
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
    saServer.sin_port = htons(5150);

    if (bind(sock, (struct sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR)
    {
        printf("Error: bind failed\n");
        return (1);
    }
    printf("Succes: Bind initialized\n");

    printf("Succes: Loop initialized\n");
    while (true)
    {
        Sleep(1000);
    }
    WSACleanup();
    return (0);
}