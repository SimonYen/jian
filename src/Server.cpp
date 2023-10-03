#include "Server.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

const int READ_BUFFER = 1024;

jian::Server::Server(jian::EventLoop* _loop)
    : loop(_loop)
{
    auto serv_sock = new Socket();
    auto serv_addr = new InetAddress("127.0.0.1", 8888);

    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->set_nonblocking();

    auto serv_chan = new Channel(loop, serv_sock->get_fd());
    std::function<void()> cb = std::bind(&Server::new_connection, this, serv_sock);
    serv_chan->set_callback(cb);
    serv_chan->enable_reading();
}

jian::Server::~Server()
{
    if (loop) {
        delete loop;
        loop = nullptr;
    }
}

void jian::Server::handle_read_event(int fd)
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
            close(fd);
            break;
        }
    }
}

void jian::Server::new_connection(jian::Socket* serv_sock)
{
    auto clnt_addr = new InetAddress();
    auto clnt_sock = new Socket(serv_sock->accept(clnt_addr));
    std::cout << "new client fd " << clnt_sock->get_fd() << " is coming! IP:\t"
              << inet_ntoa(clnt_addr->addr.sin_addr) << " port:\t"
              << ntohs(clnt_addr->addr.sin_port) << std::endl;
    clnt_sock->set_nonblocking();
    auto clnt_chan = new Channel(loop, clnt_sock->get_fd());
    std::function<void()> cb = std::bind(&Server::handle_read_event, this, clnt_sock->get_fd());
    clnt_chan->set_callback(cb);
    clnt_chan->enable_reading();
}