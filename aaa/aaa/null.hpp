#pragma  once
#include <string> 
const                             //  ����һ��const����//...
class {
public:
    template<class T>               //  ����ת���κ�����
    operator T*() const           //  �� null�ǳ�Աָ��
    { return 0; }                 //
    template<class C, class T>      //  ����ת���κ�����
    operator T C::*() const       //  �� null��Աָ��
    { return 0; }
private:
    void operator&() const;         //  ����ȡ���ַ
                                  // (������27)
} NullClass;                           //  ����Ϊ NULL

void f(int x);                    //  ����ǰһ��
void f(std::string *p);                //  ͬ��

void testNull();


#include <typeinfo>

const // It is a const object...
class nullptr_t_ 
{
public:
   template<class T>
   inline operator T*() const // convertible to any type of null non-member pointer...
   { return 0; }

   template<class C, class T>
   inline operator T C::*() const   // or any type of null member pointer...
   { return 0; }

private:
   void operator&() const;  // Can't take address of nullptr

} nullptr_ = {};

struct Cnull
{
   void func();
};

template<typename T> 
void g( T* t ) {}

template<typename T> 
void h( T t ) {}
