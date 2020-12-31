/**
 *
 * �� �� ����MyButton22.h
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

#ifndef __MYBUTTON2_H__
#define __MYBUTTON2_H__

#include "../go.h"

/*
*
*  subordinates' coordinates.
*  L - left, T - top. 
*/

#define   MYBUTTON2_BUTTON_L                 80   
#define   MYBUTTON2_BUTTON_T                 80 



/*
*
* NAME: ESMYBUTTON2
* DESC: enumerate subordinates of MyButton2
*/
typedef  enum  TAG_ESMYBUTTON2
{
    eButton_MyButton2,

	/*- number of enum - */
    eNumSub_MyButton2
}ESMYBUTTON2;

/*
*
* NAME: EBITMAP_MYBUTTON2
*/
typedef  enum  TAG_EBITMAP_MYBUTTON2
{
	/*- Button -*/
	eButton_Normal_BMP_MyButton2,
	eButton_FocusOver_BMP_MyButton2,
	eButton_FocusOn_BMP_MyButton2,
	eButton_Active_BMP_MyButton2,
	eButton_ABSInactive_BMP_MyButton2,

	/*- number of enum -*/
    eNum_BMP_MyButton2
}EBITMAP_MYBUTTON2;

/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_MYBUTTON2
*/
/*typedef struct TAG_INIT_MYBUTTON2
{
}INIT_MYBUTTON2;*/


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_MYBUTTON2
*/
typedef struct TAG_EM_MYBUTTON2
{
	OBJECT*     pOIOIC;
	OBJECT*     pVObject;
	OBJECT*     pCObject;
	OBJECT*     pCButton;
	HBITMAP     ArrHBITMAP[eNum_BMP_MyButton2];
	OBJECT*     ArrSUB[eNumSub_MyButton2];
	POINT       lastpt;
}EM_MYBUTTON2;


#endif		/* #ifndef __MYBUTTON2_H__ */
