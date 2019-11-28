#ifndef MTS_NET_IOEVENTLOOPTHREAD
#define MTS_NET_IOEVENTLOOPTHREAD

#include "base/Thread.h"
#include "net/IOEventLoop.h"

#include <condition_variable>

namespace My_TCP_Server{

namespace net{

class IOEventLoopThread : public base::Thread{
public:
    IOEventLoopThread();
    virtual void run();
    IOEventLoop *getLoopInThread();
private:
    IOEventLoop *loop;
    std::mutex mutex;
    std::condition_variable condtion;
};

}
}


#endif