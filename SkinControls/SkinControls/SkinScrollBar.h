
#ifndef SKIN_SCROLLBAR_H_
#define SKIN_SCROLLBAR_H_
#pragma once

#include "SkinControlsDefine.h"

// ��������
class SKIN_CONTROLS_EXPORT CSkinScrollBar
{
	// ��������
public:
	// ���캯��
	CSkinScrollBar();
	// ��������
	virtual ~CSkinScrollBar();

	// ���ܺ���
public:
	// ���ù���
	void InitScroolBar(CWnd* pWnd);

	// ��Դ����
public:
	static CBitmap m_bmScrollBar;
};

#endif // SKIN_SCROLLBAR_H_