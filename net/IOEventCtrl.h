#ifndef MTS_NET_IOEVENTCTRL
#define MTS_NET_IOEVENTCTRL

#include "net/Epoll.h"
#include "net/IOEvent.h"

#include <map>
#include <vector>
#include <memory>

namespace My_TCP_Server{

namespace net{

class IOEventLoop;

class IOEventCtrl{
public:
    IOEventCtrl(IOEventLoop *l);
    ~IOEventCtrl();
    void addEvent(std::shared_ptr<IOEvent> event);
    void deleteEvent(std::shared_ptr<IOEvent> event);
    void deleteEvent(int fd);
    void modifyEvent(std::shared_ptr<IOEvent> event);
    void modifyEvent(int fd);

    void waitAndRunHandle(int timeMs);
    
private:
    static const int activeEventLength;
    //事件池，kv分别是fd和事件
    std::map<int, std::weak_ptr<IOEvent>> eventPool;
    Epoll epoll;
    IOEventLoop *loop;
    std::vector<struct epoll_event> activeEvents;
};

}
}

#endif