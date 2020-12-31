// LSPHelper.cpp: implementation of the CLSPHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "main.h"
#include "LSPHelper.h"
#include <Sporder.h>
#include <rpc.h>
#include <vector>

using namespace std;

#pragma comment(lib, "Sporder.lib")
#pragma comment(lib, "Rpcrt4.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLSPHelper::CLSPHelper()
{
    m_pProtocolInfo = NULL;
	m_iProtocolNum = 0;
}

CLSPHelper::~CLSPHelper()
{
   if (m_pProtocolInfo != NULL)
	   GlobalFree(m_pProtocolInfo);
}

bool CLSPHelper::GetProvider(LPINT piProtocols)
{
	if (m_pProtocolInfo != NULL)
	   GlobalFree(m_pProtocolInfo);
	m_pProtocolInfo = NULL;

	DWORD dwSize = 0;
	int nError;
	LPWSAPROTOCOL_INFOW pInfo = NULL;

	if (SOCKET_ERROR == WSCEnumProtocols(piProtocols, pInfo, &dwSize, &nError))
	{
		if (nError != WSAENOBUFS)
			return false;
	}
	pInfo = (LPWSAPROTOCOL_INFOW)GlobalAlloc(GPTR, dwSize);
	m_iProtocolNum = WSCEnumProtocols(piProtocols, pInfo, &dwSize, &nError);
	if (m_iProtocolNum == SOCKET_ERROR)
	{
		m_iProtocolNum = 0;
		GlobalFree(pInfo);
		return false;
	}
	m_pProtocolInfo = pInfo;
	return true;
}

