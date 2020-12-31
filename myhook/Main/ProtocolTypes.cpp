#include "StdAfx.h"
#include "ProtocolTypes.h"
///////////////////////////////////////////////////////////////////////
//
// ������       : checksum
// ��������     : ����У���
// ����         : USHORT* buff		Ҫ����У��͵Ļ�����
// ����         : int size			buff�������Ĵ�С
// ����ֵ       : USHORT			���ݵ�У���	
//
///////////////////////////////////////////////////////////////////////
USHORT CProtocolHeader::ComputeChecksum(USHORT* buff, int size) 
{
	unsigned long cksum = 0;
	while(size>1)
	{
		cksum += *buff++;
		size -= sizeof(USHORT);
	}
	// ������
	if(size)
	{
		cksum += *(UCHAR*)buff;
	}
	// ��32λ��chsum��16λ�͵�16λ��ӣ�Ȼ��ȡ��
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);			
	return (USHORT)(~cksum);
}
USHORT checksum(USHORT* buff, int size) 
{
    unsigned long cksum = 0;
	while(size>1)
	{
		cksum += *buff++;
		size -= sizeof(USHORT);
	}
	// ������
	if(size)
	{
		cksum += *(UCHAR*)buff;
	}
	// ��32λ��chsum��16λ�͵�16λ��ӣ�Ȼ��ȡ��
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);			
	return (USHORT)(~cksum);
}
/* 
����UDPαͷУ��͡�UDPУ��ͻ������¼�����
	ԴIP��ַ
	Ŀ��IP��ַ
	8λ0��
	8λЭ����
	16λUDP����
	16λԴ�˿ں�
	16λĿ�Ķ˿ں�
	16λUDP�������
	16λUDPУ��ͣ�0��
	UDP����
 */
void _UDPHeader::ComputeUdpPseudoHeaderChecksum(IPHeader *pIphdr, UDPHeader *pUdphdr, char *payload, int payloadlen)
{
	char buff[1024];
	char *ptr = buff;
	int chksumlen = 0;
	ULONG zero = 0;
	
	// ����ԴIP��ַ��Ŀ��IP��ַ
	memcpy(ptr, &pIphdr->ipSource, sizeof(pIphdr->ipSource));
	ptr += sizeof(pIphdr->ipSource);
	chksumlen += sizeof(pIphdr->ipSource);

	memcpy(ptr, &pIphdr->ipDestionation, sizeof(pIphdr->ipDestionation));
	ptr += sizeof(pIphdr->ipDestionation);
	chksumlen += sizeof(pIphdr->ipDestionation);

	// ����8λ0��
	memcpy(ptr, &zero, 1);
	ptr += 1;
	chksumlen += 1;

	// Э��
	memcpy(ptr, &pIphdr->ipProtocol, sizeof(pIphdr->ipProtocol));
	ptr += sizeof(pIphdr->ipProtocol);
	chksumlen += sizeof(pIphdr->ipProtocol);

	// UDP����
	memcpy(ptr, &pUdphdr->len, sizeof(pUdphdr->len));
	ptr += sizeof(pUdphdr->len);
	chksumlen += sizeof(pUdphdr->len);

	// UDPԴ�˿ں�
	memcpy(ptr, &pUdphdr->sourcePort, sizeof(pUdphdr->sourcePort));
	ptr += sizeof(pUdphdr->sourcePort);
	chksumlen += sizeof(pUdphdr->sourcePort);

	// UDPĿ�Ķ˿ں�
	memcpy(ptr, &pUdphdr->destinationPort, sizeof(pUdphdr->destinationPort));
	ptr += sizeof(pUdphdr->destinationPort);
	chksumlen += sizeof(pUdphdr->destinationPort);

	// ����UDP����
	memcpy(ptr, &pUdphdr->len, sizeof(pUdphdr->len));
	ptr += sizeof(pUdphdr->len);
	chksumlen += sizeof(pUdphdr->len);

	// 16λ��UDPУ��ͣ���Ϊ0
	memcpy(ptr, &zero, sizeof(USHORT));
	ptr += sizeof(USHORT);
	chksumlen += sizeof(USHORT);

	// ����
	memcpy(ptr, payload, payloadlen);
	ptr += payloadlen;
	chksumlen += payloadlen;

	// ���뵽��һ��16λ�߽�
	for(int i=0; i<payloadlen%2; i++)
	{
		*ptr = 0;
		ptr++;
		chksumlen++;
	}
	// �������У��ͣ��������䵽UDPͷ
	pUdphdr->checksum = ComputeChecksum((USHORT*)buff, chksumlen);
}
 /* 
����TCPαͷУ��͡�TCPУ��ͻ������¼�����
	ԴIP��ַ
	Ŀ��IP��ַ
	8λ0��
	8λЭ����
	16λTCP����
	TCPͷ
	TCP����
 */
