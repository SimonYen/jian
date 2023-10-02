#include "InetAddress.h"
#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>

jian::InetAddress::InetAddress()
    : addr_len(sizeof(addr))
{
    bzero(&addr, addr_len);
}

jian::InetAddress::InetAddress(const char* ip, uint16_t port)
    : InetAddress()
{
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
}

jian::InetAddress::~InetAddress() { }