#include "encryption.hpp"

std::string crypt_this(const std::string &str, const std::string &key)
{
    std::string crypt_string;
    int i = 0;

    crypt_string.resize(str.size());

    while (i < (int)str.size())
    {
        crypt_string[i] = str[i] ^ key[i % key.size()];
        i++;
    }
    return crypt_string;
}