## 为什么分离出了Acceptor，新建连接的工作仍然由Server类来完成？
- Acceptor 只负责接受连接，而 Server 负责处理连接的具体逻辑。这种设计使得代码更清晰、更易于维护。
- Acceptor 的设计是通用的，它不关心具体的服务逻辑。它的唯一职责是接受连接并将新连接的 socket 传递给 Server。这种设计使得 Acceptor 可以复用于不同的服务器程序，而不需要修改其内部逻辑。


## 添加线程池
- channel类handleEvent()修改
- EventLoop里建立线程池