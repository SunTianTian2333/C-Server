
#include <unistd.h>
#include <utility>

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Channel::Channel(EventLoop *_loop, int _fd)
    : loop_(_loop), fd_(_fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}

Channel::~Channel() {
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
  }
}

void Channel::HandleEvent() {
  if (ready_events_ & (EPOLLIN | EPOLLPRI)) {
    read_callback_();
  }
  if (ready_events_ & (EPOLLOUT)) {
    write_callback_();
  }
}

void Channel::EnableRead() {
  listen_events_ |= EPOLLIN | EPOLLPRI;
  loop_->UpdateChannel(this);
}

void Channel::UseET() {
  listen_events_ |= EPOLLET;
  loop_->UpdateChannel(this);
}

int Channel::GetFd() { return fd_; }

uint32_t Channel::GetListenEvents() { return listen_events_; }
uint32_t Channel::GetReadyEvents() { return ready_events_; }

bool Channel::GetInEpoll() { return in_epoll_; }

void Channel::SetInEpoll(bool in) { in_epoll_ = in; }

void Channel::SetReadyEvents(uint32_t ev) { ready_events_ = ev; }

void Channel::SetReadCallback(std::function<void()> const &callback) { read_callback_ = callback; }

// void Channel::handleEvent(){
//     loop->addThread(callback);  //引入线程池
//     // callback();
// }

// void Channel::handleEvent(){
//     if(ready & (EPOLLIN | EPOLLPRI)){
//         if(useThreadPool)
//             loop->addThread(readCallback);
//         else
//             readCallback();
//     }
//     if(ready & (EPOLLOUT)){
//         if(useThreadPool)
//             loop->addThread(writeCallback);
//         else
//             writeCallback();
//     }
// }

// void Channel::enableReading(){
//     events = EPOLLIN | EPOLLET;
//     loop->updateChannel(this);
// }

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

// void Channel::setUseThreadPool(bool use){
//     useThreadPool = use;
// }
