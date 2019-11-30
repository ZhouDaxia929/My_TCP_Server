#ifndef MTS_DEMO_TIMERTEST
#define MTS_DEMO_TIMERTEST

#include <iostream>

using namespace std;

class TimerTest{
public:
    static void callback1(){
        cout << "callback 1 test" << endl;
    }

    static void callback2(){
        cout << "callback 2 test" << endl;
    }

    static void callback3(){
        cout << "callback 3 test" << endl;
    }

    static void callback4(){
        cout << "callback 4 test" << endl;
    }
};

#endif