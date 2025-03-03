#include "Acceptor.h"
#include <cstdio>
#include "Channel.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop *_loop) : loop_(_loop) {
  // 将创建服务器socket的任务从Server类转移到这里
  sock_ = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 8888);
  sock_->Bind(addr);
  sock_->Listen();
  // sock->setnonblocking();
  channel_ = new Channel(loop_, sock_->GetFd());
  std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
  channel_->SetReadCallback(cb);
  channel_->EnableRead();
  // acceptChannel->setUseThreadPool(false);
  delete addr;
}

Acceptor::~Acceptor() {
  delete channel_;
  delete sock_;
}

// 从Server转移到这里
void Acceptor::AcceptConnection() {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->GetFd(), clnt_addr->GetIp(), clnt_addr->GetPort());
  clnt_sock->SetNonBlocking();  // 新接受到的连接设置为非阻塞式
  new_connection_callback_(clnt_sock);
  delete clnt_addr;
}

void Acceptor::SetNewConnectionCallback(std::function<void(Socket *)> const &callback) {
  new_connection_callback_ = callback;
}