void _TCPHeader::ComputeTcpPseudoHeaderChecksum(IPHeader *pIphdr, TCPHeader *pTcphdr, char *payload, int payloadlen)
{
	char buff[1024];
	char *ptr = buff;
	int chksumlen = 0;
	ULONG zero = 0;
	
		// αͷ
	// ����ԴIP��ַ��Ŀ��IP��ַ
	memcpy(ptr, &pIphdr->ipSource, sizeof(pIphdr->ipSource));
	ptr += sizeof(pIphdr->ipSource);
	chksumlen += sizeof(pIphdr->ipSource);

	memcpy(ptr, &pIphdr->ipDestionation, sizeof(pIphdr->ipDestionation));
	ptr += sizeof(pIphdr->ipDestionation);
	chksumlen += sizeof(pIphdr->ipDestionation);

	// ����8λ0��
	memcpy(ptr, &zero, 1);
	ptr += 1;
	chksumlen += 1;

	// Э��
	memcpy(ptr, &pIphdr->ipProtocol, sizeof(pIphdr->ipProtocol));
	ptr += sizeof(pIphdr->ipProtocol);
	chksumlen += sizeof(pIphdr->ipProtocol);

	// TCP����
	USHORT tcp_len = htons(sizeof(TCPHeader) + payloadlen);
	memcpy(ptr, &tcp_len, sizeof(tcp_len));
	ptr += sizeof(tcp_len);
	chksumlen += sizeof(tcp_len);

		// TCPͷ
	memcpy(ptr, pTcphdr, sizeof(TCPHeader));
	ptr += sizeof(TCPHeader);
	chksumlen += sizeof(TCPHeader);

		// ����
	memcpy(ptr, payload, payloadlen);
	ptr += payloadlen;
	chksumlen += payloadlen;

	// ���뵽��һ��16λ�߽�
	for(int i=0; i<payloadlen%2; i++)
	{
		*ptr = 0;
		ptr++;
		chksumlen++;
	}

	// �������У��ͣ��������䵽TCPͷ
	pTcphdr->checksum = ComputeChecksum((USHORT*)buff, chksumlen);
}


_IPHeader::_IPHeader(char* szSourceIP, char* szDestIP, int nProtocol, UINT nTTL, char* pMsg, int nMsgLen)
{
	iphVerLen = (4<<4 | (sizeof(IPHeader)/sizeof(ULONG)));
	ipLength = ::htons(sizeof(IPHeader) + sizeof(UDPHeader) + nMsgLen);
	ipTTL = nTTL;
	ipProtocol = nProtocol;
	ipSource = ::inet_addr(szSourceIP);
	ipDestionation = ::inet_addr(szDestIP);
	ipChecksum = 0;
	ipChecksum = ComputeChecksum((USHORT*)this, sizeof(IPHeader));
}

_TCPHeader::_TCPHeader(PIPHeader pIpHdr, USHORT uSourcePort, USHORT uDestPort, ULONG uSequenceNumb, UCHAR flags, int windowsize, char* pMsg, int nMsgLen)
{
	sourcePort = htons(uSourcePort);
	destinationPort = htons(uDestPort);
	sequenceNumber = htonl(uSequenceNumb);
	acknowledgeNumber = 0;
	dataoffset =  (sizeof(TCPHeader)/4<<4|0); 
    flags = flags;   // #define   TCP_SYN   0x02
	urgentPointer = 0;
	windows = htons(windowsize);
	checksum = 0;
	ComputeTcpPseudoHeaderChecksum(pIpHdr, this, pMsg, nMsgLen);
}

