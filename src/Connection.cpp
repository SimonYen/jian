#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Exception.h"
#include "Socket.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <unistd.h>

jian::Connection::Connection(jian::EventLoop* _loop, jian::Socket* _sock)
    : loop(_loop)
    , sock(_sock)
    , chan(nullptr)
{
    chan = new Channel(loop, sock->get_fd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->get_fd());
    chan->set_callback(cb);
    chan->enable_reading();
    read_buffer = new Buffer();
}

jian::Connection::~Connection()
{
    delete chan;
    delete sock;
}

void jian::Connection::echo(int fd)
{
    char buf[1024];
    while (true) {
        bzero(&buf, sizeof(buf));
        auto bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer->append(buf, bytes_read);
        } else if (bytes_read == -1 && errno == EINTR) {
            std::cout << "continue reading" << std::endl;
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "finish reading once." << std::endl;
            std::cout << "message from client fd\t" << fd << " : \t" << read_buffer->c_str() << std::endl;
            if (-1 == write(fd, read_buffer->c_str(), read_buffer->size()))
                throw SocketException {};
            read_buffer->clear();
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