//   test_alloc.cpp
#include <iostream>
#include<cstring>
#include<cstdlib>
//#define _USE_ALLOC   //�ù��Ƿ������������ȷ��ʹ�õ����ĸ�������
#include"simple_alloc.h"
using namespace std;
using namespace juine;
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
    //for(;i<3;i++)           //�μ������е�һ��ȱ�ݣ���ʵpָ��Ӧ��ֻ��ָ��5��int�ģ���֪����ô�ж�Խ����û
    construct(p,student(1,"Сzhang"));   //�����������
    construct(p+1,student(2,"Сlan"));
    construct(p+2,student(3,"Сs"));
    int i;
    for(i=0;i<3;i++)
    {
        cout<<p[i]<<endl;
        destroy(p);   //��������
    }
    ss.dealloc(p);  //�ͷ��ڴ�
    if(p) //�ж��Ƿ��ͷ��ڴ�
        cout<<p[0]<<endl;
}

