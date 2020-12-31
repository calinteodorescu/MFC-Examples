// GroupTalk.h: interface for the CGroupTalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPTALK_H__C66FE0C6_3683_4FB9_89D4_8C1E43D5FE51__INCLUDED_)
#define AFX_GROUPTALK_H__C66FE0C6_3683_4FB9_89D4_8C1E43D5FE51__INCLUDED_

#include <winsock2.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_GROUPTALK	WM_USER + 105    
//#define BUFFER_SIZE 4096
#define GROUP_PORT  4567

const enum
{
	MT_JION = 1,	// һ���û�����
	MT_LEAVE,		// һ���û��뿪
	MT_MESG,		// һ���û�������Ϣ

	// �ڲ�ʹ�ã������¼�����û��Լ����û���Ϣ
	MT_MINE
};

typedef struct gt_hdr
{
	u_char gt_type;			// ��Ϣ����
	DWORD dwAddr;			// ���ʹ���Ϣ���û���IP��ַ
	char szUser[15];		// ���ʹ���Ϣ���û����û���

	int nDataLength;		// �������ݵĳ���
	char *data() { return (char*)(this + 1); }
} GT_HDR;

class CGroupTalk  
{
public:
	void SendText(CString strText, DWORD dwAddr);
	CGroupTalk(CWnd* pNotifyWnd, DWORD dwMultiAddr, DWORD dwLocalAddr = 0, int nTTL = 128);
	virtual ~CGroupTalk();
	
protected:
	CWnd* m_pNotifyWnd;
	DWORD m_dwMultiAddr;
	DWORD m_dwLocalAddr;
	int m_nTTL;
	CWinThread* m_pThread;
	CEvent m_event;
	SOCKET m_sSend;
	SOCKET m_sRead;
	BOOL m_bQuit;
	char m_szUser[MAX_PATH];
private:
	void DispatchMsg(GT_HDR* pHeader, int nLen);
	BOOL JoinGroup();
	static UINT ThreadEntry(LPVOID pParam);
	BOOL LeaveGroup();
	int Send(char* pText, int nLen, DWORD dwRemoteAddr);
};

#endif
// !defined(AFX_GROUPTALK_H__C66FE0C6_3683_4FB9_89D4_8C1E43D5FE51__INCLUDED_)
