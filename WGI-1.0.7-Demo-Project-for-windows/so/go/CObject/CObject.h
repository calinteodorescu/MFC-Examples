/**
 *
 * �� �� ����CObject.h
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

#ifndef __COBJECT_H__
#define __COBJECT_H__

#include "../go.h"
#include "../VObject/VObject.h"


/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_COBJECT
*/
typedef struct TAG_INIT_COBJECT
{
	const HWND*    pWndHandle;
}INIT_COBJECT;


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_COBJECT
*/
typedef struct TAG_EM_COBJECT
{
	OBJECT*     pOIOIC;
	OBJECT*     pVObject;
}EM_COBJECT;


#endif		/* #ifndef __COBJECT_H__ */
