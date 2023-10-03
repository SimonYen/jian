#pragma once

namespace jian {
class Epoll;
class Channel;
class EventLoop {
private:
    jian::Epoll* ep;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(jian::Channel*);
};
}