// TestCTabSheet.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestCTabSheetApp:
// �йش����ʵ�֣������ TestCTabSheet.cpp
//

class CTestCTabSheetApp : public CWinApp
{
public:
	CTestCTabSheetApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestCTabSheetApp theApp;