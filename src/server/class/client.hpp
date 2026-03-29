#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <stdio.h>

class Client
{
    public:
        Client(SOCKET sock, sockaddr_in address, int len);
        SOCKET socket;
        sockaddr_in addr;
        int addr_len;
        std::string hostname;
        std::string ip;
        long connect_time;
        bool authenticated;
};