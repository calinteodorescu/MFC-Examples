#ifndef _CALENDAR_H
#define _CALENDAR_H
 
#include <windows.h>
 
struct myDATE{
 int  year;
 int  month;
 int  day;
 bool isRunYue;
};
 
class calendar{
public:
 calendar(){};
 ~calendar(){};
 
  LONG lDaysFrom1900(myDATE date);//date��1900����������
  UINT lYearDays(UINT y);//ũ��y���������
  UINT leapDays(UINT y);//����ũ�� y�����µ�����
  UINT leapMonth(UINT y);// ����ũ�� y�����ĸ��� 1-12 , û�򴫻� 0
  UINT monthDays(UINT y,UINT m);//����ũ�� y��m�µ�������
  myDATE Lunar(myDATE date);//�����������ڣ�������������
  char* dayOfWeek(myDATE date);//�����������ڣ��������ڼ�
  char* getLunarString (myDATE date );// ������������,�õ���ʾũ�����ִ�
  UINT solarDays(UINT y,UINT m);//���ع��� y��ĳm+1�µ�����
 
 
private:
 char                    m_slunar[100];//���ڴ洢ũ����Ϣ
 
 static DWORD   lunarInfo[];//
 static UINT    solarMonth[];
 static char    sSolarTerm[];
 static DWORD   dTermInfo[];
 static char    sFtv[];
 
};
#endif