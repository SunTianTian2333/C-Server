#pragma once
#include <map>

class Acceptor;
class EventLoop;
class Socket;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int,Connection*> connections;
public:
    Server(EventLoop*);
    ~Server();

    //转移到connection中
    // void handleReadEvent(int); 

    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};

