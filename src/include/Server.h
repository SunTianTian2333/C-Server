#pragma once
#include "Macros.h"

#include <functional>
#include <map>
#include <vector>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server {
 private:
  EventLoop *main_reactor_;
  Acceptor *acceptor_;
  std::map<int, Connection *> connections_;
  std::vector<EventLoop *> sub_reactors_;
  ThreadPool *thread_pool_;
  std::function<void(Connection *)> on_connect_callback_;
  std::function<void(Connection *)> on_message_callback_;
  std::function<void(Connection *)> new_connect_callback_;

 public:
  explicit Server(EventLoop *loop);
  ~Server();

  DISALLOW_COPY_AND_MOVE(Server);

  void NewConnection(Socket *sock);
  void DeleteConnection(Socket *sock);
  void OnConnect(std::function<void(Connection *)> fn);
  void OnMessage(std::function<void(Connection *)> fn);
  void NewConnect(std::function<void(Connection *)> fn);
};

