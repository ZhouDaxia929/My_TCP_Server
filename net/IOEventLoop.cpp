#include "net/IOEventLoop.h"
#include "base/LogInterface.h"
#include "base/Thread.h"

#include <signal.h>
#include <iostream>

using namespace std;
using namespace My_TCP_Server::net;
using namespace base;

//这个类初始化后避免程序被signal信号意外退出
class IgnoreSigPipe{
public:
    static IgnoreSigPipe* getInstance(){
        return signal;
    }
private:
    IgnoreSigPipe(){
        ::signal(SIGPIPE, SIG_IGN);
    }
    static IgnoreSigPipe *signal;
};

IgnoreSigPipe *IgnoreSigPipe::signal = new IgnoreSigPipe();

const int IOEventLoop::PollTimeMs = 3000;

IOEventLoop::IOEventLoop() 
    :eventCtrl(new IOEventCtrl(this)), 
    thisThreadId(Thread::getNowThreadId()), 
    timerQueue(new TimerQueue(this)){}


IOEventLoop::~IOEventLoop(){
    delete eventCtrl;
}

bool IOEventLoop::isThisThread(){
    return (thisThreadId == Thread::getNowThreadId());
}

void IOEventLoop::runAllFunctionInLoop(){
    std::lock_guard<std::mutex> lock(mutex);
    for(unsigned int i = 0; i < functions.size(); ++i){
        functions[i]();
    }
    functions.clear();
}

void IOEventLoop::run(){
    if(!isThisThread()){
        thisThreadId = Thread::getNowThreadId();
        base::Log::OUT( base::Log::Warn) << "Loop create and run in different thread";
    }

    while (true){
        eventCtrl->waitAndRunHandle(PollTimeMs);
        runAllFunctionInLoop();
    }
}

void IOEventLoop::addEvent(std::shared_ptr<IOEvent> event){
    eventCtrl->addEvent(event);
}

void IOEventLoop::removeEvent(std::shared_ptr<IOEvent> event){
    eventCtrl->deleteEvent(event);
}

void IOEventLoop::removeEvent(int fd){
    eventCtrl->deleteEvent(fd);
}

void IOEventLoop::modifyEvent(std::shared_ptr<IOEvent> event){
    eventCtrl->modifyEvent(event);
}

void IOEventLoop::modifyEvent(int fd){
    eventCtrl->modifyEvent(fd);
}

void IOEventLoop::addFunInLoop(DefaultFunction func){
    std::lock_guard<std::mutex> lock(mutex);
    functions.push_back(func);
}

void IOEventLoop::runInLoop(const DefaultFunction func){
    if(isThisThread()){
        func();
    }
    else{
        addFunInLoop(func);
    }
}

void IOEventLoop::runOnceAfter(const DefaultFunction callback, uint32_t interval){
    timerQueue->runOnceAfter(callback, interval);
}

void IOEventLoop::runEveryInterval(const DefaultFunction callback, uint32_t interval){
    timerQueue->runEveryInterval(callback,interval);
}