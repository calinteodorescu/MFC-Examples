/**
 *
 * �� �� ����MyButton.c
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

#include <wchar.h>
#include "MyButton.h"
#include "MyButton_InitSub.h"
#include "../CObject/CObject.h"
#include "../CButton/CButton.h"


VOID  TOG_MyButton(OBJECT* pObj);
static void  LoadBitmaps(OBJECT*  This);
static void  UnloadBitmaps(OBJECT*  This);


/*=====================+ OICC label +====================*/

/*<oicc>*/
/*<ibn> CButton </ibn>*/
/*<crt>*/
RESULT  CRT_MyButton(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic)
{
	extern  VOID  InitAIBofND(OBJECT* pND, MLDSN* pMsn, NUMIB nib);
	extern  VOID  InitABNofND(OBJECT* pND, MLDSN* pMsn, NUMBN nbn);
	extern  VOID  TOG_CButton(OBJECT* pObj);
	extern  VOID  TOG_CObject(OBJECT* pObj);
	extern  VOID  TOG_VObject(OBJECT* pObj);
	extern  VOID  TOG_OIOIC(OBJECT* pObj);


	UI32   ux_ND = 5; 	/* ND������*/
	UI32   ux_BN = 10; 	/* ��ND��BN����֮�͡�*/
	UI32   ux_IB = 4; 	/* ��ND��IBN����֮�͡�*/
	UI32   ux_CS = NumCR * 15; 	/* ��ND������CS����֮�͡�*/
	UI32   ux_EM = sizeof(EM_MYBUTTON) + sizeof(EM_CBUTTON) + sizeof(EM_COBJECT)
		 + sizeof(EM_VOBJECT) + sizeof(EM_OIOIC); 	/* ��ND��EM�ߴ�֮�͡�*/

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
		MLDSN    ArrMsn[4];
		BYTE*    p = pIC + sizeof(OBJECT) * ux_ND;


		/***  ȷ��AND��Ԫ��  ***/

		/** MyButton **/
		pND = (OBJECT*)pIC;
		TOG_MyButton(pND);
		pND->MSN = MSN_MYBUTTON;
		pND->OID = (*pOID)++;
		pND->po_AND = (OBJECT*)pIC;
		pND->NND = ux_ND;

		/** CButton **/
		TOG_CButton(++pND);
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

		/** MyButton **/
		pND = (OBJECT*)pIC;

		/* - AIB - */
		pND->ppo_AIB = (OBJECT**)p;
		ArrMsn[0] = MSN_CBUTTON;
		InitAIBofND(pND, ArrMsn, 1);
		p += sizeof(OBJECT*) * 1;

		/* - ABN - */
		pND->ppo_ABN = (OBJECT**)p;
		ArrMsn[0] = MSN_CBUTTON;
		ArrMsn[1] = MSN_COBJECT;
		ArrMsn[2] = MSN_VOBJECT;
		ArrMsn[3] = MSN_OIOIC;
		InitABNofND(pND, ArrMsn, 4);
		p += sizeof(OBJECT*) * 4;

		/* - ACS - */
		pND->pvr_ACS = (VR*)p;
		pND->NCS = NumCR * 1;
		p += sizeof(VR) * pND->NCS;

		/* - EM - */
		pND->pEM = (EM*)p;
		p += sizeof(EM_MYBUTTON);

		/** CButton **/
		++pND;

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
		pND->NCS = NumCR * 2;
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
		pND->NCS = NumCR * 3;
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
		pND->NCS = NumCR * 4;
		p += sizeof(VR) * pND->NCS;

		/* - EM - */
		pND->pEM = (EM*)p;
		p += sizeof(EM_VOBJECT);

		/** OIOIC **/
		++pND;

		/* - ACS - */
		pND->pvr_ACS = (VR*)p;
		pND->NCS = NumCR * 5;
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
* ���ƣ�MyButton_Open
*/
static  RESULT  MyButton_Open(OBJECT* This, const VR* pCaller)
{
	VR	             caller;
	EM_MYBUTTON*     pem;


	OBS_OBJECT_OPEN_;

	/* ��������ɽӿ��Լ�������*/

	caller.cr = pCaller->cr;
	caller.mr = This->OID;

	if(1 == This->RefCnt)
	{
		pem = (EM_MYBUTTON*)This->pEM;
		pem->pOIOIC = GetBN(This, MSN_OIOIC); 
		pem->pVObject = GetBN(This, MSN_VOBJECT); 
		pem->pCObject = GetBN(This, MSN_COBJECT); 
		pem->pCButton = GetBN(This, MSN_CBUTTON);

		CreateObject(MSN_CBUTTON, 0, 0, 1, &pem->ArrSUB[eButton1_MyButton]);
		CreateObject(MSN_CBUTTON, 0, 0, 1, &pem->ArrSUB[eButton2_MyButton]);
		CreateObject(MSN_CBUTTON, 0, 0, 1, &pem->ArrSUB[eButton3_MyButton]);

		LoadBitmaps(This);

		return RP_RCONE;
	}

	return R_P;
}

/**
*
* ���ƣ�MyButton_Input
*/
static  RESULT  MyButton_Input(OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INPUT;
}

/**
*
* ���ƣ�MyButton_Output
*/
static  RESULT  MyButton_Output(OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_OUTPUT;
}

/**
*
* ���ƣ�MyButton_IOput
*/
static  RESULT  MyButton_IOput(OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_IOPUT;
}

/**
*
* ���ƣ�MyButton_Interact0
*/
static  RESULT  MyButton_Interact0(OBJECT* This, ACTION Act, const VR* pCaller)
{
	VR                  caller;
	EM_MYBUTTON*        pem = (EM_MYBUTTON*)This->pEM;
	EM_VOBJECT*         pem_vob = (EM_VOBJECT*)pem->pVObject->pEM;
	int                 i;
	
    caller.cr = pCaller->cr;
	caller.mr = This->OID;

	switch(Act)
	{
	case CMD_PREPARE:
		for(i = 0; i<eNumSub_MyButton; i++)
		{
			switch(i)
			{
			case eButton1_MyButton:
			case eButton2_MyButton:
			case eButton3_MyButton:
				MyButton_InitSub_Button(This, i, &caller);
				break;
			default:
				break;
			}
		}

		for(i = 0; i<eNumSub_MyButton; i++)
			VO_Interact0(pem->ArrSUB[i], CMD_PREPARE, &caller);

		pem_vob->pFirstSbdt = pem->ArrSUB[eButton1_MyButton];
		VO_Interact0(pem->pVObject, CMD_PREPARE, &caller);
		return R_P;
	default:
		break;
	}


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT0;
}

/**
*
* ���ƣ�MyButton_Interact1
*/
static  RESULT  MyButton_Interact1(OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT1;
}

/**
*
* ���ƣ�MyButton_Interact2
*/
static  RESULT  MyButton_Interact2(OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{
	VR    caller;
	EM_MYBUTTON*        pem = (EM_MYBUTTON*)This->pEM;
	EM_VOBJECT*         pem_vob = (EM_VOBJECT*)pem->pVObject->pEM;
	POINT               pt;
	MOVE                mv;

	caller.cr = pCaller->cr;
	caller.mr = This->OID;

	switch(Act)
	{
	case MSG_WGI_FOCUSDOWN:
		assert(sizeof(POINT) == Qty);
		VO_Interact2(pem->pVObject, MSG_WGI_FOCUSDOWN, IStrm, sizeof(POINT), &caller);
		if(pem_vob->pCFDownSbdt != NULL)
		{
			pem->lastpt = *(POINT*)IStrm;
			VO_Interact0(pem_vob->pCFDownSbdt, MSG_WGI_ACTIVATE, &caller);
		}
		return R_P;
	case MSG_WGI_FOCUSONMOVE:
		assert(sizeof(POINT) == Qty);
		VO_Interact2(pem->pVObject, MSG_WGI_FOCUSONMOVE, IStrm, sizeof(POINT), &caller);
		if(pem_vob->pCFDownSbdt != NULL)
		{
			memset(&mv, 0, sizeof(MOVE));
			pt = *(POINT*)IStrm;
			mv.dxLR = mv.dxUL = pt.x - pem->lastpt.x;
			mv.dyLR = mv.dyUL = pt.y - pem->lastpt.y;
			VO_Interact2(pem_vob->pCFDownSbdt, CMD_WGI_MOVE, (BYTE*)&mv, sizeof(MOVE), &caller);
			
			pem->lastpt = pt;
		}
		return R_P;
	default:
		break;
	}


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT2;
}

/**
*
* ���ƣ�MyButton_Interact3
*/
static  RESULT  MyButton_Interact3(OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{

	/* ... */


	/* ��������ɽӿ��Լ�������*/

	SBO_OBJECT_INTERACT3;
}

/**
*
* ���ƣ�MyButton_Close
*/
static  RESULT  MyButton_Close(OBJECT* This, const VR* pCaller)
{
	VR                  caller;
	EM_MYBUTTON*        pem = (EM_MYBUTTON*)This->pEM;
	int                 i;
	
	OBS_OBJECT_CLOSE_;

	/* ��������ɽӿ��Լ�������*/
	
	caller.cr = pCaller->cr;
	caller.mr = This->OID;
	
	if(0 == This->RefCnt)
	{
		UnloadBitmaps(This);
		
		for(i = 0; i<eNumSub_MyButton; i++)
		{
			VO_Close(pem->ArrSUB[i], &caller);
			DestroyObject(pem->ArrSUB[i]); 
		}
		
		return RP_RCZERO;
	}

	return R_P;
}

/*========================+ TOG +========================*/

/**
*
* ���ƣ�TOG_MyButton
*/
VOID  TOG_MyButton(OBJECT* pObj)
{
	pObj->Open = MyButton_Open;
	pObj->Input = MyButton_Input;
	pObj->Output = MyButton_Output;
	pObj->IOput = MyButton_IOput;
	pObj->Interact0 = MyButton_Interact0;
	pObj->Interact1 = MyButton_Interact1;
	pObj->Interact2 = MyButton_Interact2;
	pObj->Interact3 = MyButton_Interact3;
	pObj->Close = MyButton_Close;
}

/*====================+ Function(s) +====================*/


/**
*
* NAME: LoadBitmaps
*/
static void  LoadBitmaps(OBJECT*  This)
{
	EM_MYBUTTON*   pem = (EM_MYBUTTON*)This->pEM;
	TCHAR          szFilePath[MAX_PATH+1];
	TCHAR*         p;

	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	p = wcsrchr(szFilePath, L'\\') + 1;
	p += swprintf(p, TEXT("%s"), TEXT("skins\\"));

	/*- Button -*/
	swprintf(p, TEXT("%s"), TEXT("Button_Normal.jpg"));
	pem->ArrHBITMAP[eButton_Normal_BMP_MyButton] = LoadPictureFile(szFilePath);

	swprintf(p, TEXT("%s"), TEXT("Button_FocusOver.jpg"));
	pem->ArrHBITMAP[eButton_FocusOver_BMP_MyButton] = LoadPictureFile(szFilePath);

	swprintf(p, TEXT("%s"), TEXT("Button_FocusOn.jpg"));
	pem->ArrHBITMAP[eButton_FocusOn_BMP_MyButton] = LoadPictureFile(szFilePath);

	swprintf(p, TEXT("%s"), TEXT("Button_Active.jpg"));
	pem->ArrHBITMAP[eButton_Active_BMP_MyButton] = LoadPictureFile(szFilePath);

	swprintf(p, TEXT("%s"), TEXT("Button_ABSInactive.jpg"));
	pem->ArrHBITMAP[eButton_ABSInactive_BMP_MyButton] = LoadPictureFile(szFilePath);

	return;
}

/**
*
* NAME: UnloadBitmaps
*/
static void  UnloadBitmaps(OBJECT*  This)
{
	EM_MYBUTTON*   pem = (EM_MYBUTTON*)This->pEM;
	int            i;

	for(i = 0; i < eNum_BMP_MyButton; i++)
	{
		if(pem->ArrHBITMAP[i])
			DeleteObject(pem->ArrHBITMAP[i]);
	}

	return;
}