_UDPHeader::_UDPHeader(PIPHeader pIpHdr, USHORT uSourcePort, USHORT uDestPort, char* pMsg, int nMsgLen)
{
	sourcePort = htons(uSourcePort);
	destinationPort = htons(uDestPort);
	len = htons(sizeof(UDPHeader) + nMsgLen);
	checksum = 0;
	ComputeUdpPseudoHeaderChecksum(pIpHdr, this, pMsg, nMsgLen);

}
//������Ether Header
bool _UDPHeader::ComposeUdpPacket(char* szRetbuf, int& nBufLen, char* szSourceIP, char* szDestIP, 
					  USHORT uSourcePort, USHORT uDestPort, UINT nTTL, char* pMsg, int nMsgLen)
{
	int nSize = sizeof(IPHeader) + sizeof(UDPHeader) + nMsgLen;
	if (szRetbuf == NULL || nBufLen < nSize)
	{
		nBufLen = nSize;
		return false;
	}
	nBufLen = nSize;
	IPHeader iphdr(szSourceIP, szDestIP, IPPROTO_UDP, nTTL, pMsg, nMsgLen);
	UDPHeader udpHdr(&iphdr, uSourcePort, uDestPort, pMsg, nMsgLen);
	memcpy(szRetbuf, &iphdr, sizeof(iphdr));
	memcpy(szRetbuf+sizeof(IPHeader), &udpHdr, sizeof(udpHdr));
	memcpy(szRetbuf+sizeof(IPHeader)+sizeof(UDPHeader), pMsg, nMsgLen);
	return true;
}
//����Ether Header
bool _UDPHeader::ComposeUdpPacket2(char* szRetbuf, int& nBufLen, UCHAR sourMac[6], UCHAR destMac[6], char* szSourceIP, char* szDestIP, USHORT uSourcePort, USHORT uDestPort, UINT nTTL, char* pMsg, int nMsgLen)
{
    int nSize = sizeof(ETHeader) + sizeof(IPHeader) + sizeof(UDPHeader) + nMsgLen;
	if (szRetbuf == NULL || nBufLen < nSize)
	{
		nBufLen = nSize;
		return false;
	}
    ETHeader ethdr(sourMac, destMac, ETHERTYPE_IP);
	memcpy(szRetbuf, &ethdr, sizeof(ETHeader));
	char* pBuf = szRetbuf + sizeof(ETHeader);
	int nLen = nBufLen - sizeof(ETHeader);
	ASSERT(ComposeUdpPacket(pBuf, nLen, szSourceIP, szDestIP, uSourcePort, uDestPort, nTTL, pMsg, nMsgLen));
	nBufLen = nSize;
	return true;
}
//������Ether Header
bool _TCPHeader::ComposeTcpPacket(char* szRetbuf, int& nBufLen, char* szSourceIP, char* szDestIP, UINT nTTL, 
					  USHORT uSourcePort, USHORT uDestPort, ULONG uSequenceNumb, UCHAR flags,
					  int windowsize, char* pMsg, int nMsgLen)
{
    int nSize = sizeof(IPHeader) + sizeof(TCPHeader) + nMsgLen;
	if (szRetbuf == NULL || nBufLen < nSize)
	{
		nBufLen = nSize;
		return false;
	}
	nBufLen = nSize;
	IPHeader iphdr(szSourceIP, szDestIP, IPPROTO_TCP, nTTL, pMsg, nMsgLen);
	TCPHeader tcphdr(&iphdr, uSourcePort, uDestPort,  uSequenceNumb,  flags, windowsize,  pMsg,  nMsgLen);
    memcpy(szRetbuf, &iphdr, sizeof(iphdr));
	memcpy(szRetbuf+sizeof(IPHeader), &tcphdr, sizeof(tcphdr));
	memcpy(szRetbuf+sizeof(IPHeader)+sizeof(TCPHeader), pMsg, nMsgLen);
	return true;
}
//����Ether Header
bool _TCPHeader::ComposeTcpPacket2(char* szRetbuf, int& nBufLen, UCHAR sourMac[6], UCHAR destMac[6], char* szSourceIP, char* szDestIP, UINT nTTL, 
					  USHORT uSourcePort, USHORT uDestPort, ULONG uSequenceNumb, UCHAR flags,
					  int windowsize, char* pMsg, int nMsgLen)
{
    int nSize = sizeof(ETHeader) + sizeof(IPHeader) + sizeof(TCPHeader) + nMsgLen;
	if (szRetbuf == NULL || nBufLen < nSize)
	{
		nBufLen = nSize;
		return false;
	}

	ETHeader ethdr(sourMac, destMac, ETHERTYPE_IP);
	memcpy(szRetbuf, &ethdr, sizeof(ETHeader));
	char* pBuf = szRetbuf + sizeof(ETHeader);
	int nLen = nBufLen - sizeof(ETHeader);
	ASSERT(ComposeTcpPacket(pBuf, nLen, szSourceIP, szDestIP, nTTL, uSourcePort, uDestPort, uSequenceNumb, flags, windowsize, pMsg, nMsgLen));
	nBufLen = nSize;
	return true;
}

