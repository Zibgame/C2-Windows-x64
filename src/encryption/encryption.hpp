
#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>
#include "client.hpp"
#include "server.hpp"
#include "cli.hpp"
#include "log.hpp"

std::string crypt_this(const std::string &str, const std::string &key);