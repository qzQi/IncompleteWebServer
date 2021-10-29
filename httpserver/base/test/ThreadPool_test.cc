#include "../ThreadPool.h"
#include "../Mutex.h"
#include "../CountDownLatch.h"

ThreadPool thread_pool;

void testfunc()
{
    for(int i=0;i<100000;i++);
}

void doAllTask_return()
{
    CountDownLatch latch(1);//来等待线程池里面的任务运行完成后关闭线程池

    thread_pool.run(bind(&CountDownLatch::countDown,&latch));
    latch.wait();
    thread_pool.stop();
}

int main()
{
    thread_pool.setMaxQueueSize(10);
    thread_pool.start(4);
    for(int i=1;i<100;i++)thread_pool.run(testfunc);

    doAllTask_return();

}