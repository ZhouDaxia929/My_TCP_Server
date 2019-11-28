#include "net/TimerQueue.h"
#include "net/IOEventLoop.h"
#include "base/LogInterface.h"

#include <sys/timerfd.h>
#include <cstring>
#include <unistd.h>

using namespace My_TCP_Server::net;
using namespace std;

TimerQueue::TimerQueue(IOEventLoop *eventLoop) : loop(eventLoop), timerFd(createTimeFd()), event(new IOEvent(loop, timerFd)){
    loop->addEvent(event);
    event->enableReading(true);
    event->setReadFunc(bind(&TimerQueue::timerHandle, this));
}

TimerQueue::~TimerQueue(){
    ::close(timerFd);
}

int TimerQueue::createTimeFd(){
    int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(fd < 0){
        base::Log::OUT(base::Log::Error) << "failed to create time fd";
    }
    return fd;
}

void TimerQueue::readTimerfd(){
    uint64_t cnt;
    int n = ::read(timerFd, &cnt, sizeof(cnt));
    if(n != sizeof(cnt)){
        base::Log::OUT(base::Log::Error) << "TimerQueue::handleRead() reads " 
                                         << std::to_string(n) << " bytes instead of 8";
    }
}

//添加只运行一次的定时事件
void TimerQueue::runOnceAfter(const DefaultFunction &handle, int interval){
    std::lock_guard<std::mutex> lock(mutex);
    addOnceTimer(handle, interval);
}

void TimerQueue::addOnceTimer(const DefaultFunction &handle, uint32_t interval){
    std::shared_ptr<Timer> timer(new Timer(interval, handle));
    onceTimers.insert(pair<uint64_t, std::shared_ptr<Timer>>(timer->getTimeOutMSecond(), timer));
    if(needResetTimer(onceTimers, timer) && needResetTimer(everyTimers, timer)){
        resetTimer(timer);
    }
}

//添加定期运行的定时事件
void TimerQueue::runEveryInterval(const DefaultFunction &handle, int interval){
    std::lock_guard<std::mutex> lock(mutex);
    addEveryTimer(handle, interval);
}

void TimerQueue::addEveryTimer(const DefaultFunction &handle, uint32_t interval){
    std::shared_ptr<Timer> timer(new Timer(interval, handle));
    everyTimers.insert(pair<uint64_t, std::shared_ptr<Timer>>(timer->getTimeOutMSecond(), timer));
    if(needResetTimer(onceTimers, timer) && needResetTimer(everyTimers, timer)){
        resetTimer(timer);
    }
}

//我们要保证，multimap中第一个事件的到期时间是最近的，因此每次加入新事件后都要检查multimap是否需要调整。
bool TimerQueue::needResetTimer(std::multimap<uint64_t, std::shared_ptr<Timer>> &times, std::shared_ptr<Timer> timer){
    if(times.empty()){
        return true;
    }

    multimap<uint64_t, std::shared_ptr<Timer>>::iterator it = times.begin();

    //如果定时器容器中第一个事件的到期时间 还是早于 刚刚加入事件的到期时间，那么不需要调整
    if(it->first < timer->getTimeOutMSecond()){
        return false;
    }

    //因为是multimap，所以如果新加入的事件的到期时间在multimap中如果已经存在的话，说明不需要调整。
    if(times.count(timer->getTimeOutMSecond()) > 1){
        return false;
    }

    return true;
}

//如果新加入定时事件的到期时间比容器中即将发生的事件的到期时间还近的话，重置定时器timerFd的到期时间为新加入事件的到期时间
void TimerQueue::resetTimer(std::shared_ptr<Timer> timer){
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof(newValue));
    bzero(&oldValue, sizeof(oldValue));
    newValue.it_value = timer->getTimeInterval();
    int ret = ::timerfd_settime(timerFd, 0, &newValue, &oldValue);
    if(ret < 0){
        base::Log::OUT(base::Log::Error) << "timerfd_settime() error";
    }
}

//对两个容器整体进行调整
void TimerQueue::resetTimer(){
    if(onceTimers.empty()){
        if(!everyTimers.empty()){
            multimap<uint64_t, std::shared_ptr<Timer>>::iterator it = everyTimers.begin();
            resetTimer(it->second);
        }
    }
    else{
        if(everyTimers.empty()){
            multimap<uint64_t, std::shared_ptr<Timer>>::iterator it = onceTimers.begin();
            resetTimer(it->second);
        }
        else{
            multimap<uint64_t, std::shared_ptr<Timer>>::iterator it1 = everyTimers.begin();
            multimap<uint64_t, std::shared_ptr<Timer>>::iterator it2 = onceTimers.begin();
            if(it1->second->getTimeOutMSecond() < it2->second->getTimeOutMSecond()){
                resetTimer(it1->second);
            }
            else{
                resetTimer(it2->second);
            }
        }
    }
}


/*
定时器事件被触发时，执行到期时间小于当前时间的定时器回调函数，并删除队列。如果固定间隔多次触发的事件，则删除元素后，更新时间重新添加
*/
void TimerQueue::timerHandle(){
    std::lock_guard<std::mutex> lock(mutex);
    readTimerfd();
    multimap<uint64_t, std::shared_ptr<Timer>>::iterator it;

    //遍历multimap，触发其中到时的事件，并将其从multimap中删除
    for(it = onceTimers.begin(); it != onceTimers.end(); ++it){
        if(it->first > Timer::getNowTimeMSecond()){
            break;
        }
        it->second->timerHandle();
        onceTimers.erase(it);
    }

    //遍历multimap，触发其中到时的事件，
    for(it = everyTimers.begin(); it != everyTimers.end(); ++it){
        if(it->first > Timer::getNowTimeMSecond()){
            break;
        }
        it->second->timerHandle();
        std::shared_ptr<Timer> timer = it->second;
        timer->update();
        everyTimers.insert(pair<uint64_t, std::shared_ptr<Timer>>(timer->getTimeOutMSecond, timer));
        everyTimers.erase(it);
    }
    resetTimer();
}