// #include "src/EventLoop.h"
// #include "src/Server.h"

#include "EventLoop.h"
#include "Server.h"
int main() {
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    return 0;
}