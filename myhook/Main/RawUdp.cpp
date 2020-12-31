// RawUdp.cpp: implementation of the CSendRawUdpPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "main.h"
#include "RawUdp.h"
#include "ProtocolTypes.h"
#include "Helper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSendRawUdpPacket::CSendRawUdpPacket()
{

}

CSendRawUdpPacket::~CSendRawUdpPacket()
{

}

bool CSendRawUdpPacket::SendPacket(char* pMsg, int nMsgLen, char* szSourceIP, char* szDestIP, USHORT uSourcePort, USHORT uDestPort, UINT nTTL)
{
	DWORD dwError;
	CString strError;
	char* buf = new char[1024];
    int nSize = 1024;
	UDPHeader header;
	if (!header.ComposeUdpPacket(buf, nSize, szSourceIP, szDestIP, uSourcePort, uDestPort, nTTL, pMsg, nMsgLen))
	{
		delete [] buf;
		buf = new char[nSize];
        header.ComposeUdpPacket(buf, nSize, szSourceIP, szDestIP, uSourcePort, uDestPort, nTTL, pMsg, nMsgLen);
	}
    SOCKET sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if (sRaw == INVALID_SOCKET)
	{
		AfxMessageBox("���ܴ���ԭʼ�׽���");
		delete [] buf;
		return false;
	}
    // ��ЧIPͷ����ѡ��
	BOOL bIncl = TRUE;
	if (SOCKET_ERROR == setsockopt(sRaw, IPPROTO_IP, IP_HDRINCL, (char *)&bIncl, sizeof(bIncl)))
	{
		AfxMessageBox("setsockopt(IP_HDRINCL)ʧ��");
        delete [] buf;
		closesocket(sRaw);
		return false;
	}

	MySockaddr_In addr(uDestPort, szDestIP);
	if (SOCKET_ERROR == sendto(sRaw, buf, nSize, 0, (LPSOCKADDR)&addr, sizeof(addr)))
	{
		dwError = WSAGetLastError();
		GetErrorMessage(dwError, strError);
        AfxMessageBox("sendtoʧ��: "+strError);
        delete [] buf;
		closesocket(sRaw);
		return false;
	}
	AfxMessageBox("���ͳɹ�");
	delete [] buf;
	closesocket(sRaw);
	return true;
}
