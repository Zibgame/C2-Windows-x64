#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include "client.hpp"

class Server
{
public:
    Server();
    SOCKET socket;
    std::vector<Client> clients;
    std::vector<std::string> blacklist;
};