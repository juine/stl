/*
    simple_alloc.h
**  ����ʵ�ֶ���ͳһ�Ľӿڣ�����������Ӧ��ʹ���˵���ģʽ�����к�������������static����
*/

#ifndef SIMPLE_ALLOC_H_INCLUDED
#define SIMPLE_ALLOC_H_INCLUDED

#include<typeinfo>  //for typeid
#include<new>       //for placement new
#include"__malloc_alloc_template.h"
typedef  juine::__malloc_alloc_template  malloc_alloc;     //ע��һ�£�ֻ������д�� ˳�����Ҫ
#include"__default_alloc_template.h"

typedef juine::__default_alloc_template default_alloc;


// ָ�������������ͣ�ѡ����һ������������

#ifdef _USE_ALLOC
    typedef malloc_alloc my_alloc;
#else
    typedef default_alloc my_alloc;
#endif

namespace juine
{
    // �ú���������ʾʹ�õ�һ������ʱ����������out of memory������ʱ����Ĭ��ʹ�����º�����ָ�룩
    void fun(){std::cout<<"use default method to solve oom!"<<std::endl;}
    void (*temp)()=fun;

    template<class T,class Alloc=my_alloc>
    class simple_alloc
    {
    public:

    //��ALLOC����з�װ�������ṩͳһ�Ľӿ�
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
        void dealloc(T* &buff)
        {
            Alloc::dealloc(buff,sizeof(T));
            buff=NULL;  //�ͷ��ڴ��ָ��Ҫ��0
        }
        void dealloc(T* &buff,size_t n)
        {
            Alloc::dealloc(buff,n*sizeof(T));
            buff=NULL;
        }

        //�ƶ��쳣������ƣ�����ֻ��Ե�һ�����������
        simple_alloc()
        {
            if(typeid(Alloc)==typeid(malloc_alloc))  //��Ϊ�μ�����ʱ��Ϊ���ǲ������ṩ��һ��������Ľӿ�
                Alloc::get_function_handle()=temp;
        }
    };


    //���캯������
    template<class T,class T2>
    inline void construct(T *p,T2 value)
    {
        new(p) T(value);    //placement new ���÷�
    }

    //������������
    template<class T>
    inline void destroy(T *p)  // Ϊ�˼򵥣���ֻдһ��destroy
    {
        p->~T();
    }
}
#endif // SIMPLE_ALLOC_H_INCLUDED
