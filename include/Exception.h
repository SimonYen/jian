#pragma once

#include <cerrno>
#include <cstring>
#include <string>

namespace jian {
class JianException {
protected:
    std::string message;

public:
    virtual ~JianException() = default;
    const char* what();
};

class EpollException : public JianException {
public:
    EpollException();
};

class SocketException : public JianException {
public:
    SocketException();
};
}