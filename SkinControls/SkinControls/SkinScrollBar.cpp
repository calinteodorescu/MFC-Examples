
#include "StdAfx.h"
#include "Resource.h"
#include "SkinScrollBar.h"
#include "SkinSB.h"

CBitmap CSkinScrollBar::m_bmScrollBar;

// ���캯��
CSkinScrollBar::CSkinScrollBar()
{
	if (m_bmScrollBar.m_hObject == NULL) m_bmScrollBar.LoadBitmap(IDB_SCROLLBAR);
}

// ��������
CSkinScrollBar::~CSkinScrollBar()
{
}

// ���ù���
void CSkinScrollBar::InitScroolBar(CWnd* pWnd)
{
	// ���ù���
	SkinSB_InitH(pWnd->GetSafeHwnd(), (HBITMAP)m_bmScrollBar);

	return;
}
