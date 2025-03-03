#include "../src/include/Server.h"
#include "../src/include/EventLoop.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  loop->loop();
  delete server;
  return 0;
}
