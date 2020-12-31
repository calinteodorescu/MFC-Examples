/**
 *
 * �� �� ����o.h
 *
 * ��    ����o.h �ļ���
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
 *   <����>             <�޸���>
 *   <�޸�����...>
 *
 **/

#ifndef __O_H__
#define __O_H__
      
/* #define  NDEBUG */
#include <assert.h>


/*
*
* ���ƣ�INLINE_VOFUNCTION
* ����������VO�����Ŀ��ء�0 - ��������1 - ���������� - �������á�
* ��ע������Ϊ1ʱ���ܷ���������VO������ȡ��������ġ�_INLINE_���궨���Ƿ���Ч��
*/
#define     INLINE_VOFUNCTION          0

/*
*
* ���ƣ�_INLINE_
* �������궨������ָʾ������Ϊ������������ָʾ����ͳһ��
* ��ע��һ��Ҫ����Ϊ��Ч������ָʾ��������ʹ����ͨ��������Ҳ����Ч�ģ�����
*       �����Զ����������εĺ���תΪ���������롣
*/
#define     _INLINE_		      __inline
     

/*
*
* ������OS��HO��SO��mask��
* ��ע��ĿǰOID����64������λ��ʾ�ģ�������OBJID�������Ͷ��壩����
*       �У���16λ��OS��ʶ����32λ��HO��ʶ�򣬵�16λ��SO��ʶ��
*/
#define     MSKOS		          0xFFFF000000000000L
#define     MSKHO		          0x0000FFFFFFFF0000L
#define     MSKSO		          0x000000000000FFFFL


/*
*
* ��OS��OID��
*/
#define		OID_OICOS             0x0001000000000000L     /* Oicos��OID��*/
#define		OID_UNIX              0x0002000000000000L     /* Unix��OID��*/
#define		OID_LINUX             0x0003000000000000L     /* Linux��OID��*/
#define		OID_WINDOWS           0x0004000000000000L     /* Windows��OID��*/


/*
*
* ���ƣ�OID_OS
* ������OS��OID��
*/
#define		OID_OS                OID_WINDOWS


/*
*
* һ���������Ͷ���
*/
typedef     char                        SI8;  
typedef     unsigned char               UI8;  
typedef     short                       SI16;  
typedef     unsigned short              UI16; 
typedef     int                         SI32; 
typedef     unsigned int                UI32; 

#if OID_OS == OID_OICOS
#elif OID_OS == OID_UNIX
#elif OID_OS == OID_LINUX
typedef     long long                   SI64;
typedef     unsigned long long          UI64;
#elif OID_OS == OID_WINDOWS
typedef     __int64                     SI64;
typedef     unsigned __int64            UI64;
#else
#error the OID of OS is undefined.
#endif

typedef     float			            SR32;
/* typedef     unsigned float              UR32;  */
typedef     double                      SR64;
/* typedef     unsigned double             UR64; */
typedef     void                        VOID; 


/*
*
* �����������Ͷ���
*/
typedef     UI8         BYTE;              /* λԪ�����͡�*/
typedef     UI32        ACTION;            /* ��Ϊ���͡�*/
typedef     SI32        RESULT;            /* ������ͣ��������з������͡�*/
typedef     UI32        BYTKTY;            /* by BYTe��c(K)apaciTY and q(K)uantiTY's type����ByteΪ��λ���������������͡�*/
typedef     UI32        BITKTY;            /* by BIT��c(K)apaciTY and q(K)uantiTY's type����BitΪ��λ���������������͡�*/

typedef     UI16        MLDSN;             /* MSN���͡�*/
typedef     UI64        OBJID;             /* OID���͡�*/
typedef     UI32        REFCNT;            /* object's REFerence CouNT type����������ü������͡�*/
typedef     UI32        MODES;             /* ģʽ�����͡�*/

typedef     UI16        NUMCS;             /* the NUMber of CS type��CS���������͡�*/
typedef     UI16        NUMND;             /* the NUMber of ND type��ND���������͡�*/
typedef     UI16        NUMIB;             /* the NUMber of IB type��IB���������͡�*/
typedef     UI16        NUMBN;             /* the NUMber of BN type��BN���������͡�*/

typedef     SI8         BOOL;              /* �������ͣ��涨����������ֻ��������ֵ��TRUE �� FALSE �������� TRUE �� FALSE �Ķ��壩��*/


/*
*
* Byte��С������λ����
*/
#define     BYTESIZE        8


/*
*
* ������MSN
* ��ע��Ҫ��MSN��1��ʼ˳���ţ�����ģ�����£�
*       #define   MSN_XXX1    (MSN_ + 0)  /-* xxx1��MSN��HO *-/
*       #define   MSN_XXX2    (MSN_ + 1)  /-* xxx2��MSN��AO *-/
*       #define   MSN_XXX3    (MSN_ + 2)  /-* xxx3��MSN��.. *-/
*       #define   MSN_XXX4    (MSN_ + 3)  /-* xxx4��MSN��GO *-/
*       #define   MSN_XXX5    (MSN_ + 4)  /-* xxx5��MSN��.. *-/
*       ...     ...     ...     ...     ...     ...   
*       #define   MSN_XXXn    (MSN_ + n-1)  /-* xxxn��MSN��... *-/
*/
#define    MSN_OIOIC         1				   /* OIOIC��MSN��*/
#define    MSN_              (MSN_OIOIC + 1)   /* ����MSN����㡣*/

