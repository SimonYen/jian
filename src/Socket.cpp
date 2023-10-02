#include "Socket.h"
#include "Exception.h"
#include "InetAddress.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

jian::Socket::Socket()
    : fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
        throw SocketException {};
}

jian::Socket::Socket(int _fd)
    : fd(_fd)
{
    if (fd == -1)
        throw SocketException {};
}

jian::Socket::~Socket()
{
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void jian::Socket::bind(jian::InetAddress* addr)
{
    if (::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1)
        throw SocketException {};
}

void jian::Socket::listen()
{
    if (::listen(fd, SOMAXCONN) == -1)
        throw SocketException {};
}

void jian::Socket::set_nonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int jian::Socket::accept(jian::InetAddress* addr)
{
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    if (clnt_sockfd == -1)
        throw SocketException {};
    return clnt_sockfd;
}

int jian::Socket::get_fd()
{
    return fd;
}