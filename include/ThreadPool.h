#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace jian {
class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;

public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    void add(std::function<void()>);
};
}