/**
 *
 * �� �� ����se.h
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

#ifndef __WGIDEMO_SE_H__
#define __WGIDEMO_SE_H__

#include "../ho.h"
#include "wgi.h"

/*
*
* NAME: EBITMAP
*/
typedef  enum  TAG_EBITMAP
{
	/*- TRCClose -*/
	eTRCClose_Normal_BMP,
	eTRCClose_FocusOver_BMP,
	eTRCClose_FocusOn_BMP,
	
	/*- Button2 -*/
	eButton2_Normal_BMP,
	eButton2_FocusOver_BMP,
	eButton2_FocusOn_BMP,
	eButton2_Active_BMP,
	eButton2_ABSInactive_BMP,

	/*- number of enum -*/
    eNum_BMP
}EBITMAP;


#endif		/* #ifndef __WGIDEMO_SE_H__ */
