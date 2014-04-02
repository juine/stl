/*
** 用来模拟STL中的第一级空间配置器，充分体现了是将申请内存与构造函数分离开来处理，该allocator只负责申请和销毁空间，
采用原始maolloc和free函数管理内存，自己动手写异常处理函数

*/
#ifndef __MALLOC_ALLOC_TEMPLATE_H_INCLUDED
#define __MALLOC_ALLOC_TEMPLATE_H_INCLUDED
#include<cstdlib>
#include<cstddef>

class __malloc_alloc_template
{
//typedef void (*set_alloc_handle)();
private:
    typedef void (*handle)();
    static handle set_alloc_handle; //异常处理函数指针，由用户制定设定如何解决异常
    static void* oom_malloc(size_t n);// 当分配内存失败的时候,调用该函数重新分配内存
public:
    static void* alloc(size_t n)
    {
        void *temp=malloc(n);
        temp=0;     //故意的，目的是为了模拟出out of memory时该做些什么
        if(!temp)
            temp=oom_malloc(n);
        return temp;

    }
    static void dealloc(void* buff,size_t /*n*/)
    {
        free(buff);
    }
    static void set_function_handle( void (*temp)())
    {
        set_alloc_handle=temp;
    }
    static handle& get_function_handle()
    {
        return set_alloc_handle;
    }

};
void (*__malloc_alloc_template::set_alloc_handle)()=0;//  这个必须写 不然下面函数中将显示set_alloc_handle没有定义
void* __malloc_alloc_template::oom_malloc(size_t n)
{
    void *temp=0;
    while(!temp)
    {
        if(set_alloc_handle==0)
        {
            std::cout<<"you must find a methods to solve the memory problem "<<std::endl;
            //抛出异常元素
            exit(1);
        }
        set_alloc_handle();
        temp=malloc(n);
    }
    return temp;
}



#endif // __MALLOC_ALLOC_TEMPLATE_H_INCLUDED
