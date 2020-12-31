/**
 *
 * �� �� ����o.c
 *
 * ��    ����o.c �ļ���
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
#include "o.h"
#include "oioic.h"


/*=============+ InitAIBofND & InitABNofND +=============*/

/* ע��InitAIBofND �� InitABNofNDֻ��CRT�������á�*/

/**
*
* ���ƣ�InitAIBofND
* ��������ʼ��ND��AIB��
* ������pND --- [IN] NDָ�룻
*       pMsn --- [IN] IB��MSN���飻
*       nib --- [IN] pMsn��ָ�����Ԫ�ظ�������IB��������
* ��ע��1. ֻ��CRT�������á�
*       2. ע�⣬pMsn��ָ����Ԫ�ص����������IB�ķ���˳��
*/
VOID  InitAIBofND(OBJECT* pND, MLDSN* pMsn, NUMIB nib)
{
	NUMND   i;
	NUMIB   j;
	
	pND->NIB = nib;
	
	while(nib)
	{
		j = pND->NIB-nib;
		for(i=1; i<pND->NND; i++)
		{
			if( *(pMsn+j) == (pND->po_AND+i)->MSN )
			{
				*(pND->ppo_AIB+j) = pND->po_AND+i;
				break;
			}
		}
		nib--;
	}
}	

/**
*
* ���ƣ�InitABNofND
* ��������ʼ��ND��ABN��
* ������pND --- [IN] NDָ�룻
*       pMsn --- [IN] BN��MSN���飻
*       nbn --- [IN] pMsn��ָ�����Ԫ�ظ�������BN��������
* ��ע��ֻ��CRT�������á�
*/
VOID  InitABNofND(OBJECT* pND, MLDSN* pMsn, NUMBN nbn)
{
	NUMND  i;
	NUMBN  j;
	
	pND->NBN = nbn;
	
	while(nbn)
	{
		j = pND->NBN-nbn;
		for(i=1; i<pND->NND; i++)
		{
			if( *(pMsn+j) == (pND->po_AND+i)->MSN )
			{
				*(pND->ppo_ABN+j) = pND->po_AND+i;
				break;
			}
		}
		nbn--;
	}
}

/*=============+ CallerCome & CallerLeave +==============*/

/* ע������������ʹ�� CallerCome �� CallerLeave��*/

/**
*
* ���ƣ�CallerCome
* ���������������ã��Ǽ����ü�¼��
* ������pND --- [IN] ND��ַ��
*       pCaller --- [IN] �����ߡ�
* ��ע������������ʹ�á�
*/
RESULT  CallerCome(OBJECT* pND, const VR* pCaller)
{    
	NUMCS   i;
	VR*     pVR; 
	
	/* �ȼ����û�����VR�� */
	for(i=0; i<pND->NCS; i++)
	{
		pVR = pND->pvr_ACS+i;
		if((pVR->cr == pCaller->cr) && (pVR->mr == pCaller->mr))
			return RN_HASVR; /* �Ѿ������VR�ˡ�*/
	}

	/* �������VR�� */
	for(i=0; i<pND->NCS; i++)
	{
		pVR = pND->pvr_ACS+i;
		if( 0 == pVR->cr )
		{
			*pVR = *pCaller;
			return R_P;
		}
	}
		
	return R_N;  
}

/**
*
* ���ƣ�CallerLeave
* �������������뿪��������ü�¼��
* ������pND --- [IN] ND��ַ��
*       pCaller --- [IN] �����ߡ�
* ��ע������������ʹ�á�
*/
RESULT  CallerLeave(OBJECT* pND, const VR* pCaller)
{
	NUMBN  i;
	NUMCS  j;
	VR*    pVR;
	
	/* ����ND��BN */
	for(i=0; i<pND->NBN; i++)
	{
		for(j=0; j<(*(pND->ppo_ABN+i))->NCS; j++)
		{
			pVR = (*(pND->ppo_ABN+i))->pvr_ACS+j;
		  	if((pVR->cr == pCaller->cr) && (pVR->mr == pCaller->mr))
				pVR->cr = pVR->mr = 0;
		}
	}
	
	/* Ȼ��ND�Լ� */
	for(j=0; j<pND->NCS; j++)
	{
		pVR = pND->pvr_ACS+j;
		if((pVR->cr == pCaller->cr) && (pVR->mr == pCaller->mr))
			pVR->cr = pVR->mr = 0;
	} 

	return R_P;
}

