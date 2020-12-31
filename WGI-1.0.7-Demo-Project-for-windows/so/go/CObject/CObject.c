/**
 *
 * �� �� ����CObject.c
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

#include "CObject.h"


RESULT  CObject_I_Open(OBJECT* This, const VR* pCaller);
RESULT  CObject__CMD_INITIALIZE(OBJECT* This, const INIT_COBJECT* picob, const VR* pCaller);
RESULT  CObject__CMD_WGI_MOVE(OBJECT* This, const MOVE* pmove, const VR* pCaller);


/*=====================+ OICC label +====================*/

/*<oicc>*/
/*<ibn> VObject </ibn>*/
/*<crt>*/
/*</crt>*/
/*</oicc>*/

/*=====================+ Interface +=====================*/

/**
*
* ���ƣ�CObject_Open
*/
static  RESULT  CObject_Open(OBJECT* This, const VR* pCaller)
{
	VR    caller;

	caller.cr = pCaller->cr;
	caller.mr = This->OID;

	OBS_OBJECT_OPEN_;

	/* ��������ɽӿ��Լ�������*/

	return CObject_I_Open(This, &caller);
}

/**
*
* ���ƣ�CObject_Input
*/
static  RESULT  CObject_Input(OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INPUT;
}

/**
*
* ���ƣ�CObject_Output
*/
static  RESULT  CObject_Output(OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_OUTPUT;
}

/**
*
* ���ƣ�CObject_IOput
*/
static  RESULT  CObject_IOput(OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_IOPUT;
}

/**
*
* ���ƣ�CObject_Interact0
*/
static  RESULT  CObject_Interact0(OBJECT* This, ACTION Act, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT0;
}

/**
*
* ���ƣ�CObject_Interact1
*/
static  RESULT  CObject_Interact1(OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT1;
}

/**
*
* ���ƣ�CObject_Interact2
*/
static  RESULT  CObject_Interact2(OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{
	VR    caller;

	caller.cr = pCaller->cr;
	caller.mr = This->OID;

	switch(Act)
	{
	case CMD_INITIALIZE:
		assert(sizeof(INIT_COBJECT) == Qty);
		return CObject__CMD_INITIALIZE(This, (INIT_COBJECT*)IStrm, &caller);
	case CMD_WGI_MOVE:
		assert(sizeof(MOVE) == Qty);
		return CObject__CMD_WGI_MOVE(This, (MOVE*)IStrm, &caller);
	default:
		break;
	}



	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT2;
}

/**
*
* ���ƣ�CObject_Interact3
*/
static  RESULT  CObject_Interact3(OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT3;
}

/**
*
* ���ƣ�CObject_Close
*/
static  RESULT  CObject_Close(OBJECT* This, const VR* pCaller)
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
* ���ƣ�TOG_CObject
*/
VOID  TOG_CObject(OBJECT* pObj)
{
	pObj->Open = CObject_Open;
	pObj->Input = CObject_Input;
	pObj->Output = CObject_Output;
	pObj->IOput = CObject_IOput;
	pObj->Interact0 = CObject_Interact0;
	pObj->Interact1 = CObject_Interact1;
	pObj->Interact2 = CObject_Interact2;
	pObj->Interact3 = CObject_Interact3;
	pObj->Close = CObject_Close;
}

/*====================+ Function(s) +====================*/
/* ... */

/*=======================================================*/

/**
*
* NAME: CObject_I_Open
*/
RESULT  CObject_I_Open(OBJECT* This, const VR* pCaller)
{
	EM_COBJECT*      pem = (EM_COBJECT*)This->pEM;
	
	if(1 == This->RefCnt)
	{
		pem->pOIOIC = GetBN(This, MSN_OIOIC);
		pem->pVObject = GetBN(This, MSN_VOBJECT);

		return RP_RCONE;
	}
	
	return R_P;
}

/**
*
* NAME: CObject__CMD_INITIALIZE
*/
RESULT  CObject__CMD_INITIALIZE(OBJECT* This, const INIT_COBJECT* picob, const VR* pCaller)
{
	EM_COBJECT*      pem = (EM_COBJECT*)This->pEM;
	EM_VOBJECT*      pem_vob = (EM_VOBJECT*)pem->pVObject->pEM;
	
	pem_vob->pWndHandle = picob->pWndHandle;
	
	return R_P;
}

/**
*
* NAME: CObject__CMD_WGI_MOVE
*/
RESULT  CObject__CMD_WGI_MOVE(OBJECT* This, const MOVE* pmove, const VR* pCaller)
{
	EM_COBJECT*      pem = (EM_COBJECT*)This->pEM;
	EM_VOBJECT*      pem_vob = (EM_VOBJECT*)pem->pVObject->pEM;
	

	pem_vob->rsc.rcouter.left += pmove->dxUL;
	pem_vob->rsc.rcouter.top += pmove->dyUL;
	pem_vob->rsc.rcouter.right += pmove->dxLR;
	pem_vob->rsc.rcouter.bottom += pmove->dyLR;
	pem_vob->rsc.z += pmove->dz;

	VO_Interact0(pem->pVObject, MAK_WGI_CURRENTBITMAP0, pCaller);
	VO_Interact0(pem->pVObject, CMD_WGI_REPAINTSUPERIOR, pCaller);
	
	return R_P;
}