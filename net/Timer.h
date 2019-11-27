#ifndef MTS_NET_TIMER
#define MTS_NET_TIMER

#include <stdint.h>
#include <sys/time.h>
#include <functional>
#include <memory>

namespace My_TCP_Server{

namespace net{

    using TimerCallback = std::function<void()>;
class Timer{
public:
    Timer(uint32_t interval, const TimerCallback &funcHandle);
    struct timeval getTimeOut();
    uint64_t getTimeOutMSecond();
    struct timespec getTimeInterval();
    void update();
    void setHandle(const TimerCallback&);

    static uint64_t getNowTimeMSecond();
    static std::string getNowTimeDate();

    void timerHandle();
private:
    uint32_t intervalMs;
    struct timeval now;
    TimerCallback handle;
};


}
}

#endif