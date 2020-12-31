#ifndef __PROTOCOL_TYPES_H__
#define __PROTOCOL_TYPES_H__
//��������Э���ͷ����

class CProtocolHeader
{
	///////////////////////////////////////////////////////////////////////
	//
	// ������       : checksum
	// ��������     : ����У���
	// ����         : USHORT* buff		Ҫ����У��͵Ļ�����
	// ����         : int size			buff�������Ĵ�С
	// ����ֵ       : USHORT			���ݵ�У���	
	//
	///////////////////////////////////////////////////////////////////////
protected:
	USHORT ComputeChecksum(USHORT* buff, int size) ;
};
USHORT checksum(USHORT* buff, int size) ;
//��̫֡
//    Ŀ��MAC(6�ֽ�) | ԴMAC(6�ֽ�) | Э������(2�ֽ�) | IP���ݱ�(46---1500�ֽ�)
#define ETHERTYPE_IP 0x0800
#define ETHERTYPE_ARP 0x806
//��̫֡ͷ
typedef struct _ETHeader:public CProtocolHeader
{
	UCHAR destHost[6]; //Ŀ�Ļ�����MAC��ַ
	UCHAR sourceHost[6];//Դ������MAC��ַ
	USHORT type;//���ݵ�Э������
	_ETHeader()
	{
		memset(destHost, 0, 6);
		memset(sourceHost, 0, 6);
		type = 0;
	}
	_ETHeader(UCHAR sourceMac[6], UCHAR destMac[6], USHORT uType)
	{
		memcpy(sourceHost, sourceMac, 6);
        memcpy(destHost, destMac, 6);
        type = htons(uType);
	}
} ETHeader, *PETHeader;
//ARP���ݱ���ʽ
#define ARPHRD_ETHER 1
//ARP��������
#define ARPOP_REQUEST 1 //��ʾ��ARP���ݱ���ARP����
#define ARPOP_REPLY 2   //��ʾ��ARP���ݱ���ARP��Ӧ
//ARPͷ
typedef struct _ARPHeader:public CProtocolHeader
{
   USHORT hrd;    //Ӳ����ַ�ռ䣬 ΪARPHRD_ETHER
   USHORT eth_type;  //��̫������ ETHERTYPE_IP
   UCHAR  maclen;    //MAC��ַ���� 6
   UCHAR  iplen;     //ip��ַ����, 4
   USHORT opcode;    //�������룬 ARPOP_REQUEST�� ARPOP_REPLY
   UCHAR  sourcemac[6];   //ԴMAC
   UCHAR  sourceip[4];    //ԴIP
   UCHAR  destmac[6];     //Ŀ��MAC
   UCHAR  destip[4];      //Ŀ��IP
} ARPHeader, *PARPHeader;
//IP��ͷ
typedef struct _IPHeader:public CProtocolHeader
{
	UCHAR iphVerLen;   //�汾�ź�ͷ���� ��ռ4λ
	UCHAR ipTOS;       //��������
	USHORT ipLength;   //����ܳ��� ����IPͷ������
	USHORT ipID;       //�����ʶ�� Ψһ��ʶ���͵�ÿһ�����ݱ�
	USHORT  ipFlags;    //��־
	UCHAR  ipTTL;      //����ʱ�� ����TTL
	UCHAR  ipProtocol; //Э�飬 ������TCP, UDP, ICMP ....
	USHORT ipChecksum; //У���
	ULONG  ipSource;   //ԴIP��ַ
	ULONG  ipDestionation; //Ŀ��IP��ַ
	_IPHeader() {}
	_IPHeader(char* szSourceIP, char* szDestIP, int nProtocol, UINT nTTL, char* pMsg, int nMsgLen);
} IPHeader, *PIPHeader;
//TCP��ͷ
#define TCP_FLAG_FIN 0x01
#define TCP_FLAG_SYN 0x02
#define TCP_FLAG_RST 0x04
#define TCP_FLAG_PUSH 0x08
#define TCP_FLAG_ACK 0x10
#define TCP_FLAG_URG 0x20
#define TCP_FLAG_ACE_0x40
#define TCP_FLAG_CWR 0x80

