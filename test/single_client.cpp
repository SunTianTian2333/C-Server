#include <unistd.h>
#include <cstring>
#include <functional>
#include <iostream>

#include "../src/include/Connection.h"
#include "../src/include/Socket.h"

const int PORT = 8888;
// const int BUFFER_SIZE = 1024;

int main() {
  Socket *sock = new Socket();
  sock->Connect("127.0.0.1", PORT);

  Connection *conn = new Connection(nullptr, sock);

  while (true) {
    conn->GetlineSendBuffer();
    conn->Write();
    if (conn->GetState() == Connection::State::Closed) {
      conn->Close();
      break;
    }
    conn->Read();
    std::cout << "Message from server: " << conn->ReadBuffer() << std::endl;
  }

  delete conn;
  return 0;
}
