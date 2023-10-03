#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <unistd.h>

const int READ_BUFFER = 1024;

jian::Connection::Connection(jian::EventLoop* _loop, jian::Socket* _sock)
    : loop(_loop)
    , sock(_sock)
    , chan(nullptr)
{
    chan = new Channel(loop, sock->get_fd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->get_fd());
    chan->set_callback(cb);
    chan->enable_reading();
}

jian::Connection::~Connection()
{
    delete chan;
    delete sock;
}

void jian::Connection::echo(int fd)
{
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        auto bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            std::cout << "message from client fd " << fd << " :" << buf << std::endl;
            write(fd, buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "finish reading once, errno: " << errno << std::endl;
            break;
        } else if (bytes_read == 0) {
            std::cout << "EOF, client fd " << fd << " disconnected" << std::endl;
            delete_connection_callback(sock);
            break;
        }
    }
}

void jian::Connection::set_delete_connection_callback(
    std::function<void(jian::Socket*)> _callback)
{
    delete_connection_callback = _callback;
}