bool CLSPHelper::InstallProvider(WCHAR *szLSPName, WCHAR *szLSPDLLPath)
{
	LPWSAPROTOCOL_INFOW pInfo;
    WSAPROTOCOL_INFOW OrigProtocolInfo[3];
	DWORD dwOrigCatalogID[3];
    DWORD dwLayeredCatalogID;
	bool bFindTcp = false;
	bool bFindUdp = false;
	bool bFindRaw = false;

	//ö��ԭ����Э��
	if (!GetProvider(NULL))
		return false;
	int iCount = 0;

	//���Ȼ�ȡ�ײ��TCP, UDP, IPЭ��
	for(int i=0; i<m_iProtocolNum; i++)
	{
        pInfo = m_pProtocolInfo + i;
		
		if (pInfo->iAddressFamily != AF_INET)
			continue;
        
		if (!bFindUdp && pInfo->iProtocol == IPPROTO_UDP)
		{
			memcpy(&OrigProtocolInfo[iCount], pInfo, sizeof(WSAPROTOCOL_INFOW));
            OrigProtocolInfo[iCount].dwServiceFlags1 &= ~XP1_IFS_HANDLES;
            dwOrigCatalogID[iCount++] = pInfo->dwCatalogEntryId;
			bFindUdp = true;
		}
		else if (!bFindTcp && pInfo->iProtocol == IPPROTO_TCP)
		{
			memcpy(&OrigProtocolInfo[iCount], pInfo, sizeof(WSAPROTOCOL_INFOW));
            OrigProtocolInfo[iCount].dwServiceFlags1 &= ~XP1_IFS_HANDLES;
            dwOrigCatalogID[iCount++] = pInfo->dwCatalogEntryId;
			bFindTcp = true;
		}	
		else if (!bFindRaw && pInfo->iProtocol == IPPROTO_IP)
		{
			memcpy(&OrigProtocolInfo[iCount], pInfo, sizeof(WSAPROTOCOL_INFOW));
            OrigProtocolInfo[iCount].dwServiceFlags1 &= ~XP1_IFS_HANDLES;
            dwOrigCatalogID[iCount++] = pInfo->dwCatalogEntryId;
			bFindRaw = true;
		}	
	}
	//��װ�ֲ�Э�飬 ��ȡcatalogid, �����һ���²�Э�鸴�Ƽ���
	WSAPROTOCOL_INFOW LayeredInfo;
	memcpy(&LayeredInfo, &OrigProtocolInfo[0], sizeof(LayeredInfo));
	//��������
	wcscpy(LayeredInfo.szProtocol, szLSPName);
	//��������
	LayeredInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;
	//����PFL_HIDDEN��־
	LayeredInfo.dwProviderFlags |= PFL_HIDDEN;
	// ��װ
	GUID guid;
	if(::UuidCreate(&guid) != RPC_S_OK)
		return false;
	int nError;
	if (SOCKET_ERROR == WSCInstallProvider(&guid, szLSPDLLPath, &LayeredInfo, 1, &nError))
		return false;
	//�����о�Э��
	if (!GetProvider(NULL))
		return false;
	//��ȡ�ոհ�װ��Э���Ŀ¼���
	dwLayeredCatalogID = 0;
	for(i=0; i<m_iProtocolNum; i++)
	{
		pInfo = &m_pProtocolInfo[i];
		if (memcmp(&guid, &pInfo->ProviderId, sizeof(guid)) == 0)
		{
			dwLayeredCatalogID = pInfo->dwCatalogEntryId;
			break;
		}
	}
	if (dwLayeredCatalogID == 0)
		return false;

	WCHAR wszChainName[WSAPROTOCOL_LEN + 1];

	// ��װЭ����, ÿ���ײ�Э�鶼��һ����Ӧ��
	for(i=0; i<iCount; i++)
	{
		//��������Э�������
		swprintf(wszChainName, L"%ws On %ws", szLSPName, OrigProtocolInfo[i].szProtocol);
		wcscpy(OrigProtocolInfo[i].szProtocol, wszChainName);
		//���õײ�Э��Ŀ¼�������
		if (OrigProtocolInfo[i].ProtocolChain.ChainLen == 1)
             OrigProtocolInfo[i].ProtocolChain.ChainEntries[1] = dwOrigCatalogID[i];
		else
		{
			for(int j=OrigProtocolInfo[i].ProtocolChain.ChainLen; j>=0; j--)
				OrigProtocolInfo[i].ProtocolChain.ChainEntries[j] = OrigProtocolInfo[i].ProtocolChain.ChainEntries[j-1];
		}
		OrigProtocolInfo[i].ProtocolChain.ChainLen ++;
		OrigProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogID;
	}
	// ��ȡһ��Guid����װ֮
	GUID ProviderChainGuid;
	if (UuidCreate(&ProviderChainGuid) != RPC_S_OK)
		return false;
	if (SOCKET_ERROR == WSCInstallProvider(&ProviderChainGuid, szLSPDLLPath, OrigProtocolInfo, iCount, &nError))
		return false;
    // ��������WinsockĿ¼�������ǵ�Э������ǰ
	// ����ö�ٰ�װ��Э��
	if (!GetProvider(NULL))
		return false;

	DWORD dwIndex[100];
	int nIndex = 0;
    // ������ǵ�Э����
	for(i=0; i<m_iProtocolNum; i++)
	{
		pInfo = &m_pProtocolInfo[i];
		if((pInfo->ProtocolChain.ChainLen > 1) && (pInfo->ProtocolChain.ChainEntries[0] == dwLayeredCatalogID))
			dwIndex[nIndex++] = pInfo->dwCatalogEntryId;
	}
	// �������Э��
	for(i=0; i<m_iProtocolNum; i++)
	{
		pInfo = &m_pProtocolInfo[i];
		if((pInfo->ProtocolChain.ChainLen <= 1) || (pInfo->ProtocolChain.ChainEntries[0] != dwLayeredCatalogID))
			dwIndex[nIndex++] = pInfo->dwCatalogEntryId;
	}
	// ��������WinsockĿ¼
	if((nError = ::WSCWriteProviderOrder(dwIndex, nIndex)) != ERROR_SUCCESS)
	    return false;

	//дGUID��ע�����
	unsigned char* pGuid;
	UuidToString(&guid, &pGuid);
	int size = strlen((char*)pGuid) + 1;
	PWCHAR pWGuid = new WCHAR[size];
	ZeroMemory(pWGuid, size * 2);	
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCTSTR)pGuid, strlen((char*)pGuid), pWGuid, size);  
	WriteProfileStringW(L"InstalledLSP", szLSPDLLPath, pWGuid);
	delete [] pWGuid;
	RpcStringFree(&pGuid);
	return TRUE;
}

