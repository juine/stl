/*
**  该类实现对外统一的接口，其中配置器应该使用了单例模式，所有函数，变量都是static变量
*/

#ifndef SIMPLE_ALLOC_H_INCLUDED
#define SIMPLE_ALLOC_H_INCLUDED

#include<typeinfo>  //for typeid
#include<new>       //for placement new
#include"__malloc_alloc_template.h"
typedef __malloc_alloc_template malloc_alloc;     //注意一下，只能这样写， 顺序很重要
#include"__default_alloc_template.h"
typedef __default_alloc_template default_alloc;


// 指定配置器的类型，选择哪一个来进行配置

#ifdef _USE_ALLOC
    typedef malloc_alloc my_alloc;
#else
    typedef default_alloc my_alloc;
#endif


// 该函数用来表示使用第一配置器时，当不给定out of memory处理方法时，则默认使用如下函数（指针）
void fun(){std::cout<<"use default method to solve oom!"<<std::endl;}
void (*temp)()=fun;

template<class T,class Alloc=my_alloc>
class simple_alloc
{
public:

//对ALLOC其进行封装，对外提供统一的接口
    T* alloc(size_t n)
    {
        T* temp=(T*)Alloc::alloc(n);
        return temp;
    }
    T* alloc()
    {
         T* temp=(T*)Alloc::alloc(sizeof(T));
         return temp;
    }
    void dealloc(T* buff)
    {
        Alloc::dealloc(buff,sizeof(T));
    }
    void dealloc(T* buff,size_t n)
    {
        Alloc::dealloc(buff,n*sizeof(T));
    }

    //制定异常处理机制（但是只针对第一种配置情况）
    simple_alloc()
    {
        if(typeid(Alloc)==typeid(malloc_alloc))  //当为次级配置时，为了是不报错，提供了一个无意义的接口
            Alloc::get_function_handle()=temp;
    }
};


//构造函数工厂
template<class T,class T2>
inline void construct(T *p,T2 value)
{
    new(p) T(value);    //placement new 的用法
}

//析构函数工厂
template<class T>
inline void destroy(T *p)  // 为了简单，就只写一个destroy
{
    p->~T();
}

#endif // SIMPLE_ALLOC_H_INCLUDED
