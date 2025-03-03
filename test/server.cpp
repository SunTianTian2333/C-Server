#include <iostream>

#include "../src/include/Server.h"
#include "../src/include/Buffer.h"
#include "../src/include/Connection.h"
#include "../src/include/EventLoop.h"
#include "../src/include/Socket.h"

int main() {
  EventLoop *loop = new EventLoop();
  Server *server = new Server(loop);
  server->OnConnect([](Connection * conn) {  // 自定义业务逻辑
    conn->Read();
    if (conn->GetState() == Connection ::State::Closed) {
      conn->Close();
      return;
    }
    std::cout << "Message from client " << conn->GetSocket()->GetFd() << ": " << conn->ReadBuffer() << std::endl;
    conn->SetSendBuffer(conn->ReadBuffer());
    conn->Write();
  });
  loop->Loop();
  delete server;
  delete loop;
  return 0;
}
