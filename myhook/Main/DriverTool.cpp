// DriverTool.cpp: implementation of the CDriverTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "main.h"
#include "DriverTool.h"
#include "stdafx.h"
#include "main.h"
#include "HookDriver.h"
#include <winsvc.h>
#include "Helper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriverTool::CDriverTool()
{
	//�������ÿ�
	memset(m_szLinkName, 0, MAX_PATH);
	m_bStarted = FALSE;
	m_bCreateService = FALSE;
	m_hSCM = NULL;
	m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;
	// ��SCM������
	m_hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hSCM == NULL)
		AfxMessageBox("�򿪷�����ƹ�����ʧ��", MB_OK, MB_ICONERROR);
}

CDriverTool::~CDriverTool()
{
	//�رշ�����������
	if (m_hSCM)
		CloseServiceHandle((SC_HANDLE)m_hSCM);
}
//ж����������
BOOL CDriverTool::UnInstallDriver(LPCTSTR pszDriverPath)
{   
	if (pszDriverPath == NULL)
	{
		//�ر����������ļ����
		if (m_hDriver != INVALID_HANDLE_VALUE)
			CloseHandle(m_hDriver);
		//ֹͣ��������
		if (m_bStarted && m_hService)
			StopDriver();
		//����������������� ɾ����������
		if (m_bCreateService && m_hService)
			DeleteService((SC_HANDLE)m_hService);
		//�ر�����������
		if (m_hService)
			CloseServiceHandle((SC_HANDLE)m_hService);
		
		//�������ÿ�
		memset(m_szLinkName, 0, MAX_PATH);
		m_bStarted = FALSE;
		m_bCreateService = FALSE;
		m_hSCM = NULL;
		m_hService = NULL;
		m_hDriver = INVALID_HANDLE_VALUE;
		return true;
	}
	else
	{
		SC_HANDLE hService = NULL;
		char szLinkName[MAX_PATH] = { 0 };
		//��ȡ���������Link����
		GetProfileString("DriverLinkName", pszDriverPath, "", szLinkName, MAX_PATH);
		if (strcmp(szLinkName, "") == 0)
			return false;
		//���������ƹ�����û�д�
		if (m_hSCM == NULL)
			return false;
		//�򿪷���
		hService = OpenService((SC_HANDLE)m_hSCM, szLinkName, SERVICE_ALL_ACCESS);
		//�������û�д�
		if (hService == NULL)
			return false;
		SERVICE_STATUS ss;
		QueryServiceStatus((SC_HANDLE)hService, &ss);
		if (ss.dwCurrentState == SERVICE_RUNNING)
			StopDriver(pszDriverPath);
		BOOL bDeleted = DeleteService(hService);
        CloseServiceHandle(hService);
		return bDeleted;
	}
}
//��װ��������
bool CDriverTool::InstallDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName, bool bAttach)
{
	SC_HANDLE hService = NULL;
	if (bAttach)
	{
		if (m_hService)
		    CloseServiceHandle(m_hService);
		if (m_hDriver != INVALID_HANDLE_VALUE)
			CloseHandle(m_hDriver);
    	//������������
	    memset(m_szLinkName, 0, MAX_PATH);
	    strncpy(m_szLinkName, pszLinkName, wcslen((PWCHAR)pszLinkName)*2);
	    m_bStarted = FALSE;
	    m_bCreateService = FALSE;
	    m_hService = NULL;
	    m_hDriver = INVALID_HANDLE_VALUE;
    }
	    // ������򿪷���
	hService = CreateService((SC_HANDLE)m_hSCM, pszLinkName, pszLinkName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
		                       SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, pszDriverPath, NULL, 
							   0, NULL, NULL, NULL);
	if (hService == NULL)
	{
		int nError = GetLastError();
		if (nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
			hService = OpenService((SC_HANDLE)m_hSCM, pszLinkName, SERVICE_ALL_ACCESS);
	}
	else
	{
		if (bAttach)
		   m_bCreateService = TRUE;
		//д��ע���
        WriteProfileString("DriverLinkName", pszDriverPath, pszLinkName);
	}

	if (bAttach)
		m_hService = hService;

	if (hService == NULL)
		return false;
	else
		return true;
}
// ���豸���Ϳ��ƴ���
DWORD CDriverTool::IoControl(HANDLE hDriver, DWORD nCode, PVOID pInBuffer, DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount)
{
	if (hDriver == INVALID_HANDLE_VALUE)
		return -1;
	DWORD nBytesRet;
	BOOL bRet = DeviceIoControl(hDriver, nCode, pInBuffer, nInCount, pOutBuffer, nOutCount, &nBytesRet, NULL);
	if (bRet)
		return nBytesRet;
	else
		return -1;
}
//�������豸�ļ����
HANDLE CDriverTool::OpenControlDevice(LPCTSTR pszDriverPath)
{
	char szLinkName[MAX_PATH] = { 0 };
	HANDLE hDriver = INVALID_HANDLE_VALUE;
	//���û��ָ������·��
	if (pszDriverPath == NULL)
	{
	    if (m_hDriver != INVALID_HANDLE_VALUE)
		   return m_hDriver;
		if (strcmp(m_szLinkName, "") == 0)
			return INVALID_HANDLE_VALUE;
		memcpy(szLinkName, m_szLinkName, MAX_PATH);
    }
	else//���ָ������·��
	{
		//��ȡ���������Link����
        GetProfileString("DriverLinkName", pszDriverPath, "", szLinkName, MAX_PATH);
		if (strcmp(szLinkName, "") == 0)
			return FALSE;
	}
	
	char sz[MAX_PATH] = { 0 };
    wsprintf(sz, "\\\\.\\%s", szLinkName);
	hDriver = CreateFile(sz, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//���û��ָ������·��
	if (pszDriverPath == NULL)
        m_hDriver = hDriver;
	return hDriver;
}
//ֹͣ��������
BOOL CDriverTool::StopDriver(LPCTSTR pszDriverPath)
{
	bool bStarted = true;
	SC_HANDLE hService = NULL;
	char szLinkName[MAX_PATH] = { 0 };
	//���û��ָ������·��
	if (pszDriverPath == NULL)
	{
		if (!m_bStarted)
			return TRUE;
		if (m_hService == NULL)
			return FALSE;
		hService = m_hService;
	}
    else//���ָ������·��
	{
		//��ȡ���������Link����
        GetProfileString("DriverLinkName", pszDriverPath, "", szLinkName, MAX_PATH);
		if (strcmp(szLinkName, "") == 0)
			return FALSE;
		//���������ƹ�����û�д�
		if (m_hSCM == NULL)
			return false;
		//�򿪷���
		hService = OpenService((SC_HANDLE)m_hSCM, szLinkName, SERVICE_ALL_ACCESS);
	}

	if (hService == NULL)
		return false;

	SERVICE_STATUS ss;
	if (!ControlService((SC_HANDLE)hService, SERVICE_CONTROL_STOP, &ss))
    {
		if (GetLastError() == ERROR_SERVICE_NOT_ACTIVE)
			bStarted = FALSE;
	}
	else
	{
	    int nTry = 0;
		while(ss.dwCurrentState == SERVICE_STOP_PENDING && nTry ++ < 80)
		{
			Sleep(50);
			QueryServiceStatus((SC_HANDLE)hService, &ss);
		}
		if (ss.dwCurrentState == SERVICE_STOPPED)
			bStarted = FALSE;
	}

	if (hService == m_hService)
		m_bStarted = bStarted;
    if (pszDriverPath != NULL)
		CloseServiceHandle(hService);
	return !bStarted;
}
//������������
BOOL CDriverTool::StartDriver(LPCTSTR pszDriverPath)
{
	SC_HANDLE hService = NULL;
	char szLinkName[MAX_PATH] = { 0 };
	bool bStarted = false;
	//���û��ָ������·��
	if (pszDriverPath == NULL)
	{
		//�Ѿ�����
		if (m_bStarted)
		   return TRUE;
		//�������û�а�װ
	    if (m_hService == NULL)
	  	   return FALSE;
        hService = m_hService;
	}
	else//���ָ������·��
	{
		//��ȡ���������Link����
        GetProfileString("DriverLinkName", pszDriverPath, "", szLinkName, MAX_PATH);
		if (strcmp(szLinkName, "") == 0)
			return FALSE;
		//���������ƹ�����û�д�
		if (m_hSCM == NULL)
			return false;
		//�򿪷���
		hService = OpenService((SC_HANDLE)m_hSCM, szLinkName, SERVICE_ALL_ACCESS);
	}
	if (hService == NULL)
		return false;
	if (!StartService((SC_HANDLE)hService, 0, NULL))
	{
		DWORD dwError = GetLastError();
		CString strError;
		GetErrorMessage(dwError, strError);
		AfxMessageBox(strError);

		if (dwError == ERROR_SERVICE_ALREADY_RUNNING)
		     bStarted = TRUE;
		//else
		//	DeleteService((SC_HANDLE)hService);
	}
	else
	{
		int nTry = 0;
		SERVICE_STATUS ss;
		QueryServiceStatus((SC_HANDLE)hService, &ss);
		while(ss.dwCurrentState == SERVICE_START_PENDING && nTry ++ < 80)
		{
			Sleep(50);
            QueryServiceStatus((SC_HANDLE)hService, &ss);
		}
		if (ss.dwCurrentState == SERVICE_RUNNING)
		    bStarted = TRUE;
	}
	if (hService == m_hService)
		m_bStarted = bStarted;
	if (pszDriverPath != NULL)
		CloseServiceHandle(hService);
	return bStarted;
}