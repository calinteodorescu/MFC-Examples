/**
 *
 * �� �� ����CButton.h
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

#ifndef __CBUTTON_H__
#define __CBUTTON_H__

#include "../go.h"


/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_CBUTTON
*/
/*typedef struct TAG_INIT_CBUTTON
{
}INIT_CBUTTON;*/


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_CBUTTON
*/
typedef struct TAG_EM_CBUTTON
{
	OBJECT*     pOIOIC;
	OBJECT*     pVObject;
	OBJECT*     pCObject;
}EM_CBUTTON;


#endif		/* #ifndef __CBUTTON_H__ */
