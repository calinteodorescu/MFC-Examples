
// MetroUIDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../../MetroUI/MetroUI/MetroUI.h"
//#pragma comment(lib,"../../MetroUI/Debug/MetroUI.lib")
#pragma comment(lib,"../../MetroUI/Release/MetroUI.lib")


// CMetroUIDemoApp:
// �йش����ʵ�֣������ MetroUIDemo.cpp   
//
 
class CMetroUIDemoApp : public CWinAppEx
{
public:
	CMetroUIDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMetroUIDemoApp theApp;