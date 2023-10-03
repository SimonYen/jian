#include "Server.h"
#include "Acceptor.h"
#include "Channel.h"
#include "Connection.h"
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
    , acceptor(nullptr)
{
    acceptor = new jian::Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::new_connection, this, std::placeholders::_1);
    acceptor->set_new_connection_callback(cb);
}

jian::Server::~Server()
{
    delete acceptor;
}

void jian::Server::new_connection(jian::Socket* sock)
{
    auto conn = new Connection(loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
    conn->set_delete_connection_callback(cb);
    connections[sock->get_fd()] = conn;
}

void jian::Server::delete_connection(jian::Socket* sock)
{
    auto conn = connections[sock->get_fd()];
    connections.erase(sock->get_fd());
    delete conn;
}