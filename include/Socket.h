#pragma once

#include "InetAddress.h"
namespace jian {
class Socket {
private:
    int fd;

public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(jian::InetAddress*);
    void listen();
    void set_nonblocking();

    int accept(jian::InetAddress*);

    int get_fd();
};
}