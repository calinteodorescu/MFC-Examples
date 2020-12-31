/**
 *
 * �� �� ����VObject.h
 *
 * ��    ����
 *
 * �� �� �ߣ���ƽ��
 *
 * �������ڣ�2011.08
 *
 * ��    ����1.0.7
 *
 * ��Ȩ(C)2005-2011 ��ƽ�ǡ���������Ȩ������
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

#ifndef __VOBJECT_H__
#define __VOBJECT_H__

#include "../go.h"


/*
*
* �ı���ʽ����ʽ����
*/
#define   TF_LEFT          DT_LEFT   // aligns text to the left
#define   TF_RIGHT         DT_RIGHT  // aligns text to the right
#define   TF_CENTER        DT_CENTER // centers text horizontally


/*
*
* ���Ӷ���ķ����
*/
#define   VOBS_IMMOVABLE           0x00000001L  // 1: Immovable, 0: Movable
#define   VOBS_XIRROLLABLE         0x00000002L  // 1: Irrollable By X, 0: Rollable By X.
#define   VOBS_YIRROLLABLE         0x00000004L  // 1: Irrollable By Y, 0: Rollable By Y.
#define   VOBS_INACTIVABLE         0x00000008L  // 1: Inactivable, 0: Activable
#define   VOBS_NOABSINACTIVE       0x00000010L  // 1: no the state of "eVOBState_ABSInactive", 0: has the state of "eVOBState_ABSInactive".


/*
*
* ���ƣ�EVOBSTATE
* ���������Ӷ���״̬ö��
*/
typedef  enum  TAG_EVOBSTATE
{
	eVOBState_Normal,
	eVOBState_FocusOver,
	eVOBState_FocusOn,
	eVOBState_Active,
	eVOBState_ABSInactive,
	eVOBState_Selected,
	eVOBState_Disable,
	eVOBState_Hidden,
	
	/*- number of state -*/
	eNumVOBState		
}EVOBSTATE;


/*
*
* ���ƣ�CAPTIONINFO
* ������������Ϣ
*/
typedef struct TAG_CAPTIONINFO
{
	TCHAR*     text;
	long       cpty;
	long       clen;
    RECT       rect;
    UINT       format;
	HFONT      font;
	COLORREF   color;
}CAPTIONINFO;



/*
*
* ���ƣ�RSCOORD
* ���������οռ�����ṹ��
*/
typedef  struct  TAG_RSCOORDINATE
{
	RECT        rcouter;
	RECT        rcinner;
	int         z;
}RSCOORD;


/*
*
* ���ƣ�ZPLGSLLND
* ������Z�����ε�������Singly Linked List���ڵ�ṹ��
*/
typedef struct TAG_ZPOLYGONSLLNODE
{
	ZPOLYGON                          zp;
	struct TAG_ZPOLYGONSLLNODE*       next;
}ZPLGSLLND;


/*
*
* ���ƣ�ATVCLNSLLNODE
* ������������ײ��active collision����������ڵ�ṹ��
*/
typedef struct TAG_ATVCOLLISIONSLLNODE
{
	OBJECT*                                pObj;
	ZPLGSLLND*                             pRgn;
	struct TAG_ATVCOLLISIONSLLNODE*        next;
}ATVCLNSLLNODE;


/*
*
* ���ƣ�MOVE
* �����������ƶ�ʱ��������
*/
typedef struct TAG_MOVE
{
	int               dxUL;
	int               dxLR;
	int               dyUL;
	int               dyLR;
	int               dz;         
	ATVCLNSLLNODE*    pacsn;
}MOVE;


/*
*
* ���ƣ�PSVCOLLISION
* ������������ײ��passive collision���ṹ��
*/
typedef struct TAG_PSVCOLLISION
{
	OBJECT*          pObj;
	ZPLGSLLND*       pRgn;
}PSVCOLLISION;


/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_VOBJECT
*/
typedef struct TAG_INIT_VOBJECT
{
	OBJECT*          pSuperior;
//	OBJECT*          pFirstSbdt;
	OBJECT*          pPrevCodt;
	OBJECT*          pNextCodt; 
//	OBJECT*          pFirstIIRSbdt;
//	OBJECT*          pPrevISIRCodt;
//	OBJECT*          pNextISIRCodt;
//	long             ox_irr;
//	long             oy_irr;
	long             maxw_irr;
	long             maxh_irr;
	HBITMAP          ArrHBITMAP[eNumVOBState];
	UI32             style;
	EVOBSTATE        state;
	CAPTIONINFO      ci;
	RSCOORD          rsc;
	ZPLGSLLND        as;
	ZPLGSLLND        rgn;
}INIT_VOBJECT;


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_VOBJECT
*/
typedef struct TAG_EM_VOBJECT
{
	OBJECT*          pOIOIC;
	OBJECT*          pSuperior;
	OBJECT*          pFirstSbdt;
	OBJECT*          pPrevCodt;
	OBJECT*          pNextCodt; 
	OBJECT*          pFirstIIRSbdt;
	OBJECT*          pPrevISIRCodt;
	OBJECT*          pNextISIRCodt;
	OBJECT*          pActiveSbdt;
	OBJECT*          pCFOverSbdt;
	OBJECT*          pCFDownSbdt;
	OBJECT*          pCFOnSbdt;
	OBJECT*          pCFUpSbdt;
	long             ox_irr;
	long             oy_irr;
	long             maxw_irr;
	long             maxh_irr;
	const HWND*      pWndHandle;
	HBITMAP          ArrHBITMAP[eNumVOBState];
	HBITMAP          hBmpCurrent;
	UI32             style;
	EVOBSTATE        state;
	CAPTIONINFO      ci;
	RSCOORD          rsc;
	ZPLGSLLND        as;
	ZPLGSLLND        rgn;
	POINT            lastpt;
}EM_VOBJECT;


#endif		/* #ifndef __VOBJECT_H__ */
