#pragma once

#include "EventLoop.h"
#include <cstdint>
#include <sys/epoll.h>

#include <functional>

namespace jian {
class Epoll;
}
namespace jian {
class Channel {
private:
    jian::EventLoop* loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool in_epoll;
    std::function<void()> callback;

public:
    Channel(jian::EventLoop* _loop, int _fd);
    ~Channel();

    void enable_reading();

    int get_fd();
    uint32_t get_events();
    uint32_t get_revents();

    bool is_in_epoll();
    void set_in_epoll();

    void set_revents(uint32_t _revents);

    void handle_event();
    void set_callback(std::function<void()>);
};
}