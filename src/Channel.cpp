#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd) 
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false){}

Channel::~Channel(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

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

void Channel::handleEvent(){
    if(ready & (EPOLLIN | EPOLLPRI)){
        readCallback();
    }
    if(ready & (EPOLLOUT)){
        writeCallback();
    }
}

// void Channel::enableReading(){
//     events = EPOLLIN | EPOLLET;
//     loop->updateChannel(this);
// }

void Channel::enableRead(){
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void Channel::useET(){
    events |= EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}
uint32_t Channel::getReady(){
    return ready;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool _in){
    inEpoll = _in;
}

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

void Channel::setReady(uint32_t _ev){
    ready = _ev;
}


void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}

// void Channel::setUseThreadPool(bool use){
//     useThreadPool = use;
// }