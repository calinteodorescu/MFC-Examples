// HookDriver.h: interface for the CHookDriver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOOKDRIVER_H__647A55DC_52B2_4852_8853_E232E328707B__INCLUDED_)
#define AFX_HOOKDRIVER_H__647A55DC_52B2_4852_8853_E232E328707B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHookDriver  
{
public:
	CHookDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName);
	virtual ~CHookDriver();

	// �������Ƿ����
	virtual BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }
	// ��������Ҳ����˵������DriverEntry������������
	virtual BOOL StartDriver();
	// �������񡣼����������DriverUnload���̽�������
	virtual BOOL StopDriver();
	// ���豸����ȡ�õ���������һ�����
	virtual BOOL OpenDevice();
	// ���豸���Ϳ��ƴ���
	virtual DWORD IoControl(DWORD nCode, PVOID pInBuffer, 
			DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
	// ʵ��
protected:
	char m_szLinkName[MAX_PATH];//������������
	bool m_bStarted;//�Ƿ�����
	bool m_bCreateService;//�Ƿ���������
	HANDLE m_hSCM;//������ƹ��������
	HANDLE m_hService;//������
	HANDLE m_hDriver;//��������� �ļ�
};

#endif // !defined(AFX_HOOKDRIVER_H__647A55DC_52B2_4852_8853_E232E328707B__INCLUDED_)
