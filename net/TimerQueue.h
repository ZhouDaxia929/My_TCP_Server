#ifndef MTS_NET_TIMER_QUEUE
#define MTS_NET_TIMER_QUEUE

#include "net/IOEvent.h"
#include "net/Timer.h"

#include <map>
#include <memory>
#include <functional>
#include <mutex>

namespace My_TCP_Server{

namespace net{

class TOEventLoop;

class TimerQueue{
public:
    TimerQueue(IOEventLoop *eventLoop);
    ~TimerQueue();
    void addOnceTimer(const DefaultFunction &handle, uint32_t interval);
    void addEveryTimer(const DefaultFunction &handle, uint32_t interval);
    void runOnceAfter(const DefaultFunction &handle, int interval);
    void runEveryInterval(const DefaultFunction &handle, int interval);

private:
    IOEventLoop *loop;
    int timerFd;
    std::shared_ptr<IOEvent> event;
    std::mutex mutex;
    std::multimap<uint64_t, std::shared_ptr<Timer>> everyTimers;
    std::multimap<uint64_t, std::shared_ptr<Timer>> onceTimers;

    int createTimeFd();
    void timerHandle();
    bool needResetTimer(std::multimap<uint64_t, std::shared_ptr<Timer>> &times, std::shared_ptr<Timer> timer);
    void resetTimer(std::shared_ptr<Timer> timer);
    void resetTimer();
    void readTimerfd();
};

}
}


#endif