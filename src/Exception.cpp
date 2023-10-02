#include "Exception.h"

const char* jian::JianException::what()
{
    message += strerror(errno);
    return message.c_str();
}

jian::EpollException::EpollException()
{
    message = "Epoll Error:\t";
}

jian::SocketException::SocketException()
{
    message = "Socket Error:\t";
}