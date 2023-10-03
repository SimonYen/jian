#pragma once

namespace jian {
class EventLoop;
class Socket;
class Acceptor;
class Server {
private:
    jian::EventLoop* loop;
    jian::Acceptor* acceptor;

public:
    Server(jian::EventLoop* _loop);
    ~Server();

    void handle_read_event(int fd);
    void new_connection(jian::Socket* serv_sock);
};
}