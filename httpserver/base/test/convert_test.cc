#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

const char digits[]="9876543210123456789";
const char* zero=digits+9;

template<typename T>
size_t convert(char buff[],T value)
{
    T i=value;
    char* p=buff;

    do{
        int lsd=static_cast<int>(i%10);
        *p++=zero+lsd;
        i/=10;
    }while(i!=0);

    if(value<0)*p++='-';

    p='\0';
    std::reverse(buff,p);//左闭右开不包括，p当前的空字符
}
