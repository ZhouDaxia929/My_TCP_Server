#ifndef MTS_NET_IOEVENTLOOPTHREADPOOL
#define MTS_NET_IOEVENTLOOPTHREADPOOL

#include "net/IOEventLoopThread.h"
#include "net/IOEventLoop.h"

#include <vector>
#include <memory>

namespace My_TCP_Server{

namespace net{

class IOEventLoopThreadPool{
public:
    IOEventLoopThreadPool(IOEventLoop *loop);
    void init();
    void init(int num);
    bool isStarted();
    void setThreadNum(uint16_t num);

    IOEventLoop *getOneLoopFromPool();
    IOEventLoop *getOneLoopFromPoolWithHash(int hashCode);
private:
    IOEventLoop *mainLoop;
    bool started;
    uint16_t threadNum;
    uint32_t threadIndex;
    std::vector<std::shared_ptr<IOEventLoopThread>> threadPool;
    std::vector<IOEventLoop *> eventLoopPool;
};

}
}

#endif