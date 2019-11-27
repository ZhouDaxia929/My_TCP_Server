#include "net/Epoll.h"
#include "base/LogInterface.h"

#include <cstring>
#include <unistd.h>

using namespace My_TCP_Server::net;

Epoll::Epoll() : epollFd(epoll_create1(EPOLL_CLOEXEC)){
    if(epollFd < 0){
        base::Log::Instance()->error("create epoll error.");
    }
}

Epoll::~Epoll(){
    ::close(epollFd);
}

int Epoll::epollCtrl(int operation, int fd, int events){
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.events = events;
    event.data.fd = fd;
    int rst = epoll_ctl(epollFd, operation, fd, &event);
    return rst;
}

bool Epoll::addEvent(IOEvent *ioEvent){
    if(epollCtrl(EPOLL_CTL_ADD, ioEvent->getFd(), ioEvent->getEvents()) < 0){
        base::Log::Instance()->error("add epoll error.");
        return false;
    }
    return true;
}

bool Epoll::removeEvent(IOEvent *ioEvent){
    if(epollCtrl(EPOLL_CTL_DEL, ioEvent->getFd(), ioEvent->getEvents()) < 0){
        base::Log::Instance()->error("delete epoll error.");
        return false;
    }
    return true;
}

bool Epoll::removeEvent(int fd){
    if(epollCtrl(EPOLL_CTL_DEL, fd, 0) < 0){
        base::Log::Instance()->error("delete epoll error.");
        return false;
    }
    return true;
}

bool Epoll::modifyEvent(IOEvent *ioEvent){
    if(epollCtrl(EPOLL_CTL_MOD, ioEvent->getFd(), ioEvent->getEvents()) < 0){
        base::Log::Instance()->error("modify epoll error.");
        return false;
    }
    return true;
}

int Epoll::waitEvent(struct epoll_event *eventList, int eventSize, int timeMs){
    int rst = epoll_wait(epollFd, eventList, eventSize, timeMs);
    return rst;
}