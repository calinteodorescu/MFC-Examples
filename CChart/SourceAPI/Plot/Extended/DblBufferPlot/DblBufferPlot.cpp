/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2011 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/

/* ############################################################################################################################## */

#include "stdafx.h"
#include "DblBufferPlot.h"

/////////////////////////////////////////////////////////////////////////
// Class CPlot

CDblBufferPlot::CDblBufferPlot()
{
	m_hWnd = NULL;
	m_hMemDC = NULL;
	SetDefaults();
}

CDblBufferPlot::~CDblBufferPlot()
{
}

void	CDblBufferPlot::SetDefaults()
{
	m_bUpdate = true;
}

// Copy the settings from one chart to another
void	CDblBufferPlot::CopySettings( const CDblBufferPlot *plot )
{
	// Then CDblBufferPlot
	m_bUpdate = plot->m_bUpdate;
}

void	CDblBufferPlot::CreateVirtualWindow()
{
	// �鴰�ڴ���
	// ׼���鴰�ڣ�����ػ�����
	HDC hDC=GetDC(m_hWnd);
	HBITMAP hMembmp;
	m_maxX=GetSystemMetrics(SM_CXSCREEN);
	m_maxY=GetSystemMetrics(SM_CYSCREEN);
	hMembmp=CreateCompatibleBitmap(hDC,m_maxX,m_maxY);
	m_hMemDC=CreateCompatibleDC(hDC); 
	SelectObject(m_hMemDC,hMembmp); 
	RECT rect; 
	SetRect(&rect,0,0,m_maxX,m_maxY);
	HBRUSH hBrush;
	hBrush = CreateSolidBrush(RGB(255,255,255)); 
	SelectObject(m_hMemDC,hBrush); 
	FillRect(m_hMemDC,&rect,hBrush);
	DeleteObject(hBrush);
	DeleteObject(hMembmp);
	
	ReleaseDC(m_hWnd,hDC);
	// �鴰�ڴ������
}

void	CDblBufferPlot::ReleaseVirtualWindow()
{
	// �鴰�ڴ���
	DeleteDC(m_hMemDC);
	// �鴰�ڴ������
}

