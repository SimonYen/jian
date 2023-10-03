#pragma once

#include <functional>

namespace jian {
class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor {
private:
    jian::EventLoop* loop;
    jian::Socket* sock;
    jian::InetAddress* addr;
    jian::Channel* accept_channel;

public:
    Acceptor(jian::EventLoop* _loop);
    ~Acceptor();
    void accept_connection();
    std::function<void(jian::Socket*)> new_connection_callback;
    void set_new_connection_callback(std::function<void(jian::Socket*)> _callback);
};
}