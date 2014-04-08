//my_iterator_base.h
#ifndef MY_ITERATOR_H_INCLUDED
#define MY_ITERATOR_H_INCLUDED
#include<iostream>
#include<typeinfo>             //for typeid
using std::cout;
using std::endl;
namespace juine
{
    //5������������
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag:public input_iterator_tag,public output_iterator_tag{};
    struct bidirectional_iterator_tag:public forward_iterator_tag{};
    struct random_access_iterator_tag:public bidirectional_iterator_tag{};

    template<class T>
    class Iterator_base
    {
    public:
        //��Ϊ�������������Ԫ��(��ȡ5Ԫ��)
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef size_t difference_type;
        //iterator_category ��Ҫ���ݵ�������������ж�


        //���캯������������
        Iterator_base(T* p=0):point(p){}
        //copy���캯����ôд��
        Iterator_base(const Iterator_base& t):point(t.point){}
        ~Iterator_base(){delete point;}

        //ʵ��*��->�����ã�++
        value_type& operator*(){ return *point; }
        pointer operator->(){ return point; }
        Iterator_base operator++()
        {
            point++;
            return *this;
        }
        Iterator_base operator++(int)
        {
            Iterator_base tmp(*this);
            ++*this;
            return tmp;
        }

        bool operator!=(Iterator_base iter)
        {
            if(point==iter.point)
                return true;
            return false;
        }



    private:
        T* point;
    };

    //��ȡ���������еĸ�������
    template<class T>
    struct strait
    {
        //����д�������� ����ΪҪ�ͱ�׼��STLʵ���޷�����
        typedef typename T::value_type value_type;
        typedef typename T::reference reference;
        typedef typename T::difference_type difference_type;
        typedef typename T::pointer pointer;
        typedef typename T::iterator_category iterator_category;

    };
    //ƫ�ػ�
    template<class T>
    struct strait<T*>
    {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef size_t difference_type;
        typedef random_access_iterator_tag iterator_category;
    };

    template<class T>
    struct strait<const T*>
    {
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef size_t difference_type;
        typedef random_access_iterator_tag iterator_category;
    };

    //������������������֮advance����
    template<class InputIterator>
    void __advance(InputIterator& iter,size_t n,input_iterator_tag )
    {
        while(n--)
            iter++;
        cout<<"���õ���InputIterator������"<<endl;
    }

    template<class InputIterator>
    void __advance(InputIterator& iter,size_t n,forward_iterator_tag )
    {
        __advance(iter,n,InputIterator());
        cout<<"���õ���ForwardIterator������";
    }
    template<class InputIterator>
    void __advance(InputIterator& iter,size_t n,bidirectional_iterator_tag )
    {
        if(n>0)
            while(n-->0)
                iter++;
        else
            while(n++>=0)
                iter--;
        cout<<"���õ���BidirectionalIterator������"<<endl;
    }
    template<class InputIterator>
    void __advance(InputIterator& iter,size_t n,random_access_iterator_tag )
    {
        iter+=n;
        cout<<"���õ���RandomIterator������"<<endl;
    }

    //����ͳһ�Ľӿ�
    template<class InputIterator>
    void my_advance(InputIterator& iter,size_t n)
    {
        cout<<"����advance"<<endl;
        typedef typename strait<InputIterator>::iterator_category iterator_category;
        __advance(iter,n,iterator_category()); //�������⣬ʹ��STL ������ʱ�����õ��Ǳ�׼STL�е�__advance�����ع�ϵ��
    }

    //������������������֮distance����
    template<class InputIterator>
    typename strait<InputIterator>::difference_type
    __distance(InputIterator iter1,InputIterator iter2,input_iterator_tag)
    {
        typedef typename strait<InputIterator>::difference_type difference_type;
        difference_type length=0;
        while(iter1!=iter2)
        {
            iter1++;
            length++;
        }
        return length;
    }

    template<class InputIterator>
    typename strait<InputIterator>::difference_type
    __distance(InputIterator iter1,InputIterator iter2,forward_iterator_tag)
    {
       return __distance(iter1,iter2,input_iterator_tag());
    }

    template<class InputIterator>
    typename strait<InputIterator>::difference_type
    __distance(InputIterator iter1,InputIterator iter2,random_access_iterator_tag)
    {
        typedef typename strait<InputIterator>::difference_type difference_type;
        difference_type length=iter2-iter1;
        cout<<"mydistance������"<<endl;
        return length;
    }
    //����ʵ��ͳһ�Ľӿ�
    template<class InputIterator>
    typename strait<InputIterator>::difference_type my_distance(InputIterator iter1,InputIterator &iter2)
    {
        typedef typename strait<InputIterator>::iterator_category iterator_category;
        typedef typename strait<InputIterator>::difference_type difference_type;
        difference_type length=__distance(iter1,iter2,iterator_category());
        return length;
    }





}
#endif // MY_ITERATOR_H_INCLUDED
