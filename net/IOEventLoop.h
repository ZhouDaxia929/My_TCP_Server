#ifndef MTS_NET_IOEVENTLOOP
#define MTS_NET_IOEVENTLOOP

#include "net/TimerQueue.h"
#include "net/IOEventCtrl.h"

#include <thread>
#include <mutex>

namespace My_TCP_Server{

namespace net{

class IOEventLoop{
public:
    IOEventLoop();
    ~IOEventLoop();

    void addEvent(std::shared_ptr<IOEvent> event);
    void removeEvent(std::shared_ptr<IOEvent> event);
    void removeEvent(int fd);
    void modifyEvent(std::shared_ptr<IOEvent> event);
    void modifyEvent(int fd);

    void run();
    void runInLoop(const DefaultFunction func);
    void runOnceAfter(const DefaultFunction callback, uint32_t interval);
    void runEveryInterval(const DefaultFunction callback, uint32_t interval);

private:
    static const int PollTimeMs;
    std::mutex mutex;
    IOEventCtrl *eventCtrl;
    std::thread::id thisThreadId;
    std::shared_ptr<TimerQueue> timerQueue;
    std::vector<DefaultFunction> functions;

    void addFunInLoop(DefaultFunction func);
    bool isThisThread();
    void runAllFunctionInLoop();
};

}
}

#endif