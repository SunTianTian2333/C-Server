#include <iostream>

#include "../src/include/Buffer.h"
#include "../src/include/Connection.h"
#include "../src/include/EventLoop.h"
#include "../src/include/Server.h"
#include "../src/include/Socket.h"
#include "../src/include/SignalHandler.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);

  // 手动停止服务器
  Signal::signal(SIGINT, [&] {
    delete server;
    delete loop;
    std::cout << "\nServer exit!" << std::endl;
    exit(0);
  });

  server->NewConnect(
      [](Connection *conn) { std::cout << "New connection fd: " << conn->GetSocket()->GetFd() << std::endl; });

  server->OnMessage([](Connection *conn) {
    std::cout << "Message from client " << conn->ReadBuffer() << std::endl;
    if (conn->GetState() == Connection::State::Connected) {
      conn->Send(conn->ReadBuffer());
    }
  });

  loop->Loop();
  return 0;
}
