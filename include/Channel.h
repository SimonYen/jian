#pragma once

#include <cstdint>
#include <sys/epoll.h>

namespace jian {
class Epoll;
}
namespace jian {
class Channel {
private:
    jian::Epoll* ep;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool in_epoll;

public:
    Channel(jian::Epoll* _ep, int _fd);
    ~Channel();

    void enable_reading();

    int get_fd();
    uint32_t get_events();
    uint32_t get_revents();

    bool is_in_epoll();
    void set_in_epoll();

    void set_revents(uint32_t _revents);
};
}