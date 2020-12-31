/**
 *
 * �� �� ����CButton.c
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

#include "CButton.h"
#include "../CObject/CObject.h"


VOID  TOG_CButton(OBJECT* pObj);

RESULT  CButton_I_Open(OBJECT* This, const VR* pCaller);


/*=====================+ OICC label +====================*/

/*<oicc>*/
/*<ibn> CObject </ibn>*/
/*<crt>*/
RESULT  CRT_CButton(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic)
{
	extern  VOID  InitAIBofND(OBJECT* pND, MLDSN* pMsn, NUMIB nib);
	extern  VOID  InitABNofND(OBJECT* pND, MLDSN* pMsn, NUMBN nbn);
	extern  VOID  TOG_CObject(OBJECT* pObj);
	extern  VOID  TOG_VObject(OBJECT* pObj);
	extern  VOID  TOG_OIOIC(OBJECT* pObj);


	UI32   ux_ND = 4; 	/* ND������*/
	UI32   ux_BN = 6; 	/* ��ND��BN����֮�͡�*/
	UI32   ux_IB = 3; 	/* ��ND��IBN����֮�͡�*/
	UI32   ux_CS = NumCR * 10; 	/* ��ND������CS����֮�͡�*/
	UI32   ux_EM = sizeof(EM_CBUTTON) + sizeof(EM_COBJECT) + sizeof(EM_VOBJECT)
		 + sizeof(EM_OIOIC); 	/* ��ND��EM�ߴ�֮�͡�*/

	UI32  sum = ux_ND * sizeof(OBJECT) 	/* for AND. */
		 + ux_BN * sizeof(OBJECT*) 	/* for "ppo_ABN". */
		 + ux_IB * sizeof(OBJECT*) 	/* for "ppo_AIB". */
		 + ux_CS * sizeof(VR) + ux_EM; 	/* for "pvr_ACS" and "pEM". */


	BYTE*  pIC = NULL;

	if(NULL == pExotic)
	{
		*ppObj = NULL;
		pIC = (BYTE*)calloc(sum, 1);
	}else
	{
		pIC = pExotic;
	}


	if( pIC != NULL )
	{
		OBJECT*  pND;
		MLDSN    ArrMsn[3];
		BYTE*    p = pIC + sizeof(OBJECT) * ux_ND;


		/***  ȷ��AND��Ԫ��  ***/

		/** CButton **/
		pND = (OBJECT*)pIC;
		TOG_CButton(pND);
		pND->MSN = MSN_CBUTTON;
		pND->OID = (*pOID)++;
		pND->po_AND = (OBJECT*)pIC;
		pND->NND = ux_ND;

		/** CObject **/
		TOG_CObject(++pND);
		pND->MSN = MSN_COBJECT;
		pND->OID = (*pOID)++;
		pND->po_AND = (OBJECT*)pIC;
		pND->NND = ux_ND;

		/** VObject **/
		TOG_VObject(++pND);
		pND->MSN = MSN_VOBJECT;
		pND->OID = (*pOID)++;
		pND->po_AND = (OBJECT*)pIC;
		pND->NND = ux_ND;

		/** OIOIC **/
		TOG_OIOIC(++pND);
		pND->MSN = MSN_OIOIC;
		pND->OID = (*pOID)++;
		pND->po_AND = (OBJECT*)pIC;
		pND->NND = ux_ND;


		/***  ��ʼ����ND  ***/

		/** CButton **/
		pND = (OBJECT*)pIC;

		/* - AIB - */
		pND->ppo_AIB = (OBJECT**)p;
		ArrMsn[0] = MSN_COBJECT;
		InitAIBofND(pND, ArrMsn, 1);
		p += sizeof(OBJECT*) * 1;

		/* - ABN - */
		pND->ppo_ABN = (OBJECT**)p;
		ArrMsn[0] = MSN_COBJECT;
		ArrMsn[1] = MSN_VOBJECT;
		ArrMsn[2] = MSN_OIOIC;
		InitABNofND(pND, ArrMsn, 3);
		p += sizeof(OBJECT*) * 3;

		/* - ACS - */
		pND->pvr_ACS = (VR*)p;
		pND->NCS = NumCR * 1;
		p += sizeof(VR) * pND->NCS;

		/* - EM - */
		pND->pEM = (EM*)p;
		p += sizeof(EM_CBUTTON);

		/** CObject **/
		++pND;

		/* - AIB - */
		pND->ppo_AIB = (OBJECT**)p;
		ArrMsn[0] = MSN_VOBJECT;
		InitAIBofND(pND, ArrMsn, 1);
		p += sizeof(OBJECT*) * 1;

		/* - ABN - */
		pND->ppo_ABN = (OBJECT**)p;
		ArrMsn[0] = MSN_VOBJECT;
		ArrMsn[1] = MSN_OIOIC;
		InitABNofND(pND, ArrMsn, 2);
		p += sizeof(OBJECT*) * 2;

		/* - ACS - */
		pND->pvr_ACS = (VR*)p;
		pND->NCS = NumCR * 2;
		p += sizeof(VR) * pND->NCS;

		/* - EM - */
		pND->pEM = (EM*)p;
		p += sizeof(EM_COBJECT);

		/** VObject **/
		++pND;

		/* - AIB - */
		pND->ppo_AIB = (OBJECT**)p;
		ArrMsn[0] = MSN_OIOIC;
		InitAIBofND(pND, ArrMsn, 1);
		p += sizeof(OBJECT*) * 1;

		/* - ABN - */
		pND->ppo_ABN = (OBJECT**)p;
		ArrMsn[0] = MSN_OIOIC;
		InitABNofND(pND, ArrMsn, 1);
		p += sizeof(OBJECT*) * 1;

		/* - ACS - */
		pND->pvr_ACS = (VR*)p;
		pND->NCS = NumCR * 3;
		p += sizeof(VR) * pND->NCS;

		/* - EM - */
		pND->pEM = (EM*)p;
		p += sizeof(EM_VOBJECT);

		/** OIOIC **/
		++pND;

		/* - ACS - */
		pND->pvr_ACS = (VR*)p;
		pND->NCS = NumCR * 4;
		p += sizeof(VR) * pND->NCS;

		/* - EM - */
		pND->pEM = (EM*)p;
		if( IQCty > 0 )
		{
			/* - IQ - */
			((EM_OIOIC*)(pND->pEM))->IQ.Dtrm = (BYTE*)calloc(IQCty, 1);
			if( NULL == ((EM_OIOIC*)(pND->pEM))->IQ.Dtrm)
			{
				/* ����IQ������ʧ�ܣ�*/
				if(NULL == pExotic)
					free(pIC);
				return R_N;
			}
			((EM_OIOIC*)(pND->pEM))->IQ.Front = ((EM_OIOIC*)(pND->pEM))->IQ.Rear = ((EM_OIOIC*)(pND->pEM))->IQ.Dtrm - 1;
			((EM_OIOIC*)(pND->pEM))->IQ.Cty = IQCty;
			((EM_OIOIC*)(pND->pEM))->IQ.Qty = ((EM_OIOIC*)(pND->pEM))->IQ.Lost = 0;
		}

		if( OQCty > 0 )
 		{
			/* - OQ - */
			((EM_OIOIC*)(pND->pEM))->OQ.Dtrm = (BYTE*)calloc(OQCty, 1);
			if(NULL == ((EM_OIOIC*)(pND->pEM))->OQ.Dtrm)
			{
				/* ����OQ������ʧ�ܣ�*/
				if( ((EM_OIOIC*)(pND->pEM))->IQ.Dtrm != NULL )
					free(((EM_OIOIC*)(pND->pEM))->IQ.Dtrm);
				if(NULL == pExotic)
					free(pIC);
				return R_N;
			}
			((EM_OIOIC*)(pND->pEM))->OQ.Front = ((EM_OIOIC*)(pND->pEM))->OQ.Rear = ((EM_OIOIC*)(pND->pEM))->OQ.Dtrm - 1;
			((EM_OIOIC*)(pND->pEM))->OQ.Cty = OQCty;
			((EM_OIOIC*)(pND->pEM))->OQ.Qty = ((EM_OIOIC*)(pND->pEM))->OQ.Lost = 0;
		}
		p += sizeof(EM_OIOIC);
	}else
	{
		return R_N;
	}


	if(NULL == pExotic)
		*ppObj = (OBJECT*)pIC;


	return R_P;
}
/*</crt>*/
/*</oicc>*/

