/*
**
__default_alloc_template.h�ļ�
�μ��ռ�������
*/

#ifndef __DEFAULT_ALLOC_TEMPLATE_H_INCLUDED
#define __DEFAULT_ALLOC_TEMPLATE_H_INCLUDED
#include<cstddef> // for size_t
#include<cstdlib>
using std::cout;
using std::endl;
namespace juine
{
    enum {__ALIGN=8};
    enum {__MAX_BYTES=128};
    enum {MAXSIZE=__MAX_BYTES/__ALIGN};

    union obj
    {
        obj *next;
        char client_data[1];
    };
    class __default_alloc_template
    {
    public:
        static char *start_pool;   //����Ϊ��̬��������ʼ��ҲӦ�����ڵ�����
        static char *end_pool;     //Ϊcharָ����Ϊ�˸պ�ȡ��1byte�ڴ�
        static size_t poolSize;

        static obj *free_list[MAXSIZE];

        //��nת��Ϊ8�ı���
        static size_t ROUND_UP(size_t n)
        {
            return (((n)+__ALIGN-1)&~(__ALIGN-1));
        }
        //�������������±�
        static size_t POSITION(size_t n)
        {
            return (((n)+__ALIGN-1)/__ALIGN-1);
        }
        static size_t poolLeftSize()
        {
            return end_pool-start_pool;
        }
        //��free_liskָ��Ϊ0ʱ��Ӧ�ô��ڴ��ȡ���ڴ������ָ������
        //�����ظ��û���������ڴ��ַ��ˢ��ָ��������ָ���ڴ�ֲ���
        static void* refill(size_t n) //�˴�n����8�ı���
        {
            size_t objs=20;
            char *chunk=chunk_alloc(n,objs);
            if(objs==1)
                return chunk;
            obj *result,*current_obj,*next_obj;
            result=(obj*)chunk;   //��һ�����������ظ��ͻ��˵�

            //���������ǽ���õ��ڴ�ڵ㽫�䴮���������Ժ������ӷ���
            free_list[POSITION(n)]=next_obj=(obj*)(chunk+n);
            size_t i;
            for(i=1;;i++)
            {
                current_obj=next_obj;
                next_obj=(obj*)((char*)current_obj+n);
                if(objs-1==i)
                {
                     current_obj->next=NULL;
                     break;
                }
                current_obj->next=next_obj;
            }
            std::cout<<"����ͻ��˺�"<<n<<"bytes��С�Ŀ黹ʣ�£�"<<i<<std::endl;
            return result;

        }
        //�����ڴ�ؿռ䣬Ȼ�󷵻�ָ������ָ����׵�ַ
        static char* chunk_alloc(size_t n,size_t &objs)
        {
            char *result=NULL;
            size_t totalSize=n*objs;
            if(poolLeftSize()>totalSize)  //���ڴ�ع���ʱһ��������20��
            {
                result=start_pool;
                start_pool+=totalSize;
                cout<<"�ڴ�ռ乻��:�õ�"<<n<<"bytes��20��"<<endl;
                return result;    //ֻ�Ƿ����׵�ַ����
            }
            else if(poolLeftSize()>=n)   //���ڴ�ش��������ڴ��Сʱ
            {
                objs=poolLeftSize()/n;
                result=start_pool;
                start_pool+=objs*n;
                cout<<"�ڴ�ռ�Ƚϴ�:�õ�"<<n<<"bytes��"<<objs<<"��"<<endl;
                return result;
            }
            else
            {
                //��Ҫ������ڴ�
                size_t size_bytes_to_get=objs*n*2+ROUND_UP(poolSize>>4);
                std::cout<<"��Ҫ������ڴ��СΪ��"<<size_bytes_to_get<<std::endl;
                if(poolLeftSize()>0) //���ڴ���л�����ͷʱ����������ȥ
                {
                    obj *temp;
                    temp=free_list[POSITION(poolLeftSize())];  //��ʣ�ڴ�һ����һ���飡
                    free_list[POSITION(poolLeftSize())]=(obj*)start_pool;
                    free_list[POSITION(poolLeftSize())]->next=temp;
                    start_pool=end_pool=NULL;
                }

                start_pool=(char *)malloc(size_bytes_to_get);
                end_pool=start_pool+size_bytes_to_get;
                result=start_pool;
                start_pool+=n*objs;
                poolSize+=size_bytes_to_get;
                return result;
            }
        }
    public:
        static void *alloc(size_t n)   //nΪ������ڴ��С
        {
            obj *result=NULL;
            //����128ʱ���õ�һ��������
            if(n>(size_t) __MAX_BYTES)
            {
                return (malloc_alloc::alloc(n));
            }
            //�õ���free_list���ڵ�λ��
            result=free_list[POSITION(n)];
            if(result==NULL)
            {
                //����refill�����ڴ����������ڴ棬����ҵ�����֮��
                void *r=refill(ROUND_UP(n));
                return r;
            }
            cout<<"ָ�����黹���ڴ棬�ѵõ�ָ���С"<<endl;
            free_list[POSITION(n)]=result->next;
            return result;
        }

        //�ͷ��ڴ�
        static void dealloc(void *buff,size_t n)
        {
            if(n>(size_t) __MAX_BYTES)
            {
                malloc_alloc::dealloc(buff,n);
            }
            int pos=POSITION(n);
            ((obj*)buff)->next=free_list[pos];
            free_list[pos]=(obj*)buff;
        }

        //�����壬����Ϊ�˱�׼�ӿ�
        typedef void (*handle)();
        static handle& get_function_handle()
        {
            handle p;
            return p;
        }

    };

    //��ʼ������
    char* __default_alloc_template::start_pool=NULL;
    char* __default_alloc_template::end_pool=NULL;
    size_t __default_alloc_template::poolSize=0;


    obj* __default_alloc_template::free_list[MAXSIZE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}

#endif // __DEFAULT_ALLOC_TEMPLATE_H_INCLUDED
