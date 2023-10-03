#pragma once

#include <functional>

namespace jian {
class EventLoop;
class Socket;
class Channel;
class Acceptor {
private:
    jian::EventLoop* loop;
    jian::Socket* sock;
    jian::Channel* accept_channel;
    std::function<void(jian::Socket*)> new_connection_callback;

public:
    Acceptor(jian::EventLoop* _loop);
    ~Acceptor();
    void accept_connection();
    void set_new_connection_callback(std::function<void(jian::Socket*)> _callback);
};
}