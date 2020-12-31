#if !defined(AFX_SERIAL_H__756C04AE_85E2_4A6D_91A8_5E03B28AD2B8__INCLUDED_)
#define AFX_SERIAL_H__756C04AE_85E2_4A6D_91A8_5E03B28AD2B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Serial.h : header file
//
//  CSerial���������ڷ��͵���Ϣ
#define WM_COM_RECEIVE	WM_USER	+ 301
#define WM_COM_CTS		WM_USER	+ 302		
#define WM_COM_DSR		WM_USER	+ 303
#define WM_COM_RING		WM_USER	+ 304
#define WM_COM_RLSD		WM_USER	+ 306
#define WM_COM_BREAK	WM_USER	+ 307
#define WM_COM_TXEMPTY	WM_USER	+ 308
#define WM_COM_ERROR	WM_USER	+ 309

#define DEFAULT_COM_MASK_EVENT EV_RXCHAR | EV_ERR  \
				| EV_CTS | EV_DSR | EV_BREAK | EV_TXEMPTY | EV_RING | EV_RLSD

/////////////////////////////////////////////////////////////////////////////
// CSerial command target

class CSerial : public CCmdTarget
{
	DECLARE_DYNCREATE(CSerial)

	CSerial(bool bNeedThread = false);           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual DWORD Write(LPVOID Buffer, DWORD dwBufferLen);
	virtual DWORD WriteString(LPTSTR szBuffer);;
	virtual void SetWnd(HWND hWnd);
	virtual BOOL SetTimeouts(LPCOMMTIMEOUTS lpCO);
	virtual void SetNotifyNum(DWORD dwNum);
	virtual void SetMaskEvent(DWORD dwMask);
	virtual BOOL SetDTR(BOOL OnOrOff);
	virtual BOOL SetRTS(BOOL OnOrOff);
	virtual BOOL SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize);;
	virtual bool SetBreak(bool bOn);
	virtual TCHAR * ReadString(TCHAR *szBuffer, DWORD dwBufferLen, DWORD dwWaitTime);
	/*virtual */bool ReadData(PVOID Buffer, DWORD dwBufferLen, DWORD dwWaitTime);
	virtual bool Purge(DWORD dwFlag = PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	virtual bool Open(DWORD dwPort, DWORD dwBaudRate);
	virtual bool IsOpen();
	virtual void Close();
	virtual bool SetState(DWORD dwBaudRate, DWORD dwByteSize = 8, 
				DWORD dwPatity = NOPARITY, DWORD dwStopBits = ONESTOPBIT);

	virtual DWORD Read(LPVOID Buffer, DWORD dwBufferLen, DWORD dwWaitTime);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerial)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSerial();

	// Generated message map functions
	//{{AFX_MSG(CSerial)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	virtual bool OpenCommPort(DWORD dwPort);
	virtual void OnTXEmpty();
	virtual void OnRLSD();
	virtual void OnRing();
	virtual void OnReceive();
	virtual void OnError();
	virtual void OnDSR();
	virtual void OnCTS();
	virtual void OnBreak();
	HANDLE m_hCom;//�򿪵�com�ھ��
	OVERLAPPED m_ReadOL;  //������overlapped�ṹ
	OVERLAPPED m_WriteOL; //д����overlapped�ṹ
	OVERLAPPED m_WaitOL;  //�ȴ�����overlapped�ṹ
	BOOL m_bExit;
	HANDLE m_hThread;
	DCB  m_dcb;           //com�˿ڲ�����dcb�ṹ
	DWORD m_dwMaskEvent;  //com�ڽ����¼�������
	HWND m_hNotifyWnd;    //�����Ĵ���
	DWORD m_dwNotifyNum;  //�¼�֪ͨ����
	char m_szComm[16];    //com������
	bool m_bNeedThread;   //�Ƿ���Ҫ�����Ĵ����߳�
private:
	virtual DWORD ThreadProc();
	static DWORD WINAPI SerialThreadProc(LPVOID pParam);
};

/////////////////////////////////////////////////////////////////////////////
// ����Э���е�֡ͷ
struct CSerialFrame
{
	BYTE command;  //����
	BYTE reserved; //Ԥ��
	USHORT sequence;//���к�
	USHORT uChecksum;//У���
	USHORT uDataLen;//���ݳ���
};
//����Э������֡ͷ��command�ֶ�����
#define SERIAL_CONNECT		0x01		// ��������
#define SERIAL_ACK			0x02		// ȷ��
#define SERIAL_DATA			0x03		// ����
#define SERIAL_ERROR		0x04		// ����  Ҫ���ط�
#define SERIAL_SIZE			0x05		// ���ݴ�С
#define SERIAL_ABORT		0x06		// ��ֹ
#define SERIAL_EOF			0x07		// ����
#define SERIAL_TOOBIG		0x08		// �ļ�̫��
#define SERIAL_SYSFAILED	0x09		// ϵͳ����
#define SERIAL_INIT			0x0a		// ��ʼ��
#define SERIAL_GETSTATUS	0x0b		// ��ȡ��ǰ״̬
#define SERIAL_TRANSFAILED  0x0c		// �ļ�����ʧ��
#define SERIAL_VERIFYFAILED 0x0d		// �û���֤ʧ��
#define SERIAL_DEVICEFAILED 0x10		// �豸����

// ����֡��С
#define FRAME_SIZE     156
// ״̬
#define STATUS_TRANSFERING	0x03		// ���ڴ����ļ�
#define STATUS_IDLE			0x04		// ����
#define STATUS_CONNECTED	0x05		// �Ѿ�����



class CSCSerial : public CSerial  
{
public:
	virtual BOOL SendRetry();
    virtual BOOL SendInit();
	virtual BOOL SendFileSize(DWORD dwSize);
	virtual BOOL SendFileData(LPVOID Buffer, int nBufferLen);
	virtual BOOL SendEOF();
	virtual BOOL SendData(LPVOID Buffer, int nBufferLen);
	virtual BOOL SendCommand(BYTE command);
	virtual BOOL SendAck();
	virtual bool SetAbort();
	virtual CSerialFrame* ReadFrame();
	/*virtual */CSerialFrame* ReadData2();
	CSCSerial(bool bNeedThread = false);
	virtual ~CSCSerial();

	char m_Buffer[FRAME_SIZE + sizeof(CSerialFrame) + 1];  //�ڲ��������� ��������֡ͷ������֡�� ��������Ƶ�������ڴ�
	USHORT m_sequence;  //���к�
	int m_nStatus;      // ��¼��ǰ״̬
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIAL_H__756C04AE_85E2_4A6D_91A8_5E03B28AD2B8__INCLUDED_)
