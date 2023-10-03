#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>

jian::Channel::Channel(jian::EventLoop* _loop, int _fd)
    : loop(_loop)
    , fd(_fd)
    , events(0)
    , revents(0)
    , in_epoll(false)
{
}

jian::Channel::~Channel()
{
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void jian::Channel::enable_reading()
{
    events = EPOLLIN | EPOLLET;
    loop->update_channel(this);
}

int jian::Channel::get_fd() { return fd; }

uint32_t jian::Channel::get_events() { return events; }

uint32_t jian::Channel::get_revents() { return revents; }

bool jian::Channel::is_in_epoll() { return in_epoll; }

void jian::Channel::set_in_epoll() { in_epoll = true; }

void jian::Channel::set_revents(uint32_t _revents)
{
    revents = _revents;
}

void jian::Channel::set_callback(std::function<void()> _callback)
{
    callback = _callback;
}

void jian::Channel::handle_event()
{
    callback();
}