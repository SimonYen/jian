#pragma once

#include <string>
#include <sys/types.h>

namespace jian {
class Buffer {
private:
    std::string buf;

public:
    Buffer();
    ~Buffer();
    void append(const char* _str, int _size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
};
}