typedef struct _TCPHeader:public CProtocolHeader
{
    USHORT	sourcePort;			// 16λԴ�˿ں�
	USHORT	destinationPort;	// 16λĿ�Ķ˿ں�
	ULONG	sequenceNumber;		// 32λ���к�
	ULONG	acknowledgeNumber;	// 32λȷ�Ϻ�
	UCHAR	dataoffset;			// ��4λ��ʾ����ƫ��
	UCHAR	flags;				// 6λ��־λ
	                            //FIN - 0x01
	                            //SYN - 0x02
	                            //RST - 0x04 
	                            //PUSH- 0x08
	                            //ACK- 0x10
	                            //URG- 0x20
	                            //ACE- 0x40
	                            //CWR- 0x80
	USHORT	windows;			// 16λ���ڴ�С
	USHORT	checksum;			// 16λУ���
	USHORT	urgentPointer;		// 16λ��������ƫ���� 
	_TCPHeader() {}
	_TCPHeader(PIPHeader pIpHdr, USHORT uSourcePort, USHORT uDestPort, ULONG uSequenceNumb, UCHAR flags, int windowsize = 512, char* pMsg = NULL, int nMsgLen = 0);
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
	void ComputeTcpPseudoHeaderChecksum(IPHeader *pIphdr, _TCPHeader *pTcphdr, char *payload, int payloadlen);
	//�齨TCP����//������Ether header
	bool ComposeTcpPacket(char* szRetbuf, int& nBufLen, char* szSourceIP, char* szDestIP,  UINT nTTL, 
					  USHORT uSourcePort, USHORT uDestPort, ULONG uSequenceNumb, UCHAR flags,
					  int windowsize, char* pMsg, int nMsgLen);
	//�齨TCP����//����Ether header
	bool ComposeTcpPacket2(char* szRetbuf, int& nBufLen, UCHAR sourMac[6], UCHAR destMac[6], char* szSourceIP, char* szDestIP, UINT nTTL, 
					  USHORT uSourcePort, USHORT uDestPort, ULONG uSequenceNumb, UCHAR flags,
					  int windowsize, char* pMsg, int nMsgLen);
} TCPHeader, *PTCPHeader;
//UDP��ͷ
typedef struct _UDPHeader:public CProtocolHeader
{
	USHORT sourcePort;  //Դ�˿ں�
    USHORT destinationPort; //Ŀ�Ķ˿ں�
	USHORT len;         //�������
	USHORT checksum;    //У���
	_UDPHeader() {}
	_UDPHeader(_IPHeader* pIpHdr, USHORT uSourcePort, USHORT uDestPort, char* pMsg, int nMsgLen);
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
	void ComputeUdpPseudoHeaderChecksum(_IPHeader *pIphdr, _UDPHeader *pUdphdr, char *payload, int payloadlen);
	//�齨UDP����//������Ether header
    bool ComposeUdpPacket(char* szRetbuf, int& nBufLen, char* szSourceIP, char* szDestIP, USHORT uSourcePort, USHORT uDestPort, UINT nTTL, char* pMsg, int nMsgLen);
	//�齨UDP����//����Ether header
    bool ComposeUdpPacket2(char* szRetbuf, int& nBufLen, UCHAR sourMac[6], UCHAR destMac[6], char* szSourceIP, char* szDestIP, USHORT uSourcePort, USHORT uDestPort, UINT nTTL, char* pMsg, int nMsgLen);
} UDPHeader, *PUDPHeader;

#define ICMP_TYPE_ECHO_REPLY	0       //����Ӧ��     code = 0
#define ICMP_TYPE_NET_NOT_AVAIABLE 3  //���粻�ɵ��� code = 0
                                    //�������ɵ��� code = 1
                                    //Э�鲻�ɵ��� code = 2
                                    //�˿ڲ��ɵ��� code = 3
                                    //��Ҫ�ָ ��������DF code = 4
                                    //Դ·��ʧ�� code = 5
                                    //Ŀ������δ֪ code = 6
                                    //Ŀ������δ֪ code = 7
                                    //code = 8
                                    //Ŀ�����类ǿ�н�ֹ code = 9
                                    //Ŀ��������ǿ�н�ֹ code = 10
                                    //��ΪTOS������������), ���粻�ɵ��� code = 11
                                    //��ΪTOS, �������ɵ��� code = 12
                                    //���ڹ��ˣ� ͨѶ��ǿ�н�ֹ code = 13
                                    //����ԽȨ code = 14
                                    //����Ȩ��ֹʧЧ code = 15
#define ICMP_TYPE_ERROR 4           // code = 0 Դ�˱��ر�
#define ICM_TYPE_ERROR_REDIRECT 5   //code =0  �������ض���
                                    //code == 1�������ض���
                                    //code = 2 ��TOS�������ض���
                                    //code = 3 ��TOS�������ض���
#define ICMP_TYPE_ECHO_REQUEST	8   //code = 0 �������
#define ICMP_TYPE_ROUTE_BROADCAST 9   //code = 0 ·��ͨ��
#define ICMP_TYPE_REQUEST_REQUEST 10 //·������ code = 0
#define ICMP_TYPE_ERROR_TIMEOUT   11 //code = 0 ��ʱ�� ������TTL == 0
#define ICMP_TYPE_ERROR_BAD_PARAMETER 12 //code == 0 IPͷ����
                                         // code = 1 ȱ�ٱ�Ҫ��ѡ��

