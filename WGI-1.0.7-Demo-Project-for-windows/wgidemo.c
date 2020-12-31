/**
 *
 * �� �� ����wgidemo.c
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

#include "wgidemo.h"


/*=====================+ OICC label +====================*/

/*<oicc>*/
/*<ibn> OIOIC </ibn>*/
/*<crt>*/
/*</crt>*/
/*</oicc>*/

/*=====================+ Interface +=====================*/

/**
*
* ���ƣ�wgidemo_Open
*/
static  RESULT  wgidemo_Open(OBJECT* This, const VR* pCaller)
{
	OBS_OBJECT_OPEN_;

	/* ��������ɽӿ��Լ�������*/


	if(1 == This->RefCnt)
		return RP_RCONE;


	return R_P;
}

/**
*
* ���ƣ�wgidemo_Input
*/
static  RESULT  wgidemo_Input(OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INPUT;
}

/**
*
* ���ƣ�wgidemo_Output
*/
static  RESULT  wgidemo_Output(OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_OUTPUT;
}

/**
*
* ���ƣ�wgidemo_IOput
*/
static  RESULT  wgidemo_IOput(OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_IOPUT;
}

/**
*
* ���ƣ�wgidemo_Interact0
*/
static  RESULT  wgidemo_Interact0(OBJECT* This, ACTION Act, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT0;
}

/**
*
* ���ƣ�wgidemo_Interact1
*/
static  RESULT  wgidemo_Interact1(OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT1;
}

/**
*
* ���ƣ�wgidemo_Interact2
*/
static  RESULT  wgidemo_Interact2(OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT2;
}

/**
*
* ���ƣ�wgidemo_Interact3
*/
static  RESULT  wgidemo_Interact3(OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT3;
}

/**
*
* ���ƣ�wgidemo_Close
*/
static  RESULT  wgidemo_Close(OBJECT* This, const VR* pCaller)
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
* ���ƣ�TOG_wgidemo
*/
VOID  TOG_wgidemo(OBJECT* pObj)
{
	pObj->Open = wgidemo_Open;
	pObj->Input = wgidemo_Input;
	pObj->Output = wgidemo_Output;
	pObj->IOput = wgidemo_IOput;
	pObj->Interact0 = wgidemo_Interact0;
	pObj->Interact1 = wgidemo_Interact1;
	pObj->Interact2 = wgidemo_Interact2;
	pObj->Interact3 = wgidemo_Interact3;
	pObj->Close = wgidemo_Close;
}

/*====================+ Function(s) +====================*/
/* ... */