/*==================+ Enqueue & Dequeue +================*/

/**
*
* ���ƣ�Enqueue
* ��������ָ���Ķ�������ָ��������Byte��
* ������IData --- [IN] �������ݣ�
*       Qty --- [IN] ���е�Byte������
*/
RESULT  Enqueue(QUEUE* pq, BYTE* IData, BYTKTY Qty)
{
    BYTE*     newRear;  /* ���гɹ���pq->RearӦ��������λ�á�*/
	BYTKTY    ctyFree, qtyEnq;
	
	assert( pq != NULL );
	assert( IData != NULL );
	
	ctyFree = pq->Cty - pq->Qty;
	qtyEnq = (ctyFree > Qty) ? Qty : ctyFree;

	
	if( 0 == qtyEnq )
	{
		if( Qty > 0 )
		{
			pq->Lost = Qty;  /* ��ʧ�� Qty ��Byte ��*/
			return RN_LOST;
		}

		return RO_VAINLY;
	}

		
	newRear = pq->Dtrm + (pq->Rear + qtyEnq - pq->Dtrm ) % pq->Cty; 
	if(newRear > pq->Rear) /* ������гɹ���pq->Rear����ѭ����*/
	{
		memcpy(pq->Rear+1, IData, qtyEnq);
	}else /* ������гɹ���pq->Rear��ѭ����*/
	{	
		BYTKTY   qty1, qty2;
		qty1 = pq->Dtrm + pq->Cty - pq->Rear - 1;
		if(qty1 > 0)
			memcpy(pq->Rear+1, IData, qty1);
		qty2 = qtyEnq - qty1;
		memcpy(pq->Dtrm, IData + qty1, qty2);
	}

	pq->Rear = newRear;
	pq->Qty += qtyEnq;

	if(pq->Dtrm-1 == pq->Front)
		pq->Front = pq->Dtrm;

	if(ctyFree < Qty)
	{
	   pq->Lost = Qty - ctyFree;  /* ��ʧ�� Qty - ctyFree ��Byte ��*/
	   return RN_LOST;
	}
	
	return R_P;
}

/**
*
* ���ƣ�Dequeue
* ��������ָ�����г���Byte��
* ������OData --- [OUT] �������ݵĴ洢λ�ã�
*       Cty --- [IN] OData��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�ʳ��е�Byte������
*/
RESULT  Dequeue(QUEUE* pq, BYTE* OData, BYTKTY Cty, BYTKTY* pQty)
{
	BYTE*    newFront;  /* ���гɹ���pq->FrontӦ��������λ�á�*/
    BYTKTY   qtyDeq;  /* ʵ�ʳ�����������*/

	assert( pq != NULL );
	assert( OData != NULL );
	assert( Cty != 0 );
	assert( pQty != NULL );
	
	*pQty = 0; /* �Ƚ���������0��*/
	
	if( pq->Qty != 0 )
	{
		qtyDeq = (Cty > pq->Qty) ? pq->Qty : Cty;

		newFront = pq->Dtrm + (pq->Front + qtyDeq - pq->Dtrm) % pq->Cty;
		if(newFront > pq->Front) /* ���к�pq->Front����ѭ��? */
		{
			memcpy(OData, pq->Front, qtyDeq);
		}else /* ���к�pq->Front��ѭ����*/
		{
			BYTKTY   qty1, qty2;
			qty1 = pq->Dtrm + pq->Cty - pq->Front;
			qty2 = qtyDeq - qty1;
			memcpy(OData, pq->Front, qty1);
			memcpy(OData + qty1, pq->Dtrm, qty2);
		}

		pq->Qty -= qtyDeq;

		if(0 == pq->Qty)
			pq->Front = pq->Rear = pq->Dtrm -1;
		else
			pq->Front = newFront;

	}else
	{
	   return RO_VAINLY;
	}

	*pQty = qtyDeq;
	
	if(pq->Lost > 0)
	{
	   pq->Lost = 0;
	   return  RN_LOST;
	}	

	return R_P;
} 

