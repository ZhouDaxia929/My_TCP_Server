#include "TimerTest.h"
#include "base/LogInterface.h"
#include "net/IOEventLoop.h"
#include "net/IOEventLoopThread.h"

#include <functional>

using namespace My_TCP_Server::net;
using namespace std;

int main(){
    //创建主循环事件分发器
    IOEventLoop loop;
    //增加定时运行一次事件
    loop.runOnceAfter(std::bind(&TimerTest::callback1), 2000);
    loop.runOnceAfter(std::bind(&TimerTest::callback2), 1500);
    //增加间隔固定时间运行的事件
    loop.runEveryInterval(std::bind(&TimerTest::callback3), 2100);
    //事件循环机制运行
    loop.run();
}