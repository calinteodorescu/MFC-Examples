#pragma  once
#include <cstdio>
#include <algorithm>
class String1
{
public:
    String1(const char *str = NULL); // ͨ�ù��캯��
    String1(const String1 &another); // �������캯��
    ~ String1(); // ��������
    String1 & operator=(const String1 &rhs); // ��ֵ����
private:
    char *m_data; // ���ڱ����ַ���
};

class String2
{
   char * str;
public:
   String2 & operator = (String2 const &s)
   {
      if (this != &s)
         String2(s).swap (*this); // Copy-constructor and non-throwing swap
      // Old resources are released with the destruction of the temporary above
      return *this;
   }

   String2 & operator = (String2 s) // the pass-by-value parameter serves as a temporary
   {
      s.swap (*this); // Non-throwing swap
      return *this;
   }// Old resources released when destructor of s is called.

   void swap (String2 &s) throw () // Also see non-throwing swap idiom
   {
      std::swap(this->str, s.str);
   }
};