#include "stdafx.h"
#include "HookApi.h"
#include "shlwapi.h"

#pragma comment(lib, "shlwapi.lib")

CApiInfo::CApiInfo()
{
   m_count =0;//��ǰ����
   m_pList =NULL;//��ǰָ��
}
CApiInfo::~CApiInfo()
{
   DeleteAllHooks();
}
//���һ���ҹ�
PAPIINFO CApiInfo::AddHook(char* modulename, char* apiname, char* myapiname, int paramcount, APIFUNC oldapi,
		             APIFUNC myapi, char* myfriendapiname, int startpos)
{
 	PAPIINFO pinfo = new APIINFO;
	strcpy(pinfo->module_name, modulename);
	strcpy(pinfo->api_name, apiname);
	strcpy(pinfo->my_api_name, myapiname);
	pinfo->param_count =paramcount;
	pinfo->old_api =oldapi;
	pinfo->my_api =myapi;
	if(myfriendapiname)
		strcpy(pinfo->my_friend_api_name, myfriendapiname);
	else pinfo->my_friend_api_name[0] =0;
	pinfo->start_pos = startpos;
	pinfo->bHooking = false;
	InitializeCriticalSection(&pinfo->cs);
    pinfo->pNext =NULL;
	APIINFO *pinfo2 =m_pList;
	while(pinfo2 !=NULL && pinfo2->pNext !=NULL)
		pinfo2 =pinfo2->pNext;
	if(pinfo2 ==NULL) 
		m_pList =pinfo;             // this is head of m_pInfo
	else
		pinfo2->pNext =pinfo;
	m_count++;
	return pinfo;
}
//ɾ�����йҹ�����
void CApiInfo::DeleteAllHooks()
{
    APIINFO *pinfo =m_pList, *pinfo2;
	while(pinfo)
	{
		pinfo2 =pinfo;
		pinfo =pinfo->pNext;
		DeleteCriticalSection(&pinfo2->cs);
		free(pinfo2);
	}
	m_pList =NULL;
	m_count =0;

}
PAPIINFO CApiInfo::FindByMyApi(APIFUNC myapi)
{
    APIINFO *pinfo =m_pList;

	while(pinfo)
	{
		if(pinfo->my_api == myapi)
			break;
		pinfo =pinfo->pNext;
	}

	return pinfo;
}
PAPIINFO CApiInfo::FindByOldApi(APIFUNC oldapi)
{
	PAPIINFO pInfo = m_pList;
	while(pInfo)
	{
		if ((BYTE*)oldapi == (BYTE*)pInfo->old_api+pInfo->start_pos)
			break;
		pInfo = pInfo->pNext;
	}
    return pInfo;
}
PAPIINFO CApiInfo::FindByApiName(char* apiname)
{
    APIINFO *pinfo =m_pList;

	while(pinfo)
	{
		if(!strcmp(pinfo->api_name, apiname))
			break;
		pinfo =pinfo->pNext;
	}

	return pinfo;
}
PAPIINFO CApiInfo::FindByMyApiName(char* myapiname)
{
    APIINFO *pinfo =m_pList;

	while(pinfo)
	{
		if(!strcmp(pinfo->my_api_name, myapiname))
			break;
		pinfo =pinfo->pNext;
	}

	return pinfo;
}
void CApiInfo::Lock(PAPIINFO pInfo)
{
	EnterCriticalSection(&pInfo->cs);
}
void CApiInfo::Unlock(PAPIINFO pInfo)
{
	LeaveCriticalSection(&pInfo->cs);
}

bool WINAPI InjectLib(DWORD process_id, char *lib_name)
{
    HANDLE hProcess =OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	if(hProcess ==NULL)
	   return false;
	bool bRet = InjectLib(hProcess, lib_name);
	CloseHandle(hProcess);
	return bRet;
}

