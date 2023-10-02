#include "Epoll.h"
#include "Channel.h"
#include "Exception.h"
#include <strings.h>
#include <sys/epoll.h>
#include <vector>

const int MAX_EVENTS = 1000;

jian::Epoll::Epoll()
    : epfd(-1)
    , events(nullptr)
{
    epfd = epoll_create1(0);
    if (epfd == -1) {
        throw EpollException {};
    }
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

jian::Epoll::~Epoll()
{
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

void jian::Epoll::add_fd(int fd, uint32_t op)
{
    auto ev = new epoll_event;
    bzero(ev, sizeof(epoll_event));
    ev->data.fd = fd;
    ev->events = op;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, ev) == -1)
        throw EpollException {};
}

void jian::Epoll::update_channel(jian::Channel* chan)
{
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = chan;
    ev.events = chan->get_events();
    if (chan->is_in_epoll()) {
        if (-1 == epoll_ctl(epfd, EPOLL_CTL_MOD, chan->get_fd(), &ev))
            throw EpollException {};
    } else {
        if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, chan->get_fd(), &ev))
            throw EpollException {};
        chan->set_in_epoll();
    }
}

std::vector<jian::Channel*> jian::Epoll::poll(int timeout)
{
    decltype(poll()) active_channels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    if (-1 == nfds)
        throw EpollException {};
    for (int i = 0; i < nfds; i++) {
        auto chan = reinterpret_cast<jian::Channel*>(events[i].data.ptr);
        chan->set_revents(events[i].events);
        active_channels.push_back(chan);
    }
    return active_channels;
}