/*=====================+ Interface +=====================*/

/**
*
* ���ƣ�CButton_Open
*/
static  RESULT  CButton_Open(OBJECT* This, const VR* pCaller)
{
	VR    caller;

	caller.cr = pCaller->cr;
	caller.mr = This->OID;

	OBS_OBJECT_OPEN_;

	/* ��������ɽӿ��Լ�������*/

	return CButton_I_Open(This, &caller);
}

/**
*
* ���ƣ�CButton_Input
*/
static  RESULT  CButton_Input(OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INPUT;
}

/**
*
* ���ƣ�CButton_Output
*/
static  RESULT  CButton_Output(OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_OUTPUT;
}

/**
*
* ���ƣ�CButton_IOput
*/
static  RESULT  CButton_IOput(OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_IOPUT;
}

/**
*
* ���ƣ�CButton_Interact0
*/
static  RESULT  CButton_Interact0(OBJECT* This, ACTION Act, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT0;
}

/**
*
* ���ƣ�CButton_Interact1
*/
static  RESULT  CButton_Interact1(OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT1;
}

/**
*
* ���ƣ�CButton_Interact2
*/
static  RESULT  CButton_Interact2(OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT2;
}

/**
*
* ���ƣ�CButton_Interact3
*/
static  RESULT  CButton_Interact3(OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT3;
}

/**
*
* ���ƣ�CButton_Close
*/
static  RESULT  CButton_Close(OBJECT* This, const VR* pCaller)
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
* ���ƣ�TOG_CButton
*/
VOID  TOG_CButton(OBJECT* pObj)
{
	pObj->Open = CButton_Open;
	pObj->Input = CButton_Input;
	pObj->Output = CButton_Output;
	pObj->IOput = CButton_IOput;
	pObj->Interact0 = CButton_Interact0;
	pObj->Interact1 = CButton_Interact1;
	pObj->Interact2 = CButton_Interact2;
	pObj->Interact3 = CButton_Interact3;
	pObj->Close = CButton_Close;
}

/*====================+ Function(s) +====================*/
/* ... */

/*=======================================================*/

/**
*
* NAME: CButton_I_Open
*/
RESULT  CButton_I_Open(OBJECT* This, const VR* pCaller)
{
	EM_CBUTTON*      pem = (EM_CBUTTON*)This->pEM;
	
	if(1 == This->RefCnt)
	{
		pem->pOIOIC = GetBN(This, MSN_OIOIC);
		pem->pVObject = GetBN(This, MSN_VOBJECT);
		pem->pCObject = GetBN(This, MSN_COBJECT);

		return RP_RCONE;
	}
	
	return R_P;
}
