#ifndef MTS_NET_EPOLL_H
#define MTS_NET_EPOLL_H

#include "net/IOEvent.h"

#include <sys/epoll.h>

namespace My_TCP_Server{

namespace net{

class Epoll{
public:
    Epoll(const Epoll&) = delete;

    Epoll();
    ~Epoll();
    bool addEvent(IOEvent *ioEvent);
    bool removeEvent(int fd);
    bool removeEvent(IOEvent *ioEvent);
    bool modifyEvent(IOEvent *ioEvent);
    int waitEvent(struct epoll_event *eventList, int eventSize, int timeMs);
private:
    int epollCtrl(int operation, int fd, int events);
    int epollFd;
};

}
}

#endif