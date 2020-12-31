/**
 *
 * �� �� ����WPanel.c
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

#include "WPanel.h"


RESULT  WPanel_I_Open(OBJECT* This, const VR* pCaller);


/*=====================+ OICC label +====================*/

/*<oicc>*/
/*<ibn> WObject </ibn>*/
/*<crt>*/
/*</crt>*/
/*</oicc>*/

/*=====================+ Interface +=====================*/

/**
*
* ���ƣ�WPanel_Open
*/
static  RESULT  WPanel_Open(OBJECT* This, const VR* pCaller)
{
	VR    caller;

	caller.cr = pCaller->cr;
	caller.mr = This->OID;

	OBS_OBJECT_OPEN_;

	/* ��������ɽӿ��Լ�������*/

	return WPanel_I_Open(This, &caller);
}

/**
*
* ���ƣ�WPanel_Input
*/
static  RESULT  WPanel_Input(OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INPUT;
}

/**
*
* ���ƣ�WPanel_Output
*/
static  RESULT  WPanel_Output(OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_OUTPUT;
}

/**
*
* ���ƣ�WPanel_IOput
*/
static  RESULT  WPanel_IOput(OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_IOPUT;
}

/**
*
* ���ƣ�WPanel_Interact0
*/
static  RESULT  WPanel_Interact0(OBJECT* This, ACTION Act, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT0;
}

/**
*
* ���ƣ�WPanel_Interact1
*/
static  RESULT  WPanel_Interact1(OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT1;
}

/**
*
* ���ƣ�WPanel_Interact2
*/
static  RESULT  WPanel_Interact2(OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT2;
}

/**
*
* ���ƣ�WPanel_Interact3
*/
static  RESULT  WPanel_Interact3(OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT3;
}

/**
*
* ���ƣ�WPanel_Close
*/
static  RESULT  WPanel_Close(OBJECT* This, const VR* pCaller)
{
	OBS_OBJECT_CLOSE_;

	/* ��������ɽӿ��Լ�������*/


	if(0 == This->RefCnt)
		return RP_RCZERO;


	return R_P;
}

/*========================+ TOG +========================*/

/**
*
* ���ƣ�TOG_WPanel
*/
VOID  TOG_WPanel(OBJECT* pObj)
{
	pObj->Open = WPanel_Open;
	pObj->Input = WPanel_Input;
	pObj->Output = WPanel_Output;
	pObj->IOput = WPanel_IOput;
	pObj->Interact0 = WPanel_Interact0;
	pObj->Interact1 = WPanel_Interact1;
	pObj->Interact2 = WPanel_Interact2;
	pObj->Interact3 = WPanel_Interact3;
	pObj->Close = WPanel_Close;
}

/*====================+ Function(s) +====================*/
/* ... */

/*=======================================================*/

/**
*
* NAME: WPanel_I_Open
*/
RESULT  WPanel_I_Open(OBJECT* This, const VR* pCaller)
{
	EM_WPANEL*      pem = (EM_WPANEL*)This->pEM;
	
	if(1 == This->RefCnt)
	{
		pem->pOIOIC = GetBN(This, MSN_OIOIC);
		pem->pVObject = GetBN(This, MSN_VOBJECT);
		pem->pWObject = GetBN(This, MSN_WOBJECT);

		return RP_RCONE;
	}
	
	return R_P;
}

