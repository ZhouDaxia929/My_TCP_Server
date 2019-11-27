#include "net/TimerQueue.h"
#include "net/IOEventLoop.h"
#include "base/LogInterface.h"

#include <sys/timerfd.h>
#include <cstring>
#include <unistd.h>

using namespace My_TCP_Server::net;
using namespace std;

TimerQueue::TimerQueue(IOEventLoop *eventLoop) : loop(eventLoop), timerFd(createTimeFd()), event(new IOEvent(loop, timerFd)){
    
}
