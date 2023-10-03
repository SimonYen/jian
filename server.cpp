#include "Server.h"
#include "EventLoop.h"
#include "Exception.h"
#include <iostream>

using namespace jian;
using namespace std;
int main(void)
{
    try {
        auto loop = new EventLoop();
        auto server = new Server(loop);
        loop->loop();

    } catch (JianException& e) {
        cerr << e.what() << endl;
    }

    return 0;
}