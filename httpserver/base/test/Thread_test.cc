#include<iostream>
#include"../Thread.h"
#include"../Mutex.h"
#include<functional>
#include<memory>
using namespace std;
using namespace std::placeholders;
// using Thread::ThreadFunc;
int global_i=0,global_j=0;
MutexLock global_lock;
void func()
{
    MutexLockGuard guard(global_lock);
    for(int i=0;i<100000;i++)global_i+=1;
}

void func2(int k)
{
    // MutexLockGuard guard(global_lock);
    for(int i=0;i<100000;i++)global_j+=k;
}

 
 function<void()> lambda_fuc=[](){
     cout<<"lambda"<<endl;
 };

struct Foo
{
    int i_;
    Foo(int i):i_(i){}
    void memfunc()
    {
        cout<<"test mem func"<<i_<<endl;
    }
    void memfunc2(const string &s)
    {
        cout<<s<<endl;
    }
};


int main()
{
    Thread th1(func);
    Thread th2(bind(func2,1));
    unique_ptr<Thread> th3_ptr=make_unique<Thread>(lambda_fuc);
    Foo test(1);
    unique_ptr<Thread> th4_ptr=make_unique<Thread>(bind(&Foo::memfunc,ref(test)));
    // unique_ptr<Thread> th5_ptr=make_unique<Thread>(bind(&Foo::memfunc2,ref(test),string("qi")));
    unique_ptr<Thread> th5_ptr=make_unique<Thread>(bind(&Foo::memfunc2,&test,string("qi")));
    th5_ptr->start();
    th4_ptr->start();
    th3_ptr->start();
    th1.start();
    th2.start();
    cout<<global_i<<"\n"<<global_j<<endl;
    th1.join();th2.join();
    th3_ptr->join();
    return 0;
}