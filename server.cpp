#include "Channel.h"
#include "Epoll.h"
#include "Exception.h"
#include "InetAddress.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>

using namespace jian;
using namespace std;

const int MAX_EVENTS = 1024;
const int READ_BUFFER = 1024;

void set_nonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void handleReadEvent(int);

int main(void)
{
    try {
        Socket serv_sock;
        auto serv_addr = new InetAddress("127.0.0.1", 8888);
        serv_sock.bind(serv_addr);
        serv_sock.listen();
        Epoll ep;
        serv_sock.set_nonblocking();
        auto serv_chan = new Channel(&ep, serv_sock.get_fd());
        serv_chan->enable_reading();
        //轮询
        while (true) {
            auto events = ep.poll();
            for (auto event : events) {
                //如果是当前服务器的fd
                if (event->get_fd() == serv_sock.get_fd()) {
                    auto clnt_addr = new InetAddress;
                    auto clnt_sock = new Socket(serv_sock.accept(clnt_addr));
                    cout << "new client fd " << clnt_sock->get_fd();
                    cout << " IP: " << inet_ntoa(clnt_addr->addr.sin_addr);
                    cout << " port: " << ntohs(clnt_addr->addr.sin_port) << endl;
                    clnt_sock->set_nonblocking();
                    auto clnt_chan = new Channel(&ep, clnt_sock->get_fd());
                    clnt_chan->enable_reading();
                } else if (event->get_revents() & EPOLLIN)
                    handleReadEvent(event->get_fd());
                else {
                    cout << "something else happened!" << endl;
                }
            }
        }
    } catch (JianException& e) {
        cerr << e.what() << endl;
        return -1;
    }
}

void handleReadEvent(int fd)
{
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        auto bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            cout << "message from client fd " << fd << " :" << buf << endl;
            write(fd, buf, sizeof(buf));
        } else if (bytes_read == -1 && errno == EINTR) {
            cout << "continue reading" << endl;
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            cout << "finish reading once, errno: " << errno << endl;
            break;
        } else if (bytes_read == 0) {
            cout << "EOF, client fd " << fd << " disconnected" << endl;
            close(fd);
            break;
        }
    }
}