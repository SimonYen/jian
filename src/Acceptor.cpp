#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include <functional>
#include <iostream>
#include <ostream>

jian::Acceptor::Acceptor(jian::EventLoop* _loop)
    : loop(_loop)
    , sock(nullptr)
    , accept_channel(nullptr)
{
    sock = new Socket();
    auto addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->set_nonblocking();
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accept_channel = new Channel(loop, sock->get_fd());
    accept_channel->set_callback(cb);
    accept_channel->enable_reading();

    delete addr;
}

jian::Acceptor::~Acceptor()
{
    delete sock;
    delete accept_channel;
}

void jian::Acceptor::accept_connection()
{
    auto clnt_addr = new InetAddress();
    auto clnt_sock = new Socket(sock->accept(clnt_addr));
    std::cout << "New Client fd " << clnt_sock->get_fd() << " is coming !"
              << std::endl;
    clnt_sock->set_nonblocking();
    new_connection_callback(clnt_sock);
    delete clnt_addr;
}

void jian::Acceptor::set_new_connection_callback(
    std::function<void(jian::Socket*)> _callback)
{
    new_connection_callback = _callback;
}