/*=================+ GetBN & GetEMofBN +=================*/

/**
*
* ���ƣ�GetBN
* ��������ȡָ��BN�ĵ�ַ��
* ������pND --- [IN] NDָ�룻
*       msn --- [IN] BN��M��
* ���أ�BN��ָ�롣
*/
OBJECT*  GetBN(OBJECT* pND, MLDSN msn)
{
	NUMBN   i;
	
	assert( pND != NULL );

	for(i=0; i<pND->NBN; i++)
	{
		if( msn == (*(pND->ppo_ABN+i))->MSN )
			return  *(pND->ppo_ABN+i);
	}
	
	return NULL;  /* û�����BN��*/
}

/**
*
* ���ƣ�GetEMofBN
* ��������ȡָ��BN��EM��ַ��
* ������pND --- [IN] NDָ�룻
*       msn --- [IN] BN��M��
* ���أ�EM��ָ�롣
*/
EM*  GetEMofBN(OBJECT* pND, MLDSN msn)
{
	NUMBN   i;
	
	assert( pND != NULL );

	for(i=0; i<pND->NBN; i++)
	{
		if( msn == (*(pND->ppo_ABN+i))->MSN )
			return  (*(pND->ppo_ABN+i))->pEM;
	}
	
	return NULL; 
}	

/*===================+ GetIQ & GetOQ +===================*/

/**
*
* ���ƣ�GetIQ
* ��������ȡ�ڵ��IQָ�롣
* ������pND --- [IN] �ڵ�ָ�롣
* ���أ�IQ��ָ�롣
*/
QUEUE*  GetIQ(OBJECT* pND)
{
	NUMBN   i;
	
	assert( pND != NULL );

	for(i=0; i<pND->NBN; i++)
	{
		if( MSN_OIOIC == (*(pND->ppo_ABN+i))->MSN )
			return  &((EM_OIOIC*)((*(pND->ppo_ABN+i))->pEM))->IQ;
	}
	
	return NULL; 
}

/**
*
* ���ƣ�GetOQ
* ��������ȡ�ڵ��OQָ�롣
* ������pND --- [IN] �ڵ�ָ�롣
* ���أ�OQ��ָ�롣
*/
QUEUE*  GetOQ(OBJECT* pND)
{
	NUMBN   i;
	
	assert( pND != NULL );

	for(i=0; i<pND->NBN; i++)
	{
		if( MSN_OIOIC == (*(pND->ppo_ABN+i))->MSN )
			return  &((EM_OIOIC*)((*(pND->ppo_ABN+i))->pEM))->OQ;
	}
	
	return NULL; 
}


/*==================+  ������VO����  +===================*/

/* ע�⣺����VO�����ͷ�����VO�����Ĺ�������ȫ��ͬ�ġ�*/


#if INLINE_VOFUNCTION == 0

/**
*
* ���ƣ�VO_Open
* �������ӿ�Open��VO������
* ������pObject --- [IN] ָ�����
*       pCaller --- [IN] �����ߡ�
*/    	             
RESULT  VO_Open(OBJECT* pObject, const VR* pCaller)
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );
	
	if( R_P == CallerCome(pObject, pCaller) )
	{
		ir = pObject->Open(pObject, pCaller);
		CallerLeave(pObject, pCaller);
	}else { ir = R_N; }
	
	return ir;
}
       
