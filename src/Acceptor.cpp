#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include <functional>

jian::Acceptor::Acceptor(jian::EventLoop* _loop)
    : loop(_loop)
{
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->set_nonblocking();
    std::function<void()> cb = std::bind(&Acceptor::accept_connection, this);
    accept_channel = new Channel(loop, sock->get_fd());
    accept_channel->set_callback(cb);
    accept_channel->enable_reading();
}

jian::Acceptor::~Acceptor()
{
    delete sock;
    delete addr;
    delete accept_channel;
}

void jian::Acceptor::accept_connection() { new_connection_callback(sock); }

void jian::Acceptor::set_new_connection_callback(
    std::function<void(jian::Socket*)> _callback)
{
    new_connection_callback = _callback;
}