/*
*
* ������ͨ��ģʽ���塣
* ��ע���ɶ��� sizeof(MODES)*BYTESIZE ��ͨ��ģʽ�������£�
*
*       #define   MOD_                     1
*       #define   MOD_XXX0             ( MOD_ << 0 )     /-* BIT0��1��xxx��0��xxx��*-/
*       #define   MOD_XXX1             ( MOD_ << 1 )     /-* BIT1��1��xxx��0��xxx��*-/
*       #define   MOD_XXX2             ( MOD_ << 2 )     /-* BIT2��1��xxx��0��xxx��*-/
*       ...     ...     ...     ...     ...     ...    
*       #define   MOD_XXXn            ( MOD_ << (sizeof(MODES)*BYTESIZE-1) )   /-* BITn�� 1��xxx�� 0��xxx��*-/
*/
#define   MOD_                  1
#define   MOD_SHARED            ( MOD_ << 0 )     /* BIT0��0������1���ǹ���*/
#define   MOD_BLOCKI            ( MOD_ << 1 )     /* BIT1��0��������룻1����������롣ֻ������Input��IOput�ӿڡ�*/
#define   MOD_BLOCKO            ( MOD_ << 2 )     /* BIT2��0����������1������������ֻ������Output��IOput�ӿڡ�*/


/*
*
* ���������󽻻���Ϊ���ඨ�塣
* ��ע��1. ���Ƿ���Ҫ����Byte������û�����Byte��,�ɽ�������Ϊ��Ϊ���֣�
*          AC0 - ��������Byte��Ҳû�����Byte����
*          AC1 - ����Ҫ����Byte���������Byte����
*          AC2 - ��Ҫ����Byte����û�����Byte����
*          AC3 - ����Ҫ����Byte��Ҳ�����Byte����
*
*       2. TSI��TSO
*          TSI - the Type of Stream Inputted�����������͡�
*          TSO - the Type of Stream Outputted����������͡�
*/
#define     AC0        0  
#define     AC1        (1 << (sizeof(ACTION)*BYTESIZE - 2))   
#define     AC2        (2 << (sizeof(ACTION)*BYTESIZE - 2))   
#define     AC3        (3 << (sizeof(ACTION)*BYTESIZE - 2))  

/* �жϽ�����Ϊ�����mask */
#define     MSKAC      (3 << (sizeof(ACTION)*BYTESIZE - 2)) 


/*
*
* ��������Ϊ���塣   
* ��ע��=== ����ģ�� ===
*    
*       /-** AC0 ��Ϊ��**-/
*       #define     XXX_YYYYY            (AC0_ + 0)  
*       #define     XXX_YYYYY            (AC0_ + 1)  
*       ...     ...     ...     ...     ...     ...   
*
*       /-** AC1 ��Ϊ��**-/
*       #define     XXX_YYYYY            (AC1_ + 0)  
*       #define     XXX_YYYYY            (AC1_ + 1)  
*       ...     ...     ...     ...     ...     ...   
*
*       /-** AC2 ��Ϊ��**-/
*       #define     XXX_YYYYY            (AC2_ + 0)  
*       #define     XXX_YYYYY            (AC2_ + 1)  
*       ...     ...     ...     ...     ...     ...   
*
*       /-** AC3 ��Ϊ��**-/
*       #define     XXX_YYYYY            (AC3_ + 0)  
*       #define     XXX_YYYYY            (AC3_ + 1)  
*       ...     ...     ...     ...     ...     ...   
*                      
*/

/** AC0 ��Ϊ��**/
#define     ASK_SHARED                  (AC0 + 0)                /* �ʶ�����ô��*/
#define     SET_SHARED                  (AC0 + 1)                /* ���ö�����*/
#define     SET_UNSHARED                (AC0 + 2)                /* ���ö���ǹ���*/
#define     SET_BLOCKI                  (AC0 + 3)                /* ���ö���������롣*/
#define     SET_UNBLOCKI                (AC0 + 4)                /* ���ö����������롣*/
#define     SET_BLOCKO                  (AC0 + 5)                /* ���ö�����������*/
#define     SET_UNBLOCKO                (AC0 + 6)                /* ���ö������������*/
#define     CMD_RESET                   (AC0 + 7)                /* ���á�*/
#define     CMD_PREPARE                 (AC0 + 8)                /* ׼����*/
#define     AC0_                        (AC0 + 9)                /* <����AC0��Ϊ�����> */ 

/** AC1 ��Ϊ��**/
#define     GET_IQCTY                   (AC1 + 0)                /* ��ȡIQ������*/
#define     GET_OQCTY                   (AC1 + 1)                /* ��ȡOQ������*/
#define     AC1_                        (AC1 + 2)                /* <����AC1��Ϊ�����> */ 