/**
*
* ���ƣ�VO_Input
* �������ӿ�Input��VO������
* ������pObject --- [IN] ָ����� 
*       IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       pCaller --- [IN] �����ߡ�
*/          
RESULT  VO_Input(OBJECT* pObject, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( IStrm != NULL );
	assert( Qty > 0 );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->Input(pObject, IStrm, Qty, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_Output
* �������ӿ�Output��VO������
* ������pObject --- [IN] ָ����� 
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/      
RESULT  VO_Output(OBJECT* pObject, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller) 
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( OStrm != NULL );
	assert( Cty > 0 );
	assert( pQty != NULL );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );

	*pQty = 0;

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->Output(pObject, OStrm, Cty, pQty, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_IOput
* �������ӿ�IOput��VO������
* ������pObject --- [IN] ָ����� 
*       IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/  
RESULT  VO_IOput(OBJECT* pObject, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);
	
	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( IStrm != NULL );
	assert( Qty > 0 );
	assert( OStrm != NULL );
	assert( Cty > 0 );
	assert( pQty != NULL );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );
	
	*pQty = 0;

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->IOput(pObject, IStrm, Qty, OStrm, Cty, pQty, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_Interact0
* �������ӿ�Interact0��VO������
* ������pObject --- [IN] ָ�����
*       Act --- [IN] ������Ϊ��
*       pCaller --- [IN] �����ߡ�
*/	       
RESULT  VO_Interact0(OBJECT* pObject, ACTION Act, const VR* pCaller) 
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( (MSKAC & Act) == AC0 );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );
	
	if( R_P == CallerCome(pObject, pCaller) )
	{
		ir = pObject->Interact0(pObject, Act, pCaller);
		CallerLeave(pObject, pCaller);
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_Interact1
* �������ӿ�Interact1��VO������
* ������pObject --- [IN] ָ�����
*       Act --- [IN] ������Ϊ��
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/       
RESULT  VO_Interact1(OBJECT* pObject, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller) 
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( (MSKAC & Act) == AC1 );
	assert( OStrm != NULL );
	assert( Cty > 0 );
	assert( pQty != NULL );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );

	*pQty = 0;

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->Interact1(pObject, Act, OStrm, Cty, pQty, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_Interact2
* �������ӿ�Interact2��VO������
* ������pObject --- [IN] ָ�����
*       Act --- [IN] ������Ϊ��
*       IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       pCaller --- [IN] �����ߡ�
*/       
RESULT  VO_Interact2(OBJECT* pObject, ACTION Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)  
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;
	
	assert( pObject != NULL );
	assert( (MSKAC & Act) == AC2 );
	assert( IStrm != NULL );
	assert( Qty > 0 );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->Interact2(pObject, Act, IStrm, Qty, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_Interact3
* �������ӿ�Interact3��VO������
* ������pObject --- [IN] ָ�����
*       Act --- [IN] ������Ϊ��
*       IStrm --- [IN] ��������
*       Qty --- [IN] �����Byte������
*       OStrm --- [OUT] �����������
*       Cty --- [IN] OStrm��ָ�洢�ռ��������
*       pQty --- [OUT] ָ��ʵ�������Byte������
*       pCaller --- [IN] �����ߡ�
*/       
RESULT  VO_Interact3(OBJECT* pObject, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)  
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;

	assert( pObject != NULL );
	assert( (MSKAC & Act) == AC3 );
	assert( IStrm != NULL );
	assert( Qty > 0 );
	assert( OStrm != NULL );
	assert( Cty > 0 );
	assert( pQty != NULL );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );

	*pQty = 0;

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->Interact3(pObject, Act, IStrm, Qty, OStrm, Cty, pQty, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

/**
*
* ���ƣ�VO_Close
* �������ӿ�Close��VO������
* ������pObject --- [IN] ָ����� 
*       pCaller --- [IN] �����ߡ�
*/       
RESULT  VO_Close(OBJECT* pObject, const VR* pCaller) 
{
	extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller);
	extern  RESULT  CallerLeave(OBJECT* pND, const VR* pCaller);

	RESULT  ir = R_O;
	
	assert( pObject != NULL );
	assert( pCaller->cr != 0 );
	assert( pCaller->mr != 0 );

	if( R_P == CallerCome(pObject, pCaller) ) 
	{ 
		ir = pObject->Close(pObject, pCaller); 
		CallerLeave(pObject, pCaller); 
	}else { ir = R_N; }
	
	return ir;
}

#endif  /* #if INLINE_VOFUNCTION == 0 */