bool WINAPI InjectLib(HANDLE hProcess, char *lib_name)
{
	if (hProcess == NULL)
		return false;
	PTHREAD_START_ROUTINE pLoad = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "LoadLibraryA");
	if (pLoad == NULL)
		return false;
    int size = strlen(lib_name) + 1;
	void* pBuf = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT, PAGE_READWRITE);
	if (pBuf == NULL)
		return false;
    int nRet = WriteProcessMemory(hProcess, pBuf, lib_name, size, NULL);
	if (nRet == STATUS_ACCESS_VIOLATION || nRet == 0)
	{
		VirtualFreeEx(hProcess, pBuf, 0, MEM_RELEASE);
		return false;
	}
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pLoad, pBuf, 0, NULL);
    if (hThread == NULL)
	{
		VirtualFreeEx(hProcess, pBuf, 0, MEM_RELEASE);
		return false;
	}
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, pBuf, 0, MEM_RELEASE);
	CloseHandle(hThread);
	return true;;
}

bool WINAPI EjectLib(DWORD process_id, char *lib_name)
{
    HANDLE hProcess =OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	if(hProcess ==NULL)
	   return false;
	bool bRet = EjectLib(hProcess, lib_name);
	CloseHandle(hProcess);
	return bRet;
}

bool WINAPI EjectLib(HANDLE hProcess, char *lib_name)
{
	if (hProcess == NULL)
		return false;
	PTHREAD_START_ROUTINE pLoad = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("Kernel32"), "FreeLibrary");
	if (pLoad == NULL)
		return false;
    int size = strlen(lib_name) + 1;
	void* pBuf = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT, PAGE_READWRITE);
	if (pBuf == NULL)
		return false;
    int nRet = WriteProcessMemory(hProcess, pBuf, lib_name, size, NULL);
	if (nRet == STATUS_ACCESS_VIOLATION || nRet == 0)
	{
		VirtualFreeEx(hProcess, pBuf, 0, MEM_RELEASE);
		return false;
	}
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pLoad, pBuf, 0, NULL);
    if (hThread == NULL)
	{
		VirtualFreeEx(hProcess, pBuf, 0, MEM_RELEASE);
		return false;
	}
	WaitForSingleObject(hThread, INFINITE);
	VirtualFreeEx(hProcess, pBuf, 0, MEM_RELEASE);
	CloseHandle(hThread);
	return true;;
}

//////////////////////////////////////////////////////////////////////
// CHookApi Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHookApi::CHookApi()
{
    m_strDllPath = "";
	m_hMyDll = NULL;
    //װ��Dll HookAPINT.dll
	HMODULE hMod = GetModuleHandle(HOOKAPI_DLL_NAME);
	char modname[MAX_PATH];
	//�����ģ���Ѿ������ڴ�
	if (hMod)
	{
		GetModuleFileName(hMod, modname, MAX_PATH);
		PathRemoveFileSpec(modname);
		WriteProfileString("HookAPI", "DLL_PATH", modname);
	}
	//���û��װ�سɹ��� ��ȡע����ȡdll·��
	else 
		GetProfileString("HookAPI", "DLL_PATH", "", modname, sizeof(modname));
	m_strDllPath = modname;
	//���ϴ�����Ϊ�˻��mydll.dll��·���� HOOKAPI_DLL_NAME�ǵ�ǰdll���ƣ� ��mydll.dll��ͬһ��Ŀ¼��
    //װ��mydll.dll
	LoadMyDll();
}

CHookApi::~CHookApi()
{

}

bool CHookApi::LoadMyDll()
{
   //ֱ��װ�أ� �ڵ�ǰĿ¼�³ɹ�, �Ѿ������ڴ�
	if(GetModuleHandle(MY_DLL_NAME))
		return true;
    //������ڵ�ǰĿ¼��
	char fdll[128];
	//��ָ��·����װ��
	wsprintf(fdll, "%s\\%s", m_strDllPath, MY_DLL_NAME);
	if((m_hMyDll =LoadLibrary(fdll)) ==NULL)
	    return false;
	return true;
}
