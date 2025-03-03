#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cstddef>
#include <memory>


#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr), readBuffer(nullptr) {
  channel = new Channel(loop, sock->getFd());
  channel->enableRead();
  channel->useET();
  std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
  channel->setReadCallback(cb);
  // channel->setUseThreadPool(true);
  readBuffer = new Buffer();
}

Connection::~Connection() {
  delete channel;
  delete sock;
  delete readBuffer;
}

void Connection::echo(int sockfd) {
  char buf[READ_BUFFER];
  while (true) {  // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    memset(&buf, 0, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      readBuffer->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) {  // 客户端正常中断、继续读取
      printf("continue reading");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {  // 非阻塞IO，这个条件表示数据全部读取完毕
      printf("finish reading once\n");
      printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
      errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
      readBuffer->clear();
      break;
    } else if (bytes_read == 0) {  // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", sockfd);
      // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
      deleteConnectionCallback(sockfd);
      break;
    } else {
      printf("Connection reset by peer\n");
      deleteConnectionCallback(sockfd);  // 会有bug，注释后单线程无bug
      break;
    }
  }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb) { deleteConnectionCallback = _cb; }

void Connection::send(int sockfd) {
  size_t bufferSize = readBuffer->size();

  // char buf[readBuffer->size()];
  std::unique_ptr<char[]> buf(new char[bufferSize]);

  snprintf(buf.get(), sizeof(buf), "%s", readBuffer->c_str());
  int data_size = readBuffer->size();
  int data_left = data_size;
  while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf.get() + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    data_left -= bytes_write;
  }
}