_ICMPHeader::_ICMPHeader(UCHAR uType, UCHAR uCode, USHORT nID, USHORT nSequence, char* pMsg, int nMsgLen)
{	
	Type		= uType;
	Code		= uCode;
	Checksum	= 0;
	ID			= nID;
	Seq			= nSequence;
	TimeStamp	= GetTickCount();
	ComputeICMPPesudoChecksum(this, pMsg, nMsgLen);
}

void _ICMPHeader::ComputeICMPPesudoChecksum(_ICMPHeader* pIcmpHdr, char* pMsg, int nMsgLen)
{
    int nSize = sizeof(ICMPHeader) + nMsgLen;
	char* buf = new char[nSize];
	memcpy(buf, pIcmpHdr, sizeof(ICMPHeader));
	memcpy(buf+sizeof(ICMPHeader), pMsg, nMsgLen);
	pIcmpHdr->Checksum = ComputeChecksum((USHORT*)buf, nSize);
	delete [] buf;	
}
//������Ether header
bool _ICMPHeader::ComposeIcmpPacket(char* szRetbuf, int& nBufLen, char* szSourceIP, char* szDestIP, UINT nTTL, UCHAR uType, UCHAR uCode, USHORT nID, USHORT nSequence, char* pMsg, int nMsgLen)
{
    int nSize = sizeof(IPHeader) + sizeof(ICMPHeader) + nMsgLen;
	if (szRetbuf == NULL || nBufLen < nSize)
	{
		nBufLen = nSize;
		return false;
	}
	nBufLen = nSize;
    IPHeader iphdr(szSourceIP, szDestIP, IPPROTO_TCP, nTTL, pMsg, nMsgLen);
    ICMPHeader icmphdr(uType, uCode, nID, nSequence, pMsg, nMsgLen);
	memcpy(szRetbuf, &iphdr, sizeof(iphdr));
	memcpy(szRetbuf+sizeof(IPHeader), &icmphdr, sizeof(icmphdr));
	memcpy(szRetbuf+sizeof(IPHeader)+sizeof(ICMPHeader), pMsg, nMsgLen);
	return true;
}
//����Ether Header
bool _ICMPHeader::ComposeIcmpPacket2(char* szRetbuf, int& nBufLen, UCHAR sourMac[6], UCHAR destMac[6], char* szSourceIP, char* szDestIP, UINT nTTL, UCHAR uType, UCHAR uCode, USHORT nID, USHORT nSequence, char* pMsg, int nMsgLen)
{
    int nSize = sizeof(ETHeader) + sizeof(IPHeader) + sizeof(ICMPHeader) + nMsgLen;
	if (szRetbuf == NULL || nBufLen < nSize)
	{
		nBufLen = nSize;
		return false;
	}

	ETHeader ethdr(sourMac, destMac, ETHERTYPE_IP);
	memcpy(szRetbuf, &ethdr, sizeof(ETHeader));
	char* pBuf = szRetbuf + sizeof(ETHeader);
	int nLen = nBufLen - sizeof(ETHeader);
	ASSERT(ComposeIcmpPacket(pBuf, nLen, szSourceIP, szDestIP, nTTL, uType, uCode, nID, nSequence, pMsg, nMsgLen));
	nBufLen = nSize;
	return true;
}