/**
 *
 * �� �� ����oioic.h
 *
 * ��    ����OIOIC �ڵ�� .h �ļ���
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

#ifndef __OIOIC_H__
#define __OIOIC_H__
   
#include "o.h"


/*========================+ INIT +=======================*/

/*
*
* ���ƣ�INIT_OIOIC
*/
/*typedef struct TAG_INIT_OIOIC
{
}INIT_OIOIC;*/


/*==================+ Extended Member +==================*/

/*
*
* ���ƣ�EM_OIOIC
*/
typedef struct TAG_EM_OIOIC
{
	QUEUE	   IQ;	/* ������С�*/  
	QUEUE	   OQ;  /* ������С�*/  
	MODES      mds;	/* ģʽ����*/
}EM_OIOIC;


#endif  /* #ifndef __OIOIC_H__ */