/** AC2 ��Ϊ��**/
#define     SET_IQCTY                   (AC2 + 0)                /* ����IQ������*/
#define     SET_OQCTY                   (AC2 + 1)                /* ����OQ������*/
#define     CMD_INITIALIZE              (AC2 + 2)                /* ��ʼ�� */
#define     AC2_                        (AC2 + 3)                /* <����AC2��Ϊ�����> */ 

/** AC3 ��Ϊ��**/
/* ...     ...     ...     ...     ...     ... */
#define     AC3_                        (AC3 + 0)                /* <����AC3��Ϊ�����> */



/*
*
* ������������塣 
* ��ע��=== ����ģ�� ===
*
*       ...     ...     ...     ...     ...     ...  
*       #define       RN_XXXXX            (RN_ - 1) 
*       #define       RN_XXXXX            (RN_ - 0) 
*       /-** N ������ϣ���**-/
* 
*       ...     ...     ...     ...     ...     ...  
*       #define       RO_XXXXX           (RON_ - 1) 
*       #define       RO_XXXXX           (RON_ - 0) 
*       /-**
*       ** �� O ������ϣ���
*       **
*       ** �� O ������£���
*       **-/
*       #define       RO_XXXXX           (ROP_ + 0)  
*       #define       RO_XXXXX           (ROP_ + 1)  
*       ...     ...     ...     ...     ...     ...     
*
*       /-** P ������£���**-/ 
*       #define       RP_XXXXX            (RP_ + 0)  
*       #define       RP_XXXXX            (RP_ + 1)  
*       ...     ...     ...     ...     ...     ...     
*
*/

#define        RN_                         (R_N - 7)                 /* <����N��������> */
#define        RN_LACKOID                  (R_N - 6)                 /* __CreateObject__��û���㹻��OID�����䡣*/
#define        RN_HASVR                    (R_N - 5)                 /* CallerCome���Ѿ������VR�ˡ�*/
#define        RN_LOST                     (R_N - 4)                 /* Enqueue/Dequeue���Ѷ�ʧ��Byte��*/
#define        RN_UNSHARED                 (R_N - 3)                 /* Open�������Ƿǹ���ġ�*/
#define        RN_FULL                     (R_N - 2)                 /* Open���������������������ٴ򿪡�*/
#define        RN_INVALIDARG               (R_N - 1)                 /* ��Ч���� */
#define        R_N                          __RON__		             /* N��������Խ��������㡣*/

#define        __RON__                     (RON_ - 1000)             /* O������N�硣*/

#define        RON_		                   (R_O - 3)                 /* <������O��������> */
#define        RO_NOPEN		               (R_O - 2)                 /* Close������û�б��򿪡�*/
#define        RO_VAINLY                   (R_O - 1)                 /* �ղ��� */
#define        R_O			                0			             /* O����������Խ��������е㡣*/
#define        RO_OBSOIX                   (R_O + 1)                 /* OBS˳��ǽ�������ģʽר��ִ�н����*/
#define        RO_SBOOIX                   (R_O + 2)                 /* SBO˳��ǽ�������ģʽר��ִ�н����*/
#define        RO__SBOOIX                  (R_O + 3)                 /* SBO˳���������ģʽר��ִ�н����*/
#define        ROP_		                   (R_O + 4)                 /* <������O��������> */

#define        __ROP__                     (ROP_ + 1000)             /* O������P�硣*/

#define        R_P                          __ROP__		             /* P������϶��Խ��������㡣*/
#define        RP_RCZERO                   (R_P + 1)                 /* Close���������ü�����Ϊ0��*/
#define        RP_RCONE                    (R_P + 2)                 /* Open���������ü���Ϊ1��*/    
#define        RP_                         (R_P + 3)                 /* <����P��������> */

/*
*
* ����������ķ���/������/�϶��Լ�ⶨ�塣
* ��ע��RN --- Negative Result��
*       RO --- O  Result��
*       RP --- Positive Result��
*/
#define     RN(Result)      ( (RESULT)(Result) <= __RON__ )  
#define     RO(Result)      ( ( (RESULT)(Result) > __RON__ ) && ( (RESULT)(Result) < __ROP__ ) )
#define     RP(Result)      ( (RESULT)(Result) >= __ROP__ )  



/*
*
* ���岼��ֵ�Ϳ�ָ��ֵ��
*/
#undef TRUE
#undef FALSE
#undef NULL

#define TRUE          1      /* ������������ֵ���� */
#define FALSE         0      /* ������������ֵ���� */
#define NULL          0      /* ָ����������ֵ���� */


/*======================+  OIOIC  +======================*/

/*
*
* ���������нṹ��
* ��ע��q_��pq_��
*/
typedef struct TAG_QUEUE
{
   BYTE*    Dtrm;        /* Data Room�����ݿռ䡣*/
   BYTKTY   Cty;         /* Dtrm��������*/
   BYTE*    Front;       /* ��ǰ��һ��Byte���ݵĵ�ַ��*/
   BYTE*    Rear;        /* ��ǰ���һ��Byte���ݵĵ�ַ��*/
   BYTKTY   Qty;         /* ��ǰDtrm�ڴ��е�Byte������*/
   BYTKTY   Lost;        /* ��Dtrm������������Byte������*/
}QUEUE;


