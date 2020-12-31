/**
 *
 * �� �� ����oioic.c
 *
 * ��    ����OIOIC �ڵ�� .c �ļ���
 *
 * �� �� �ߣ���ƽ��   <bruce.zhao.zpz@gmail.com>
 *
 * �������ڣ�20050310
 *
 * ��    ����1.0.18
 *
 * ��Ȩ(C)2005-2011 ��ƽ�ǡ���������Ȩ����������
 *
 * ��    ע��
 *
 *
 * * ά����ʷ *
 *
 *   <����>              <�޸���>
 *   <�޸�����...>
 *
 **/

#include <stdlib.h>
#include <string.h> 
#include "oioic.h"


/*=====================+ Interface +=====================*/

/**
*
* ���ƣ�OIOIC_Open
* ������Open�ӿڡ�
* ������pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Open(OBJECT* This, const VR* pCaller)
{
	This->RefCnt++; 
	return R_P;
}

/**
*
* ���ƣ�OIOIC_Input
* ������Input�ӿڡ�
* ������IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       pCaller --- [IN] �����ߡ� 
*/
static  RESULT  OIOIC_Input(OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{
	return RO_SBOOIX;
}

/**
*
* ���ƣ�OIOIC_Output
* ������Output�ӿڡ�
* ������OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Output(OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{	
   	return RO_SBOOIX;
}

/**
*
* ���ƣ�OIOIC_IOput
* ������IOput�ӿڡ�
* ������IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_IOput(OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{
	return RO_SBOOIX;
}

/**
*
* ���ƣ�OIOIC_Interact0
* ������Interact0�ӿڡ�
* ������Act --- [IN] ������Ϊ��
*       pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Interact0(OBJECT* This, ACTION Act, const VR* pCaller)
{
	EM_OIOIC*  pem = (EM_OIOIC*)This->pEM;

	switch( Act )
	{
		case ASK_SHARED: /* �ʹ���ô��*/
			if(pem->mds & MOD_SHARED) 
				return R_N;
			return R_P;
		case SET_SHARED: /* ���ù���*/
			pem->mds &= ~MOD_SHARED;
			return R_P;
		case SET_UNSHARED: /* ���÷ǹ���*/
			pem->mds |= MOD_SHARED;
			return R_P;
		case SET_BLOCKI: /* ����������롣*/
			pem->mds &= ~MOD_BLOCKI;
			return R_P;
		case SET_UNBLOCKI: /* ���÷�������롣*/
			pem->mds |= MOD_BLOCKI;
			return R_P;
		case SET_BLOCKO: /* ������������*/
			pem->mds &= ~MOD_BLOCKO;
			return R_P;
		case SET_UNBLOCKO: /* ���÷���������*/
			pem->mds |= MOD_BLOCKO;
			return R_P;
		case CMD_RESET: /* ���á�*/
			pem->IQ.Front = pem->IQ.Rear = pem->IQ.Dtrm - 1;
			pem->IQ.Qty = pem->IQ.Lost = 0;
			pem->OQ.Front = pem->OQ.Rear = pem->OQ.Dtrm - 1;
			pem->OQ.Qty = pem->OQ.Lost = 0;
			return R_P;
		default:
			break;					
	}

	return RO_SBOOIX;			
}

/**
*
* ���ƣ�OIOIC_Interact1
* ������Interact1�ӿڡ�
* ������Act --- [IN] ������Ϊ��
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Interact1(OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{
	EM_OIOIC*   pem = (EM_OIOIC*)(This->pEM);

	switch( Act )
	{
		case GET_IQCTY: /* ��ȡIQ������*/
			assert( Cty >= sizeof(BYTKTY) );
			*((BYTKTY*)OStrm) = pem->IQ.Cty;
			*pQty = sizeof(BYTKTY); 
			return R_P;
		case GET_OQCTY: /* ��ȡOQ������*/
			assert( Cty >= sizeof(BYTKTY) );
			*((BYTKTY*)OStrm) = pem->OQ.Cty;
			*pQty = sizeof(BYTKTY); 
			return R_P;
		default:
			break;						
	}
	
	return RO_SBOOIX;
}

/**
*
* ���ƣ�OIOIC_Interact2
* ������Interact2�ӿڡ�
* ������Act --- [IN] ������Ϊ��
*       IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Interact2(OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{
	EM_OIOIC*   pem = (EM_OIOIC*)(This->pEM);

	switch( Act )
	{
		case SET_IQCTY: /* ����IQ������*/
			assert( sizeof(BYTKTY) == Qty );
			if(pem->IQ.Dtrm != NULL)
				free(pem->IQ.Dtrm); 
			pem->IQ.Dtrm = (BYTE*)calloc(*((BYTKTY*)IStrm), 1);
			if( pem->IQ.Dtrm != NULL )
			{
				pem->IQ.Front = pem->IQ.Rear = pem->IQ.Dtrm - 1;
				pem->IQ.Cty = *((BYTKTY*)IStrm);
				pem->IQ.Qty = pem->IQ.Lost = 0;
				return R_P;
			}else 
			{ 
				return R_N;
			}
		case SET_OQCTY: /* ����OQ������*/
			assert( sizeof(BYTKTY) == Qty );
			if(pem->OQ.Dtrm != NULL)
				free(pem->OQ.Dtrm); 
			pem->OQ.Dtrm = (BYTE*)calloc(*((BYTKTY*)IStrm), 1);
			if( pem->OQ.Dtrm != NULL ) 
			{
				pem->OQ.Front = pem->OQ.Rear = pem->OQ.Dtrm - 1;
				pem->OQ.Cty = *((BYTKTY*)IStrm);
				pem->OQ.Qty = pem->OQ.Lost = 0;
				return R_P; 
			}else 
			{ 
				return R_N; 
			}
		default:
			break;						
	}
	
	return RO_SBOOIX;
}

/**
*
* ���ƣ�OIOIC_Interact3
* ������Interact3�ӿڡ�
* ������Act --- [IN] ������Ϊ��
*       IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Interact3(OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{	
	return RO_SBOOIX;
}

/**
*
* ���ƣ�OIOIC_Close
* ������Close�ӿڡ�
* ������pCaller --- [IN] �����ߡ�
*/
static  RESULT  OIOIC_Close(OBJECT* This, const VR* pCaller)
{
	This->RefCnt--; 
	return R_P;
}

/*========================+ TOG +========================*/

/**
*
* ���ƣ�TOG_OIOIC
*/
VOID  TOG_OIOIC(OBJECT* pObj)
{
	pObj->Open = OIOIC_Open;
	pObj->Input = OIOIC_Input;
	pObj->Output = OIOIC_Output;
	pObj->IOput = OIOIC_IOput;
	pObj->Interact0 = OIOIC_Interact0;
	pObj->Interact1 = OIOIC_Interact1;
	pObj->Interact2 = OIOIC_Interact2;
	pObj->Interact3 = OIOIC_Interact3;
	pObj->Close = OIOIC_Close;
}

/*====================+ Function(s) +====================*/
/* ... */
