#include "EventLoop.h"
#include "Epoll.h"

jian::EventLoop::EventLoop()
    : ep(nullptr)
    , quit(false)
{
    ep = new Epoll();
}

jian::EventLoop::~EventLoop() { delete ep; }

void jian::EventLoop::loop()
{
    while (!quit) {
        auto channels = ep->poll();
        for (auto chan : channels)
            chan->handle_event();
    }
}

void jian::EventLoop::update_channel(jian::Channel* chan)
{
    ep->update_channel(chan);
}