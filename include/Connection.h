#pragma once

#include <functional>
#include <string>

namespace jian {
class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection {
private:
    jian::EventLoop* loop;
    jian::Socket* sock;
    jian::Channel* chan;
    std::function<void(jian::Socket*)> delete_connection_callback;
    std::string* in_buffer;
    jian::Buffer* read_buffer;

public:
    Connection(jian::EventLoop* _loop, jian::Socket* _sock);
    ~Connection();

    void echo(int fd);
    void set_delete_connection_callback(std::function<void(jian::Socket*)>);
};
}