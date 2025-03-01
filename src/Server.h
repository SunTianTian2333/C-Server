#pragma once
#include <map>
#include <vector>

class Acceptor;
class EventLoop;
class Socket;
class Connection;
class ThreadPool;
class Server
{
private:
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int,Connection*> connections;
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;
public:
    Server(EventLoop*);
    ~Server();

    //转移到connection中
    // void handleReadEvent(int); 

    void newConnection(Socket *sock);
    void deleteConnection(int sockfd);
};

