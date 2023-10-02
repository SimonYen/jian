#pragma once

#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace jian {
class Epoll {
private:
    int epfd;
    epoll_event* events;

public:
    Epoll();
    ~Epoll();

    void add_fd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
};
}