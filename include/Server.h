#pragma once

#include <unordered_map>

namespace jian {
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server {
private:
    jian::EventLoop* loop;
    jian::Acceptor* acceptor;
    std::unordered_map<int, jian::Connection*> connections;

public:
    Server(jian::EventLoop* _loop);
    ~Server();

    void new_connection(jian::Socket* sock);
    void delete_connection(jian::Socket* sock);
};
}