// ICMP Header - RFC 792
typedef struct _ICMPHeader:public CProtocolHeader
{
	UCHAR	Type;			// Type
	UCHAR	Code;			// Code
	USHORT	Checksum;		// Checksum
	USHORT	ID;				// Identification
	USHORT	Seq;			// Sequence
	ULONG	TimeStamp;			// Data

	_ICMPHeader() { }
	_ICMPHeader(UCHAR uType, UCHAR uCode, USHORT nID, USHORT nSequence, char* pMsg, int nMsgLen);
    void ComputeICMPPesudoChecksum(_ICMPHeader* pIcmpHdr, char* pMsg, int nMsgLen);
	bool ComposeIcmpPacket(char* szRetbuf, int& nBufLen, char* szSourceIP, char* szDestIP, UINT nTTL, UCHAR uType, UCHAR uCode, USHORT nID, USHORT nSequence, char* pMsg, int nMsgLen);
    bool ComposeIcmpPacket2(char* szRetbuf, int& nBufLen, UCHAR sourMac[6], UCHAR destMac[6], char* szSourceIP, char* szDestIP, UINT nTTL, UCHAR uType, UCHAR uCode, USHORT nID, USHORT nSequence, char* pMsg, int nMsgLen);
}ICMPHeader, *PICMPHeader;

#define REQ_DATASIZE 32

/*
// ICMP Echo Request
typedef struct _ECHOREQUEST
{
	ICMPHeader icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;


// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHeader	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;
*/

// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHeader	ipHdr;
	ICMPHeader	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;

// ������˹���Ľṹ
typedef struct _IPFilterRule
{
    USHORT protocol;   //ʹ�õ�Э��
	ULONG sourceIP;    //ԴIP��ַ
	ULONG destIP;      //Ŀ��IP��ַ
	ULONG sourceMask;  //Դ��ַ������
	ULONG destMask;    //Ŀ�ĵ�ַ������
	USHORT sourcePort; //Դ�˿ں�
	USHORT destPort;   //Ŀ�Ķ˿ں�
	BOOLEAN bDrop;        //�Ƿ����ķ��
} IPFilterRule, *PIPFilterRule;

// ���˹����б�
typedef struct _FilterRuleList
{
    IPFilterRule ipf;
	_FilterRuleList* pNext;
} FilterRuleList, *PFilterRuleList;

struct MySockaddr_In : public sockaddr_in
{
    MySockaddr_In() { }
	MySockaddr_In(USHORT uPort, char* szIP)
	{
		sin_family = AF_INET;
		sin_port = htons(uPort);
		sin_addr.S_un.S_addr = inet_addr(szIP);
	}
	MySockaddr_In(USHORT uPort, ULONG dwIP = INADDR_ANY)
	{
        sin_family = AF_INET;
		sin_port = htons(uPort);
		sin_addr.S_un.S_addr = dwIP;
	}
};
// �Զ����豸���ͣ��ڴ����豸����ʱʹ��
// ע�⣬�Զ���ֵ�ķ�Χ��32768-65535
#define FILE_DEVICE_MYHOOKDRV 0x006543

// �Զ����IO���ƴ��룬�������ֲ�ͬ���豸��������
// ע�⣬�Զ���ֵ�ķ�Χ��2048-4095
#define MYHOOKDRV_IOCTL_INDEX 0x830

#undef FILE_ALL_ACCESS 
#define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)
//
//
// ��������豸���ƴ��롣�ֱ��ǿ�ʼ���ˡ�ֹͣ���ˡ���ӹ��˹���������˹���
//
#define START_IP_HOOK   CTL_CODE(FILE_DEVICE_MYHOOKDRV, MYHOOKDRV_IOCTL_INDEX, METHOD_BUFFERED, FILE_ALL_ACCESS)
#define STOP_IP_HOOK    CTL_CODE(FILE_DEVICE_MYHOOKDRV, MYHOOKDRV_IOCTL_INDEX+1, METHOD_BUFFERED, FILE_ALL_ACCESS)
#define ADD_FILTER      CTL_CODE(FILE_DEVICE_MYHOOKDRV, MYHOOKDRV_IOCTL_INDEX+2, METHOD_BUFFERED, FILE_ALL_ACCESS)
#define CLEAR_FILTER    CTL_CODE(FILE_DEVICE_MYHOOKDRV, MYHOOKDRV_IOCTL_INDEX+3, METHOD_BUFFERED, FILE_ALL_ACCESS)

#define MAX_RULE_NUM 256

#endif