bool CLSPHelper::RemoveProvider(WCHAR* szLSPDllPath)
{
	unsigned char szGuid[MAX_PATH];
	GetProfileStringW(L"InstalledLSP", szLSPDllPath, L"\0", (WCHAR*)szGuid, MAX_PATH);
	PWCHAR wszGuid[MAX_PATH];
	memcpy(wszGuid, szGuid, 74);
	int nLen = wcslen((PWCHAR)wszGuid);
    
	unsigned char* buf = new unsigned char[nLen+1];
    unsigned char* p = buf;
	char ch;
	for(int i=0; i<nLen; i++)
	{
        ch = szGuid[i*2];
		*(p++) = ch; 
	}
    *p = 0;
	DWORD dwCatalogID = 0;
	GUID guid;
	if (RPC_S_OK != UuidFromString(buf, &guid))
	{
		delete [] buf;
		return false;
	}

	delete [] buf;

	if (!GetProvider(NULL))
		return false;
	int nError;
    //�ҵ��ֲ�Э���Ŀ¼���ID
	for(i=0; i<m_iProtocolNum; i++)
	{
		if(memcmp(&guid, &m_pProtocolInfo[i].ProviderId, sizeof(guid)) == 0)
		{
			dwCatalogID = m_pProtocolInfo[i].dwCatalogEntryId;
			break;
		}
	}

	if (dwCatalogID == 0)
		return false;
	vector<GUID> aDelGuid;
	aDelGuid.clear();
	// �Ƴ�Э����
	for(i=0; i<m_iProtocolNum; i++)
	{
		if((m_pProtocolInfo[i].ProtocolChain.ChainLen > 1) && (m_pProtocolInfo[i].ProtocolChain.ChainEntries[0] == dwCatalogID))
		{
			bool bFind = false;
			for(vector<GUID>::iterator it=aDelGuid.begin(); it!=aDelGuid.end(); it++)
			{
				if (memcmp(it, &m_pProtocolInfo[i].ProviderId, sizeof(GUID)) ==  0)
				{
					bFind = true;
					break;
				}	
			}
			if (bFind == true)
				continue;
			if (SOCKET_ERROR == ::WSCDeinstallProvider(&m_pProtocolInfo[i].ProviderId, &nError))
				return false;
			else
				aDelGuid.push_back(m_pProtocolInfo[i].ProviderId);
		}
	}
	// �Ƴ��ֲ�Э��
	if (SOCKET_ERROR == ::WSCDeinstallProvider(&guid, &nError))
		return false;
	return TRUE;
}

bool CLSPHelper::RemoveProvider(DWORD dwCatalogID)
{
	if (!GetProvider(NULL))
		return false;
	int nError;
	vector<GUID> aDelGuid;
	aDelGuid.clear();
    // �Ƴ�Э����
	for(int i=0; i<m_iProtocolNum; i++)
	{
		if((m_pProtocolInfo[i].ProtocolChain.ChainLen > 1) && (m_pProtocolInfo[i].ProtocolChain.ChainEntries[0] == dwCatalogID))
		{
			bool bFind = false;
			for(vector<GUID>::iterator it=aDelGuid.begin(); it!=aDelGuid.end(); it++)
			{
				if (memcmp(it, &m_pProtocolInfo[i].ProviderId, sizeof(GUID)) ==  0)
				{
					bFind = true;
					break;
				}	
			}
			if (bFind == true)
				continue;
			if (SOCKET_ERROR == ::WSCDeinstallProvider(&m_pProtocolInfo[i].ProviderId, &nError))
				return false;
			else
				aDelGuid.push_back(m_pProtocolInfo[i].ProviderId);
		}
		else if (m_pProtocolInfo[i].dwCatalogEntryId == dwCatalogID)
		{
             if (SOCKET_ERROR == ::WSCDeinstallProvider(&m_pProtocolInfo[i].ProviderId, &nError))
				return false;
		}
	}

	return TRUE;
}
