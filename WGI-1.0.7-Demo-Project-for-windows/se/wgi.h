/**
 *
 * �� �� ����wgi.h
 *
 * ��    ����
 *
 * �� �� �ߣ�
 *
 * �������ڣ�
 *
 * ��    ����1.0.8
 *
 * ��    ע��
 *
 *
 * * ά����ʷ *
 *
 *   <����>                <�޸���>
 *   <�޸�����...>
 *
 **/

#ifndef __WGI_H__
#define __WGI_H__

#include "se.h"


/*
*
* ���ƣ�POLYGON
* ����������νṹ��
*/
typedef struct TAG_POLYGON
{
	POINT*    pAPT;
	int       npt;
}POLYGON;


/*
*
* ���ƣ�ZPOLYGON
* ������Z�����νṹ�塣
*/
typedef struct TAG_ZPOLYGON
{
	POLYGON     plg;
	int         z;
}ZPOLYGON;


/*=======================================================*/

extern  HBITMAP  LoadPictureFile(const TCHAR* adrame);
extern  BOOL  IsPointInPolygon(int x, int y, const POLYGON*  pplg);
extern  BOOL  IsInter2Rects(const RECT* pSrcRect1, const RECT* pSrcRect2, RECT*  pDstRect);


#endif		/* #ifndef __WGI_H__ */