/*
__malloc_alloc_template.h�ļ�
** ����ģ��STL�еĵ�һ���ռ�������������������ǽ������ڴ��빹�캯�����뿪��������allocatorֻ������������ٿռ䣬
����ԭʼmaolloc��free���������ڴ棬�Լ�����д�쳣������

*/
#ifndef __MALLOC_ALLOC_TEMPLATE_H_INCLUDED
#define __MALLOC_ALLOC_TEMPLATE_H_INCLUDED
#include<cstdlib>
#include<cstddef>
namespace juine
{
    class __malloc_alloc_template
    {
    //typedef void (*set_alloc_handle)();
    private:
        typedef void (*handle)();
        static handle set_alloc_handle; //�쳣������ָ�룬���û��ƶ��趨��ν���쳣
        static void* oom_malloc(size_t n);// �������ڴ�ʧ�ܵ�ʱ��,���øú������·����ڴ�
    public:
        static void* alloc(size_t n)
        {
            void *temp=malloc(n);
            temp=0;     //����ģ�Ŀ����Ϊ��ģ���out of memoryʱ����Щʲô
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
    void (*__malloc_alloc_template::set_alloc_handle)()=0;//  �������д ��Ȼ���溯���н���ʾset_alloc_handleû�ж���
    void* __malloc_alloc_template::oom_malloc(size_t n)
    {
        void *temp=0;
        while(!temp)
        {
            if(set_alloc_handle==0)
            {
                std::cout<<"you must find a methods to solve the memory problem "<<std::endl;
                //�׳��쳣Ԫ��
                exit(1);
            }
            set_alloc_handle();
            temp=malloc(n);
        }
        return temp;
    }

}

#endif // __MALLOC_ALLOC_TEMPLATE_H_INCLUDED
