/**
 *
 * �� �� ����WPanel.h
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

#ifndef __WPANEL_H__
#define __WPANEL_H__

#include "../go.h"
#include "../WObject/WObject.h"


#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))


/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_WPANEL
*/
/*typedef struct TAG_INIT_WPANEL
{
}INIT_WPANEL;*/


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_WPANEL
*/
typedef struct TAG_EM_WPANEL
{
	OBJECT*     pOIOIC;
	OBJECT*     pVObject;
	OBJECT*     pWObject;
}EM_WPANEL;


#endif		/* #ifndef __WPANEL_H__ */
