#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "client.hpp"
#include "server.hpp"
#include "log.hpp"

#define LOG_FILE_PATH "other/logs/log.txt"

void create_logfile()
{
    std::ofstream logfile(LOG_FILE_PATH, std::ios::out);
    if (!logfile.is_open())
        return;
    logfile.close();
}

void add_to_log(const std::string &str)
{
    std::ofstream logfile(LOG_FILE_PATH, std::ios::app);
    if (!logfile.is_open())
        return;
    logfile << str << std::endl;
    logfile.close();
}