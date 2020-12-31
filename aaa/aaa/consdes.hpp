#ifndef _CONSDES_
#define _CONSDES_
#include <iostream>
using namespace std;
class Bi
{
public:
    Bi()
    {
        cout<<"default constructor"<<endl;
    }
    ~Bi()
    {
        cout<<"destructed"<<endl;
    }
    Bi(int i):data(i) //Bi(int) works as a converter ( int -> instance of Bi)
    {
        cout<<"constructed by parameter " << data <<endl;
    }
    Bi(const Bi& b)
    {
        cout<<"copy constructor by parameter "<<b.data<<endl;
    }
private:
    int data;
};


struct CLS
{
    int m_i;
    CLS( int i ) : m_i(i) {}
    CLS()
    {
        CLS(0);
    }
};

Bi Play(Bi b);
int testB1(int a ) ;
int testB2(int b ) ;

int testC1(void) ;

class Empty
{
public:
     Empty();                // ȱʡ���캯��
     Empty( const Empty& );  // �������캯��
     ~Empty();               // ��������
     Empty& operator=( const Empty& );  // ��ֵ�����
     Empty* operator&();                // ȡַ�����
     const Empty* operator&() const;    // ȡַ�����const
};


#endif /* _CONSDES_ */