#pragma once

#include "Channel.h"
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace jian {
class Channel;
}
namespace jian {
class Epoll {
private:
    int epfd;
    epoll_event* events;

public:
    Epoll();
    ~Epoll();

    void add_fd(int fd, uint32_t op);

    void update_channel(jian::Channel* chan);
    std::vector<jian::Channel*> poll(int timeout = -1);
};
}