/*
*
* ������VR�ṹ�塣
* ��ע��vr_��pvr_��
*/
typedef struct TAG_VR
{
	OBJID    cr;  /* CR��ID��*/
	OBJID    mr;  /* MR��ID��*/
}VR;   

/*
*
* ���������� EM �������͡�
* ��ע��1. em_��pem_��
*       2. �����ʵ����չ��Ա����������¸�ʽ��EM�ṹ���
*			typedef struct TAG_EM_XXX
*			{
*				...
*			}EM_XXX;
*/
typedef    VOID     EM;  /* ����EM�������͡�*/


/*
*
* ������OIOIC����ģ�ͽṹ�塣
* ��ע����OIOIC����5��ͨ�ýӿڣ�Open��Input��Output��Interact��Close���ĵ�һ���ַ��ĺ�д��
*/
typedef struct TAG_OBJECT
{   
   /* RefCnt���������ü�����Reference Count����*/
   REFCNT     RefCnt;

   /* MSN��ģ��SN��*/  
   MLDSN      MSN;
   
   /* OID�������ID��*/  
   OBJID      OID; 
   
   /* Ϊ��... ��Ҫ����������Ա�� 
   * po_AND��ָ��AND��
   * NND��the Number of ND of IC��IC��ND������Ҳ����AND��Ԫ��������*/
   struct TAG_OBJECT*   po_AND; 
   NUMND      NND; 
   
   /* Ϊ�ܸ�Ч�ʷ���IBN����Ҫ����������Ա��
   * ppo_AIB��ָ��AIB���顣
   * NIB��the Number of IB��IB��������AIB��Ԫ��������*/
   struct TAG_OBJECT**  ppo_AIB;
   NUMIB      NIB; 
   
   /* Ϊ���ܷ���BN��EM�͸�Ч�ʵ�������ü�¼����Ҫ����������Ա��
   * ppo_ABN��ָ��ABN���顣
   * NBN��the Number of BN��BN��������ABN��Ԫ��������*/
   struct TAG_OBJECT**  ppo_ABN;
   NUMBN	  NBN; 
   
   /* �������루Not Reentrant�����ʹ��򣬼�ͬһMR��ͬһCR��ÿ��BNֻ�ܷ���һ�Ρ�
   * Ϊ���ز���������ʹ�����Ҫ����������Ա��
   * pvr_ACS��ָ��ACS��
   * NCS��the Number of CS��CS����������ACS��Ԫ��������*/
   VR*        pvr_ACS;
   NUMCS      NCS;    
   
   /** ����ӿڣ�Open��Input��Output��IOput��Interact0~3��Close��**/
   
   /* ���ƣ�Open
   *  �������򿪡�
   *  ������pCaller --- [IN] �����ߡ�*/
   RESULT  (*Open) (struct TAG_OBJECT* This, const VR* pCaller);
   /* ���ƣ�Input
   *  ���������롣
   *  ������IStrm --- [IN] ��������
   *        Qty --- [IN] �����Byte������
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*Input) (struct TAG_OBJECT* This, BYTE* IStrm, BYTKTY Qty, const VR* pCaller);
   /* ���ƣ�Output
   *  �����������
   *  ������OStrm --- [OUT] �����������
   *        Cty --- [IN] OStrm��ָ�洢�ռ��������
   *        pQty --- [OUT] ָ��ʵ�������Byte������
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*Output) (struct TAG_OBJECT* This, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
   /* ���ƣ�IOput
   *  ���������������
   *  ������IStrm --- [IN] ��������
   *        Qty --- [IN] �����Byte������
   *        OStrm --- [OUT] �����������
   *        Cty --- [IN] OStrm��ָ�洢�ռ��������
   *        pQty --- [OUT] ָ��ʵ�������Byte������
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*IOput) (struct TAG_OBJECT* This, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
   /* ���ƣ�Interact0
   *  ����������������AC0���͵���Ϊ��
   *  ������Act --- [IN] ������Ϊ��
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*Interact0) (struct TAG_OBJECT* This, ACTION Act, const VR* pCaller);
   /* ���ƣ�Interact1
   *  ����������������AC1���͵���Ϊ��
   *  ������Act --- [IN] ������Ϊ��
   *        OStrm --- [OUT] �����������
   *        Cty --- [IN] OStrm��ָ�洢�ռ��������
   *        pQty --- [OUT] ָ��ʵ�������Byte������
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*Interact1) (struct TAG_OBJECT* This, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
   /* ���ƣ�Interact2
   *  ����������������AC2���͵���Ϊ��
   *  ������Act --- [IN] ������Ϊ��
   *        IStrm --- [IN] ��������
   *        Qty --- [IN] �����Byte������
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*Interact2) (struct TAG_OBJECT* This, ACTION  Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller);
   /* ���ƣ�Interact3
   *  ����������������AC3���͵���Ϊ��
   *  ������Act --- [IN] ������Ϊ��
   *        IStrm --- [IN] ��������
   *        Qty --- [IN] �����Byte������
   *        OStrm --- [OUT] �����������
   *        Cty --- [IN] OStrm��ָ�洢�ռ��������
   *        pQty --- [OUT] ָ��ʵ�������Byte������
   *        pCaller --- [IN] �����ߡ�*/
   RESULT  (*Interact3) (struct TAG_OBJECT* This, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
   /* ���ƣ�Close
   *  �������رա�
   *  ������pCaller --- [IN] �����ߡ�*/
   RESULT  (*Close) (struct TAG_OBJECT* This, const VR* pCaller);
   
   /* pEM��ָ������EM����չ��Ա����*/
   EM*    pEM;   
}OBJECT;


/*=============+  ���װ����Byte���кͳ���  +============*/

/* ����Byte����������ģʽ��
*  1. PLACE --- ����ģʽ�����п��пռ�Ķ��з���һ��Byte��
*  2. PUSH --- ����ģʽ����������û�п��пռ䣩�Ķ�������һ��Byte���ѵ�һ��Byte�����ӵ���*/

/*
*
* ���ƣ�ENQ_PLACE_BYTE
* �������꣬���п��пռ�Ķ��з���һ��Byte��
* ������pQ --- [IN] QUEUE����ָ�룬ָ����У�������б������п��пռ�ģ� 
*       byte --- [IN] ���е�Byte��
* ��ע��ֻ���ڶ����п��пռ�ʱ����ʹ�á����£�
*
*		 if(pQ->Qty < pQ->Cty)
*		 { 
*		  	ENQ_PLACE_BYTE(pQ, byte);
*		 }else
*		 {
*		 	pQ->Lost++; 
* 
*	 		/-* ... *-/
*		 }
*/
#define  ENQ_PLACE_BYTE(pQ, byte) \
	do{ \
		if((pQ)->Rear == (pQ)->Dtrm + (pQ)->Cty - 1) \
			(pQ)->Rear = (pQ)->Dtrm; \
		else \
			(pQ)->Rear++; \
		*((pQ)->Rear) = (byte); \
		(pQ)->Qty++; \
		if((pQ)->Dtrm-1 == (pQ)->Front) \
			(pQ)->Front = (pQ)->Dtrm; \
	}while(0)

/*
*
* ���ƣ�ENQ_PUSH_BYTE
* �������꣬��������û�п��пռ䣩�Ķ�������һ��Byte���ѵ�һ��Byte�����ӵ���
* ������pQ --- [IN] QUEUE����ָ�룬ָ����У�
*       byte --- [IN] ���е�Byte��
* ��ע��ֻ���ڶ�������ʱ����ʹ�á����£�
*
*	  	 if(pQ->Qty < pQ->Cty)
*		 {
*			ENQ_PLACE_BYTE(pQ, byte);
*		 }else
*		 {
*		 	ENQ_PUSH_BYTE(pQ, byte);
*		 	pQ->Lost++; 
* 
*			/-* ... *-/
*		 }
*/
#define  ENQ_PUSH_BYTE(pQ, byte) \
	do{ \
		(pQ)->Rear = (pQ)->Front; \
		*((pQ)->Rear) = (byte); \
		if( (pQ)->Front == ((pQ)->Dtrm + (pQ)->Cty -1) ) \
			(pQ)->Front = (pQ)->Dtrm; \
		else \
			(pQ)->Front++; \
	}while(0)

/*
*
* ���ƣ�DEQ_BYTE
* �������꣬�ӷǿյĶ��г���һ��Byte��
* ������pQ --- [IN] QUEUE����ָ�룬ָ����У�
*       pByte --- [OUT] Byte��ַ��������е�Byte��
* ��ע��ֻ���ڶ��зǿ�ʱ����ʹ�á����£�
*
*	  	 if(pQ->Qty > 0)
*		 {
*			DEQ_BYTE(pQ, pByte);
*		 }
*/
#define  DEQ_BYTE(pQ, pByte) \
	do{ \
		*(pByte) = *((pQ)->Front); \
		(pQ)->Qty--; \
		if(0 == (pQ)->Qty) \
			(pQ)->Front = (pQ)->Rear = (pQ)->Dtrm -1; \
		else if( (pQ)->Front == ((pQ)->Dtrm + (pQ)->Cty -1) ) \
			(pQ)->Front = (pQ)->Dtrm; \
		else \
			(pQ)->Front++; \
	}while(0)


/*===================+  ����VO����  +====================*/

/* ע�⣺����VO�����ͷ�����VO�����Ĺ�������ȫ��ͬ�ġ�*/


#if INLINE_VOFUNCTION == 1

/**
*
* ���ƣ�VO_Open
* �������ӿ�Open��VO������
* ������pObject --- [IN] ָ�����
*       pCaller --- [IN] �����ߡ�
*/    	             
static  _INLINE_  RESULT  VO_Open(OBJECT* pObject, const VR* pCaller)
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
static  _INLINE_  RESULT  VO_Input(OBJECT* pObject, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)
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
static  _INLINE_  RESULT  VO_Output(OBJECT* pObject, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller) 
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
static  _INLINE_  RESULT  VO_IOput(OBJECT* pObject, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)
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
static  _INLINE_  RESULT  VO_Interact0(OBJECT* pObject, ACTION Act, const VR* pCaller) 
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
static  _INLINE_  RESULT  VO_Interact1(OBJECT* pObject, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller) 
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
static  _INLINE_  RESULT  VO_Interact2(OBJECT* pObject, ACTION Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller)  
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
static  _INLINE_  RESULT  VO_Interact3(OBJECT* pObject, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller)  
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
static  _INLINE_  RESULT  VO_Close(OBJECT* pObject, const VR* pCaller) 
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

#endif  /* #if INLINE_VOFUNCTION == 1 */

/*===============+ �̳��������ַ���ģʽ +================*/

/*  
* 1. �̳��������ַ���˳��OBS��SBO��
* ��1��OBS��others before self���ȷ��ʽڵ��IBN����ʽڵ�����
* ��2��SBO��self before others���ȷ��ʽڵ��������ʽڵ��IBN��
*
* 2. �̳��������ַ��ʷ�ʽ�������ͷǽ������ʡ�
* ��1���������ʣ�Relay Visiting��������һ���ڵ�󣬲�����ʲôִ�н����
*      �ŷ�����һ�ڵ㡣OBS����LND��ִ�н����SBO�ķ��ؽ�������������
*      ���ؼ̳�������FND��ִ�н�������ؼ̳�������RO__SBOOIX��
* ��2���෴���Ƿǽ������ʣ�Non-Relay Visiting��������һ���ڵ�������
*      ��RO_OBSOIX������OBS����RO_SBOOIX������SBO���ͷ���ִ�н��������
*      �ͷ�����һ���ڵ㡣
*
* 3. �̳��������ַ���˳������ַ��ʷ�ʽ���Ϊ�̳��������ַ���ģʽ��
* ��1��OBS_O_IX��OBS_OBJECT_<INTERFACE> �͵ĺ꣬��OBS˳��ǽ������ʣ�
* ��2��OBS_O_IX_��OBS_OBJECT_<INTERFACE>_ �͵ĺ꣬��OBS˳��������ʣ�
* ��3��SBO_O_IX��SBO_OBJECT_<INTERFACE> �͵ĺ꣬��SBO˳��ǽ������ʣ�
* ��4��_SBO_O_IX��_SBO_OBJECT_<INTERFACE> �͵ĺ꣬��SBO˳��������ʡ�
*
* ע��1. Open��Close�ӿ�ֻ�ܰ�OBS˳��������ʣ�����Open��Close�ӿ�û
*        ��OBS_O_IX��SBO_O_IX��_SBO_O_IX��
*     2. ͬһ�ӿڣ����ַ���ģʽֻ��ѡ����һ�����ͬһ�ӿ��ڲ���ͬʱ��
*        ���ֲ�ͬ�ķ���ģʽ��
*     3. ��������һ�������ڵ��ؼ̳У����ڶ��ؼ̳����岻��
*/

/*
*
* ���ƣ�OBS_OBJECT_OPEN_
* �������꣬Open�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_OPEN_ \
	do{ \
		RESULT   ir; \
		RESULT   fr; \
		NUMIB	 i; \
		VR	Caller; \
		EM_OIOIC*  pem = (EM_OIOIC*)GetEMofBN(This, MSN_OIOIC); \
		Caller.cr = pCaller->cr; \
		Caller.mr = This->OID; \
  		if(0 == ++This->RefCnt) \
		{ \
			This->RefCnt--; \
			return  RN_FULL; \
		} \
		if(!(MOD_SHARED & pem->mds) || 1 == This->RefCnt) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			for(i=0; i<This->NIB; i++) \
			{ \
				fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
				if(R_P == fr) ir = (*(This->ppo_AIB+i))->Open(*(This->ppo_AIB+i), pCaller); \
				else if(RN_HASVR == fr) continue; \
				else ir = R_N; \
				if( RN(ir) ) \
				{ \
					NUMIB  j; \
					for( j=0; j<i; j++) \
					{ \
						if((*(This->ppo_AIB+j))->RefCnt == This->RefCnt) \
							VO_Close(*(This->ppo_AIB+j), &Caller); \
					} \
					This->RefCnt--; \
					return ir; \
				} \
			} \
		}else \
		{ \
			This->RefCnt--; \
			return RN_UNSHARED; \
		} \
	}while(0)
	
/*
*
* ���ƣ�OBS_OBJECT_INPUT
* �������꣬Input�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_INPUT \
	do{ \
		RESULT   ir = RO_OBSOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Input(*(This->ppo_AIB+i), IStrm, Qty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_INPUT_
* �������꣬Input�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_INPUT_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Input(*(This->ppo_AIB+i), IStrm, Qty, pCaller); \
		} \
	}while(0)

/*
*
* ���ƣ�SBO_OBJECT_INPUT
* �������꣬Input�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_INPUT \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Input(*(This->ppo_AIB+i), IStrm, Qty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�_SBO_OBJECT_INPUT
* �������꣬Input�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_INPUT \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				ir = (*(This->ppo_AIB+i))->Input(*(This->ppo_AIB+i), IStrm, Qty, pCaller); \
		} \
		return ir; \
	}while(0)
	
/*
*
* ���ƣ�OBS_OBJECT_OUTPUT
* �������꣬Output�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_OUTPUT \
	do{ \
		RESULT   ir = RO_OBSOIX;  \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Output(*(This->ppo_AIB+i), OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_OUTPUT_
* �������꣬Output�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_OUTPUT_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Output(*(This->ppo_AIB+i), OStrm, Cty, pQty, pCaller); \
		} \
	}while(0)

/*
*
* ���ƣ�SBO_OBJECT_OUTPUT
* �������꣬Output�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_OUTPUT \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Output(*(This->ppo_AIB+i), OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�_SBO_OBJECT_OUTPUT
* �������꣬Output�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_OUTPUT \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				ir = (*(This->ppo_AIB+i))->Output(*(This->ppo_AIB+i), OStrm, Cty, pQty, pCaller); \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_IOPUT
* �������꣬IOput�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_IOPUT \
	do{ \
		RESULT   ir = RO_OBSOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->IOput(*(This->ppo_AIB+i), IStrm, Qty, OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_IOPUT_
* �������꣬IOput�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_IOPUT_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->IOput(*(This->ppo_AIB+i), IStrm, Qty, OStrm, Cty, pQty, pCaller); \
		} \
	}while(0)

/*
*
* ���ƣ�SBO_OBJECT_IOPUT
* �������꣬IOput�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_IOPUT \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->IOput(*(This->ppo_AIB+i), IStrm, Qty, OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�_SBO_OBJECT_IOPUT
* �������꣬IOput�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_IOPUT \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				ir = (*(This->ppo_AIB+i))->IOput(*(This->ppo_AIB+i), IStrm, Qty, OStrm, Cty, pQty, pCaller); \
		} \
		return ir; \
	}while(0)
	
/*
*
* ���ƣ�OBS_OBJECT_INTERACT0
* �������꣬Interact0�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_INTERACT0 \
	do{ \
		RESULT   ir = RO_OBSOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact0(*(This->ppo_AIB+i), Act, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_INTERACT0_
* �������꣬Interact0�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_INTERACT0_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Interact0(*(This->ppo_AIB+i), Act, pCaller); \
		} \
	}while(0)

/*
*
* ���ƣ�SBO_OBJECT_INTERACT0
* �������꣬Interact0�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_INTERACT0 \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact0(*(This->ppo_AIB+i), Act, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)
	
/*
*
* ���ƣ�_SBO_OBJECT_INTERACT0
* �������꣬Interact0�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_INTERACT0 \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Interact0(*(This->ppo_AIB+i), Act, pCaller); \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_INTERACT1
* �������꣬Interact1�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_INTERACT1 \
	do{ \
		RESULT   ir = RO_OBSOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact1(*(This->ppo_AIB+i), Act, OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_INTERACT1_
* �������꣬Interact1�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_INTERACT1_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Interact1(*(This->ppo_AIB+i), Act, OStrm, Cty, pQty, pCaller); \
		} \
	}while(0)

/*
*
* ���ƣ�SBO_OBJECT_INTERACT1
* �������꣬Interact1�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_INTERACT1 \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact1(*(This->ppo_AIB+i), Act, OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�_SBO_OBJECT_INTERACT1
* �������꣬Interact1�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_INTERACT1 \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				ir = (*(This->ppo_AIB+i))->Interact1(*(This->ppo_AIB+i), Act, OStrm, Cty, pQty, pCaller); \
		} \
		return ir; \
	}while(0)
	
/*
*
* ���ƣ�OBS_OBJECT_INTERACT2
* �������꣬Interact2�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_INTERACT2 \
	do{ \
		RESULT   ir = RO_OBSOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact2(*(This->ppo_AIB+i), Act, IStrm, Qty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_INTERACT2_
* �������꣬Interact2�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_INTERACT2_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Interact2(*(This->ppo_AIB+i), Act, IStrm, Qty, pCaller); \
		} \
	}while(0)

/*
*
* ���ƣ�SBO_OBJECT_INTERACT2
* �������꣬Interact2�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_INTERACT2 \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact2(*(This->ppo_AIB+i), Act, IStrm, Qty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�_SBO_OBJECT_INTERACT2
* �������꣬Interact2�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_INTERACT2 \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				ir = (*(This->ppo_AIB+i))->Interact2(*(This->ppo_AIB+i), Act, IStrm, Qty, pCaller); \
		} \
		return ir; \
	}while(0)
	
/*
*
* ���ƣ�OBS_OBJECT_INTERACT3
* �������꣬Interact3�ӿڵ�OBS_O_IX��
*/
#define  OBS_OBJECT_INTERACT3 \
	do{ \
		RESULT   ir = RO_OBSOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact3(*(This->ppo_AIB+i), Act, IStrm, Qty, OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_OBSOIX) return ir; \
		} \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_INTERACT3_
* �������꣬Interact3�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_INTERACT3_ \
	do{ \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				(*(This->ppo_AIB+i))->Interact3(*(This->ppo_AIB+i), Act, IStrm, Qty, OStrm, Cty, pQty, pCaller); \
		} \
	}while(0)
 
