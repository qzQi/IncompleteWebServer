
#include "../Logging.h"
#include "../Thread.h"
#include <string>
#include <unistd.h>
#include <vector>
#include <memory>
#include <iostream>
using namespace std;

void thread_log_test()
{
    int i=100;
    while(i--)
    LOG<<"hello log";
}

int main()
{
    LOG<<"hhhhh";
    vector<unique_ptr<Thread>>threads;
    threads.resize(5);
    for(auto &i:threads)i=make_unique<Thread>(thread_log_test);
    for(auto &i:threads)i->start();

    for(auto &i:threads)i->join();
}