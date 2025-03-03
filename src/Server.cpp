#include "Server.h"
#include <unistd.h>
#include <functional>
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "ThreadPool.h"
#include "util.h"

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : mainReactor_(_loop), acceptor_(nullptr) {
  acceptor_ = new Acceptor(mainReactor_);
  std::function<void(Socket *)> cb = std::bind(&Server::NewConnection, this, std::placeholders::_1);
  acceptor_->SetNewConnectionCallback(cb);

  int size = std::thread::hardware_concurrency();
  thread_pool_ = new ThreadPool(size);
  for (int i = 0; i < size; ++i) {
    subReactors_.push_back(new EventLoop());
  }

  for (int i = 0; i < size; ++i) {
    // 直接使用 subReactors[i]->loop() 是同步调用，
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop, subReactors_[i]);
    thread_pool_->Add(std::move(sub_loop));
  }
}

Server::~Server() {
  delete acceptor_;
  delete thread_pool_;
}

void Server::NewConnection(Socket *sock) {
  if (sock->GetFd() != -1) {
    ErrorIf(sock->GetFd() == -1, "new connection error");
    uint64_t random = sock->GetFd() % subReactors_.size();
    Connection *conn = new Connection(subReactors_[random], sock);
    std::function<void(Socket *)> cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
    conn->SetDeleteConnectionCallback(cb);
    conn->SetOnConnectCallback(on_connect_callback_);
    connections_[sock->GetFd()] = conn;
  }
}

void Server::DeleteConnection(Socket *sock) {
  int sockfd = sock->GetFd();
  auto it = connections_.find(sockfd);
  if (it != connections_.end()) {
    Connection *conn = connections_[sockfd];
    connections_.erase(sockfd);
    delete conn;
    conn = nullptr;
  }
}

void Server::OnConnect(std::function<void(Connection *)> fn) { on_connect_callback_ = std::move(fn); }
