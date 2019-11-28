#include "net/Timer.h"
#include <sstream>

using namespace My_TCP_Server::net;
using namespace std;

Timer::Timer(uint32_t interval, const TimerCallback &funcHandle) : intervalMs(interval), handle(funcHandle){
    update();
}

//获取下一次的到期时间（timeval）
struct timeval Timer::getTimeOut(){
    struct timeval timeOut = now;
    timeOut.tv_sec += intervalMs / 1000;
    timeOut.tv_usec += (intervalMs % 1000) * 1000;
    return timeOut;
}

//获取下一次到期时间（毫秒数）
uint64_t Timer::getTimeOutMSecond(){
    struct timeval timeOut = getTimeOut();
    int mSecond = timeOut.tv_sec * 1000 + timeOut.tv_usec / 1000;
    return mSecond;
}

//获取当前时间的毫秒数
uint64_t Timer::getNowTimeMSecond(){
    struct timezone tz;
    struct timeval nowDate;
    gettimeofday(&nowDate, &tz);
    uint64_t ms = (nowDate.tv_sec) * 1000 + nowDate.tv_usec / 1000;
    return ms;
}

//获取到期时间和当前时间的间隔
struct timespec Timer::getTimeInterval(){
    int64_t interval = getTimeOutMSecond() - getNowTimeMSecond();
    if(interval <= 10)
        interval = 10;
    struct timespec timerInterval ;
    timerInterval.tv_sec = interval / 1000;
    timerInterval.tv_nsec = (interval % 1000) * 1000000;
    return timerInterval;
}

//更新当前时间到now中
void Timer::update(){
    struct timezone tz;
    gettimeofday(&now, &tz);
}

//设置到期处理函数
void Timer::setHandle(const TimerCallback &func){
    handle = func;
}

//获取当前日期信息
string Timer::getNowTimeDate(){
    time_t timep;
    time(&timep);
    struct tm *p = localtime(&timep);
    stringstream stream;
    stream << (1900 + p->tm_year) << " " << (1 + p->tm_mon) << "/" << p->tm_mday << " " << p->tm_hour
           << ":" << p->tm_min << ":" << p->tm_sec << endl;
    return stream.str();
}

//执行到期处理函数
void Timer::timerHandle(){
    if(handle){
        handle();
    }
}