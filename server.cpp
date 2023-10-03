#include "Server.h"
#include "EventLoop.h"

using namespace jian;
int main(void)
{
    auto loop = new EventLoop();
    auto server = new Server(loop);
    loop->loop();
    return 0;
}