#include "Acceptor.h"
#include <cstdio>
#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop) {
  // 将创建服务器socket的任务从Server类转移到这里
  sock = new Socket();
  InetAddress *addr = new InetAddress("127.0.0.1", 8888);
  sock->bind(addr);
  sock->listen();
  // sock->setnonblocking();
  acceptChannel = new Channel(loop, sock->getFd());
  std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
  acceptChannel->setReadCallback(cb);
  acceptChannel->enableRead();
  // acceptChannel->setUseThreadPool(false);
  delete addr;
}

Acceptor::~Acceptor() {
  delete sock;
  delete acceptChannel;
}

// 从Server转移到这里
void Acceptor::acceptConnection() {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
  printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->getAddr().sin_addr),
         ntohs(clnt_addr->getAddr().sin_port));
  clnt_sock->setnonblocking();
  newConnectionCallback(clnt_sock);
  delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _cb) { newConnectionCallback = _cb; }
