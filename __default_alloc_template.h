#ifndef __DEFAULT_ALLOC_TEMPLATE_H_INCLUDED
#define __DEFAULT_ALLOC_TEMPLATE_H_INCLUDED
#include<cstddef> // for size_t
#include<cstdlib>
using std::cout;
using std::endl;

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
    static char *start_pool;   //尽管为静态变量，初始化也应该在内的外面
    static char *end_pool;     //为char指针是为了刚好取得1byte内存
    static size_t poolSize;

    static obj *free_list[MAXSIZE];

    //将n转化为8的倍数
    static size_t ROUND_UP(size_t n)
    {
        return (((n)+__ALIGN-1)&~(__ALIGN-1));
    }
    //获得所在数组的下标
    static size_t POSITION(size_t n)
    {
        return (((n)+__ALIGN-1)/__ALIGN-1);
    }
    static size_t poolLeftSize()
    {
        return end_pool-start_pool;
    }
    //当free_lisk指针为0时，应该从内存池取出内存挂在在指针下面
    //，返回给用户的所需的内存地址（刷新指针数组所指的内存分布）
    static void* refill(size_t n) //此处n已是8的倍数
    {
        size_t objs=20;
        char *chunk=chunk_alloc(n,objs);
        if(objs==1)
            return chunk;
        obj *result,*current_obj,*next_obj;
        result=(obj*)chunk;   //这一块是用来返回给客户端的

        //接下来的是将获得的内存节点将其串起来，让以后分配更加方便
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
        std::cout<<"分配客户端后，"<<n<<"bytes大小的块还剩下："<<i<<std::endl;
        return result;

    }
    //分配内存池空间，然后返回指向数组指针的首地址
    static char* chunk_alloc(size_t n,size_t &objs)
    {
        char *result=NULL;
        size_t totalSize=n*objs;
        if(poolLeftSize()>totalSize)  //当内存池够大时一般是申请20块
        {
            result=start_pool;
            start_pool+=totalSize;
            cout<<"内存空间够大:得到"<<n<<"bytes块20个"<<endl;
            return result;    //只是返回首地址而已
        }
        else if(poolLeftSize()>=n)   //当内存池大于申请内存大小时
        {
            objs=poolLeftSize()/n;
            result=start_pool;
            start_pool+=objs*n;
            cout<<"内存空间比较大:得到"<<n<<"bytes块"<<objs<<"个"<<endl;
            return result;
        }
        else
        {
            //需要申请的内存
            size_t size_bytes_to_get=objs*n*2+ROUND_UP(poolSize>>4);
            std::cout<<"需要申请的内存大小为："<<size_bytes_to_get<<std::endl;
            if(poolLeftSize()>0) //当内存池中还有零头时，将其分配出去
            {
                obj *temp;
                temp=free_list[POSITION(poolLeftSize())];  //所剩内存一定是一个块！
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
    static void *alloc(size_t n)   //n为申请的内存大小
    {
        obj *result=NULL;
        //大于128时调用第一级配置器
        if(n>(size_t) __MAX_BYTES)
        {
            return (malloc_alloc::alloc(n));
        }
        //得到在free_list所在的位子
        result=free_list[POSITION(n)];
        if(result==NULL)
        {
            //调用refill，从内存池中来获得内存，将其挂到链表之下
            void *r=refill(ROUND_UP(n));
            return r;
        }
        cout<<"指针数组还有内存，已得到指针大小"<<endl;
        free_list[POSITION(n)]=result->next;
        return result;
    }

    //释放内存
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

    //无意义，纯粹为了标准接口
    typedef void (*handle)();
    static handle& get_function_handle()
    {
        handle p;
        return p;
    }

};

//初始化数据
char* __default_alloc_template::start_pool=NULL;
char* __default_alloc_template::end_pool=NULL;
size_t __default_alloc_template::poolSize=0;


obj* __default_alloc_template::free_list[MAXSIZE]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#endif // __DEFAULT_ALLOC_TEMPLATE_H_INCLUDED
