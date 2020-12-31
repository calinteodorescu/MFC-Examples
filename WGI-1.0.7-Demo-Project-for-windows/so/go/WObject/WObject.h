/**
 *
 * �� �� ����WObject.h
 *
 * ��    ����
 *
 * �� �� �ߣ�
 *
 * �������ڣ�
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

#ifndef __WOBJECT_H__
#define __WOBJECT_H__

#include "../go.h"
#include "../VObject/VObject.h"


/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_WOBJECT
*/
typedef struct TAG_INIT_WOBJECT
{
	HINSTANCE        hInstance;
	const TCHAR*     pClassName;
	const TCHAR*     pWindowName;
}INIT_WOBJECT;


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_WOBJECT
*/
typedef struct TAG_EM_WOBJECT
{
	OBJECT*          pOIOIC;
	OBJECT*          pVObject;
	HINSTANCE        hInstance;
	HWND             hWnd;
	const  TCHAR*    pClassName;
	const  TCHAR*    pWindowName;
}EM_WOBJECT;


#endif		/* #ifndef __WOBJECT_H__ */