/*
*
* ���ƣ�SBO_OBJECT_INTERACT3
* �������꣬Interact3�ӿڵ�SBO_O_IX��
*/
#define  SBO_OBJECT_INTERACT3 \
	do{ \
		RESULT   ir = RO_SBOOIX; \
		RESULT   fr; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
			if(R_P == fr) ir = (*(This->ppo_AIB+i))->Interact3(*(This->ppo_AIB+i), Act, IStrm, Qty, OStrm, Cty, pQty, pCaller); \
			else if(RN_HASVR == fr) continue; \
			else ir = R_N; \
			if(ir != RO_SBOOIX) return ir; \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�_SBO_OBJECT_INTERACT3
* �������꣬Interact3�ӿڵ�_SBO_O_IX��
*/
#define  _SBO_OBJECT_INTERACT3 \
	do{ \
		RESULT   ir = RO__SBOOIX; \
		NUMIB    i; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if(R_P == CallerCome(*(This->ppo_AIB+i), pCaller)) \
				ir = (*(This->ppo_AIB+i))->Interact3(*(This->ppo_AIB+i), Act, IStrm, Qty, OStrm, Cty, pQty, pCaller); \
		} \
		return ir; \
	}while(0)

/*
*
* ���ƣ�OBS_OBJECT_CLOSE_
* �������꣬Close�ӿڵ�OBS_O_IX_��
*/
#define  OBS_OBJECT_CLOSE_ \
	do{ \
		RESULT   ir; \
		RESULT   fr; \
		NUMIB    i; \
		if( 0 == This->RefCnt ) \
			return RO_NOPEN; \
		This->RefCnt--; \
		for(i=0; i<This->NIB; i++) \
		{ \
			extern  RESULT  CallerCome(OBJECT* pND, const VR* pCaller); \
			if((*(This->ppo_AIB+i))->RefCnt -1 == This->RefCnt) \
			{ \
				fr = CallerCome(*(This->ppo_AIB+i), pCaller); \
				if(R_P == fr) ir = (*(This->ppo_AIB+i))->Close(*(This->ppo_AIB+i), pCaller); \
				else if(RN_HASVR == fr) continue; \
				else ir = R_N; \
				if( RN(ir) ) \
				{ \
					This->RefCnt++; \
					return ir; \
				} \
			} \
		} \
	}while(0)

