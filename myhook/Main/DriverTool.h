// DriverTool.h: interface for the CDriverTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVERTOOL_H__257EF964_5488_41B6_9618_A742B05DD18B__INCLUDED_)
#define AFX_DRIVERTOOL_H__257EF964_5488_41B6_9618_A742B05DD18B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDriverTool  
{
public:
	CDriverTool();
	virtual ~CDriverTool();

	// �������Ƿ����
	virtual BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }
	// ��������Ҳ����˵������DriverEntry������������
	virtual BOOL StartDriver(LPCTSTR pszDriverPath = NULL);
	// �������񡣼����������DriverUnload���̽�������
	virtual BOOL StopDriver(LPCTSTR pszDriverPath = NULL);
	// ���豸����ȡ�õ���������һ�����
	virtual HANDLE  OpenControlDevice(LPCTSTR pszDriverPath = NULL);
	// ���豸���Ϳ��ƴ���
	virtual DWORD IoControl(HANDLE hDriver, DWORD nCode, PVOID pInBuffer, DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
	//ж������
	virtual BOOL UnInstallDriver(LPCTSTR pszDriverPath = NULL);
	//��װ����
	virtual bool  InstallDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName, bool bAttach = true);
	// ʵ��
protected:
	char m_szLinkName[MAX_PATH];//������������
	bool m_bStarted;//�Ƿ�����
	bool m_bCreateService;//�Ƿ���������
	HANDLE m_hSCM;//������ƹ��������
	HANDLE m_hService;//������
	HANDLE m_hDriver;//��������� �ļ�
};


#endif // !defined(AFX_DRIVERTOOL_H__257EF964_5488_41B6_9618_A742B05DD18B__INCLUDED_)
