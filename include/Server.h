#pragma once

namespace jian {
class EventLoop;
class Socket;
class Server {
private:
    EventLoop* loop;

public:
    Server(jian::EventLoop* _loop);
    ~Server();

    void handle_read_event(int fd);
    void new_connection(jian::Socket* serv_sock);
};
}