/*==========+  CreateObject & DestroyObject  +===========*/

/*
*
* ���ƣ�CreateObject
* �������꣬��̬��������
* ������Msn --- [IN] ģ��ţ�ָ�������ĸ�ģ�Ķ���
*       IQCty --- [IN] ������е�������
*       OQCty --- [IN] ������е�������
*       NumCR --- [IN] ������������
*       ppObj --- [OUT] ��������Ķ���
*/
extern  RESULT  __CreateObject__(MLDSN Msn, BYTKTY IQCty, BYTKTY OQCty, SI32 NumCR, OBJECT** ppObj, BYTE* pExotic);
#define  CreateObject(Msn, IQCty, OQCty, NumCR, ppObj) \
	    __CreateObject__(Msn, IQCty, OQCty, NumCR, ppObj, NULL)

/*
*
* ���ƣ�DestroyObject
* �������꣬���ٶ�̬�����Ķ��󣬼�������CreateObject�괴���Ķ���
* ������pObj --- [IN] ָ�����
*/
extern  BOOL  __DestroyObject__(OBJECT*  pObj); 
#define  DestroyObject( pObj ) \
	do{ \
		if(TRUE == __DestroyObject__(pObj)) \
		{ \
			free(pObj); \
			(pObj) = NULL; \
		} \
	}while(0) 

