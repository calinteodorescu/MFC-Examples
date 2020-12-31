#pragma once


#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <mbstring.h>
#include <tchar.h>
#include <commctrl.h>
#include <iostream>
#include <map>
using namespace std;

#pragma warning(disable:4996)

//GDI+ ���� �����û�Ǵ�Ļ���װ���� Microsoft Platform SDK  2003 ��� SDK��������ġ�
#include "C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Include\\GdiPlus.h"
#pragma comment(lib,"C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A\\Lib\\GdiPlus.lib")
// ��GDI+����Դ�е���PNGλͼ
BOOL ImageFromIDResource( unsigned char * lpResStreamName,unsigned int len,Gdiplus::Image * &pImg );


void ClientToScreen( HWND hWnd,RECT * lpRect );
void ScreenToClient( HWND hWnd,RECT * lpRect );

// ����ԴHRGN������������ŵ���HRGN
HRGN CreateStretchRgn( HRGN hSrcRgn, 
                       float xScale , 
                       float yScale , 
                       int xOffset , 
                       int yOffset );

// �ý���ɫ������
void GradientFillRect( HDC hDC, RECT & rect, COLORREF col_from, COLORREF col_to, bool vert_grad );


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif
