/**
 *
 * �� �� ����ho.c
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

#include "ho.h"


/*====================+ Definition +=====================*/

/*
*
* ���ƣ�DOID
* ��������¼�����ٶ����OID��
* ��ע��DOID��Destroyed Object's ID��
*/
typedef struct TAG_DOID
{
	MLDSN  msn;
	OBJID  oid;
}DOID;

/*
*
* ���ƣ�TARRDOID
* ������DOID����ṹ�塣
*/
typedef struct TAG_ARRDOID
{
	SI32    neo; /* ���ڵĶ����������û�����ٵĶ��������eo - existent object */
	DOID*   pad; /* ָ��Ԫ������ΪDOID�����飬������鶯̬���䡣ad - the array of DOID */
	SI32    lad; /* pad��ָ����Ŀռ䳤�ȣ���Ԫ��Ϊ��λ��lad - the length of array of DOID */
	SI32    nwe; /* pad��ָ����������д����Ԫ�ظ�����nwe - the number of written element */
	OBJID   oid; /* ����CRT������OID��*/
}TARRDOID;

/*
*
* ���ƣ�STP_GLAD
* �������꣬DOID����ռ�����������������Ԫ��Ϊ��λ��
* ��ע��STP - step��LAD - the length of array of DOID��GLAD - the growth of LAD��
*/
#define    STP_GLAD        10


/*====================+ Variable(s) +====================*/

static  TARRDOID   gs_tad = {0, NULL, 0, 0, OID_};


/*=================+ __CreateObject__ +==================*/

/**
*
* ���ƣ�__CreateObject__
* ��������������
* ������Msn --- [IN] ģ��ţ�ָ�������ĸ�ģ�Ķ���
*       IQCty --- [IN] ������е�������
*       OQCty --- [IN] ������е�������
*       NumCR --- [IN] ������������
*       ppObj --- [OUT] ��������Ķ���
*       pExotic --- [IN] ������ΪIC����Ŀռ䣬�����ڴ�����̬�����纯���ڵľֲ����󣩡�
* ���أ��ɹ�����P�����ʧ�ܷ���N�����
* ��ע��1. NumCR�������0��
*       2. ppObj��pExotic���ܶ�ΪNULL���NULL��
*       3. ���pExotic����NULL����ʾIC�ռ��������������ˡ�
*/
RESULT  __CreateObject__(MLDSN Msn, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic)
{
	/* extern  RESULT  CRT_xxx(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic); */
	extern  RESULT  CRT_MyDialog(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic);
	extern  RESULT  CRT_CButton(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic);
	extern  RESULT  CRT_MyButton(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic);
	extern  RESULT  CRT_MyButton2(OBJID* pOID, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic);


	RESULT        fr;
	OBJID         oidbak;

	EM_OIOIC*     pem;
	SI32          idd; /* DOID��ָ�ꡣidd - the index of DOID */

	assert( NumCR > 0 );
	assert( !(NULL == ppObj && NULL == pExotic) );
	assert( !(ppObj != NULL && pExotic != NULL) );

	oidbak = gs_tad.oid;

	for(idd = 0; idd < gs_tad.nwe; idd++)
	{
		if((Msn == (gs_tad.pad+idd)->msn) && ((gs_tad.pad+idd)->oid != 0))
		{
			gs_tad.oid = (gs_tad.pad+idd)->oid;
			break;
		}
	}

	switch(Msn)
	{
		case MSN_MYDIALOG:
			fr = CRT_MyDialog(&gs_tad.oid, IQCty, OQCty, NumCR, ppObj, pExotic);
			break;
		case MSN_CBUTTON:
			fr = CRT_CButton(&gs_tad.oid, IQCty, OQCty, NumCR, ppObj, pExotic);
			break;
		case MSN_MYBUTTON:
			fr = CRT_MyButton(&gs_tad.oid, IQCty, OQCty, NumCR, ppObj, pExotic);
			break;
		case MSN_MYBUTTON2:
			fr = CRT_MyButton2(&gs_tad.oid, IQCty, OQCty, NumCR, ppObj, pExotic);
			break;
		default:
			fr = R_N;
			break;
	}

	if(!RN(fr))
	{
		if(idd < gs_tad.nwe)
		{
			gs_tad.oid = oidbak;
			(gs_tad.pad+idd)->oid = 0;
		}else if((MSKSO & gs_tad.oid) < (MSKSO & oidbak))
		{
			gs_tad.oid = oidbak;

			pem = (EM_OIOIC*)GetEMofBN(*ppObj, MSN_OIOIC);
			if(pem->IQ.Dtrm != NULL)
				free(pem->IQ.Dtrm);
			if(pem->OQ.Dtrm != NULL)
				free(pem->OQ.Dtrm);

			if(NULL == pExotic)
			{
				free(*ppObj);
				*ppObj = NULL;
			}

			return RN_LACKOID;
		}

		gs_tad.neo++;

	}else
	{
		gs_tad.oid = oidbak;
	}

	return  fr;
}

