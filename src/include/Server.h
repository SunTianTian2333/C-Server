#pragma once
#include <functional>
#include "Macros.h"

#include <map>
#include <vector>

class Acceptor;
class EventLoop;
class Socket;
class Connection;
class ThreadPool;
class Server {
 private:
  EventLoop *mainReactor_;
  Acceptor *acceptor_;
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> subReactors_;
  ThreadPool *thread_pool_;
  std::function<void(Connection *)> on_connect_callback_;

 public:
  explicit Server(EventLoop *loop);
  ~Server();

  DISALLOW_COPY_AND_MOVE(Server);

  void NewConnection(Socket *sock);
  void DeleteConnection(Socket *sock);
  void OnConnect(std::function<void(Connection *)> fn);
};
