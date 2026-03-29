#include "client.hpp"

Client::Client(SOCKET sock, sockaddr_in address, int len)
    : socket(sock), addr(address), addr_len(len)
{
}