/*===========+  ������������ֱ�ӵ��õĺ���  +============*/

extern  RESULT   Enqueue(QUEUE* pq, BYTE* IData, BYTKTY Qty);
extern  RESULT   Dequeue(QUEUE* pq, BYTE* OData, BYTKTY Cty, BYTKTY* pQty);
extern  OBJECT*  GetBN(OBJECT* pND, MLDSN msn);
extern  EM*      GetEMofBN(OBJECT* pND, MLDSN msn);
extern  QUEUE*   GetIQ(OBJECT* pND);
extern  QUEUE*   GetOQ(OBJECT* pND);

#if INLINE_VOFUNCTION == 0

extern  RESULT   VO_Open(OBJECT* pObject, const VR* pCaller);
extern  RESULT   VO_Input(OBJECT* pObject, BYTE* IStrm, BYTKTY Qty, const VR* pCaller);
extern  RESULT   VO_Output(OBJECT* pObject, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller); 
extern  RESULT   VO_IOput(OBJECT* pObject, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
extern  RESULT   VO_Interact0(OBJECT* pObject, ACTION Act, const VR* pCaller); 
extern  RESULT   VO_Interact1(OBJECT* pObject, ACTION Act, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
extern  RESULT   VO_Interact2(OBJECT* pObject, ACTION Act, BYTE* IStrm, BYTKTY Qty, const VR* pCaller);
extern  RESULT   VO_Interact3(OBJECT* pObject, ACTION Act, BYTE* IStrm, BYTKTY Qty, BYTE* OStrm, BYTKTY Cty, BYTKTY* pQty, const VR* pCaller);
extern  RESULT   VO_Close(OBJECT* pObject, const VR* pCaller);

#endif


#endif  /* #ifndef __O_H__ */
