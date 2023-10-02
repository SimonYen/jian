#include "Epoll.h"
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

std::vector<epoll_event> jian::Epoll::poll(int timeout)
{
    std::vector<epoll_event> active_events;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    if (nfds == -1)
        throw EpollException {};
    for (int i = 0; i < nfds; i++)
        active_events.push_back(events[i]);
    return active_events;
}