/*=================+ __DestroyObject__ +=================*/

/*
*
* ���ƣ�__DestroyObject__
* ���������ٶ���
* ������pObj --- [IN] ָ�����
* ���أ�TRUE - �ɹ���FALSE - ʧ�ܡ�
* ��ע����̬�����纯���ڵľֲ�����Ҳ���ô˺������١�
*/
BOOL  __DestroyObject__(OBJECT*  pObj)
{
	DOID*       ptr;
	SI32        idd;   /* DOID��ָ�ꡣidd - the index of DOID */
	EM_OIOIC*   pem;

	assert( pObj != NULL );

	pem = (EM_OIOIC*)GetEMofBN(pObj, MSN_OIOIC);

	if(NULL == gs_tad.pad)
	{
		if(gs_tad.neo-1 > 0)
		{
			gs_tad.pad = (DOID*)calloc(STP_GLAD, sizeof(DOID));
			if(NULL == gs_tad.pad)
				return FALSE;
			gs_tad.pad->msn = pObj->MSN;
			gs_tad.pad->oid = pObj->OID;
			gs_tad.lad = STP_GLAD;
			gs_tad.nwe = 1;
		}else if(0 == gs_tad.neo-1)
		{
			gs_tad.oid = OID_;
		}else
		{
			return FALSE;
		}
	}else
	{
		for(idd = 0; idd < gs_tad.nwe; idd++)
		{
			if((pObj->MSN == (gs_tad.pad+idd)->msn) && (0 == (gs_tad.pad+idd)->oid))
			{
				(gs_tad.pad+idd)->oid = pObj->OID;
				break;
			}
		}

		if(idd == gs_tad.nwe)
		{
			if(idd == gs_tad.lad)
			{
				ptr = (DOID*)calloc(gs_tad.lad + STP_GLAD, sizeof(DOID));
				if(NULL == ptr)
					return FALSE;
				memcpy(ptr, gs_tad.pad, gs_tad.nwe*sizeof(DOID));
				free(gs_tad.pad);
				gs_tad.pad = ptr;
				gs_tad.lad += STP_GLAD;
			}
			(gs_tad.pad+gs_tad.nwe)->msn = pObj->MSN;
			(gs_tad.pad+gs_tad.nwe)->oid = pObj->OID;
			gs_tad.nwe++;
		}
	}

	if(pem->IQ.Dtrm != NULL)
		free(pem->IQ.Dtrm);
	if(pem->OQ.Dtrm != NULL)
		free(pem->OQ.Dtrm);

	gs_tad.neo--;

	if(gs_tad.pad != NULL && 0 == gs_tad.neo)
	{
		free(gs_tad.pad);
		gs_tad.pad = NULL;
		gs_tad.lad = 0;
		gs_tad.nwe = 0;
		gs_tad.oid = OID_;
	}

	return TRUE;
}


/*=============+ Global object pointer(s) +==============*/

OBJECT*   g_poMainWnd = NULL;
