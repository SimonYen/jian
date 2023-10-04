#pragma once
#include <functional>

namespace jian {
class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
private:
    jian::Epoll* ep;
    jian::ThreadPool* threadpool;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(jian::Channel*);

    void add_thread(std::function<void()> func);
};
}