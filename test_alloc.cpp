/*#include <iostream>
#include<cstring>
#include<cstdlib>
//#define _USE_ALLOC   //用过是否定义这个变量，确定使用的是哪个配置器
#include"simple_alloc.h"
using namespace std;
struct student
{
    int id;
    string  name;
    student(int _id,string _name):id(_id),name(_name){}
};
ostream& operator<<(ostream &os,const student& ss)
{
    os<<"id:"<<ss.id<<" "<<"name:"<<ss.name;
    return os;
}
int main(void)
{
    simple_alloc<student> ss;
    student *p=ss.alloc(3*sizeof(student));
    student *q=ss.alloc(3*sizeof(student));
    student *t=ss.alloc(3*sizeof(student));
    //for(;i<3;i++)           //次级配置中的一个缺陷，其实p指针应该只是指向5个int的，不知到怎么判断越界了没
    construct(p,student(1,"小zhang"));   //用来构造对象
    construct(p+1,student(2,"小lan"));
    construct(p+2,student(3,"小s"));
    int i;
    for(i=0;i<3;i++)
        cout<<p[i]<<endl;
    destroy(p);
    cout<<p[0]<<endl;
}*/
