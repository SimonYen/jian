#include "EventLoop.h"
#include "Epoll.h"
#include "ThreadPool.h"

jian::EventLoop::EventLoop()
    : ep(nullptr)
    , threadpool(nullptr)
    , quit(false)
{
    ep = new Epoll();
    threadpool = new ThreadPool();
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

void jian::EventLoop::add_thread(std::function<void()> func)
{
    threadpool->add(func);
}