// ComFilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"
#include "ComFilDlg.h"
#include "ProtocolTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComFilDlg dialog
IMPLEMENT_DYNCREATE(CComFileDlg, CRootDlg)

CComFileDlg::CComFileDlg(CWnd* pParent /*=NULL*/)
	: CRootDlg(CComFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CRootDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComFileDlg)
	DDX_Control(pDX, IDC_BOUDRATE, m_BoudRate);
	DDX_Control(pDX, IDC_EDITLOG, m_Log);
	DDX_Control(pDX, IDC_BUTTONCLOSE, m_Close);
	DDX_Control(pDX, IDC_BUTTONSEND, m_Send);
	DDX_Control(pDX, IDC_BUTTONOPEN, m_Open);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_COMBOPORT, m_ComboPort);
	DDX_Control(pDX, IDC_EDITFILE, m_EditFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComFileDlg, CRootDlg)
	//{{AFX_MSG_MAP(CComFileDlg)
	ON_BN_CLICKED(IDC_BUTTONBROWSER, OnButtonbrowser)
	ON_BN_CLICKED(IDC_BUTTONCLOSE, OnButtonclose)
	ON_BN_CLICKED(IDC_BUTTONOPEN, OnButtonopen)
	ON_BN_CLICKED(IDC_BUTTONSEND, OnButtonsend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComFilDlg message handlers

BOOL CComFileDlg::OnInitDialog() 
{
	CRootDlg::OnInitDialog();
	
// ��ʼ�����û�ѡ��˿ڵ���Ͽ�Ĭ��ѡ��˿�COM1
	m_ComboPort.AddString("COM1");
	m_ComboPort.AddString("COM2");
	m_ComboPort.AddString("COM3");
	m_ComboPort.SelectString(0, "COM1");

	// ��ʼ�����û�ѡ�����ʵ���Ͽ�Ĭ��ѡ��38400   ע��ͨѶ˫��������Ӧ����ͬ
	m_BoudRate.InsertString(-1, "110");
	m_BoudRate.InsertString(-1, "300");
	m_BoudRate.InsertString(-1, "600");
	m_BoudRate.InsertString(-1, "1200");
	m_BoudRate.InsertString(-1, "2400");
	m_BoudRate.InsertString(-1, "9600");
	m_BoudRate.InsertString(-1, "14400");
	m_BoudRate.InsertString(-1, "19200");
	m_BoudRate.InsertString(-1, "38400");
	m_BoudRate.InsertString(-1, "56000");
	m_BoudRate.InsertString(-1, "115200");	
	m_BoudRate.SelectString(0, "38400");

	// ���ݶ˿��Ƿ�����ø�����ť��״̬
	BOOL bOpen = FALSE;
	m_Open.EnableWindow(!bOpen);
	m_Send.EnableWindow(bOpen);
	m_Close.EnableWindow(bOpen);

	// ��ʾ��ǰ״̬��Ϣ
	m_Status.SetWindowText(" �˿ڹر� ");
	
	m_pComFile = new CSCSerial(true);
	m_pData = NULL;
	m_hServerThread = NULL;

	return TRUE; 
}

void CComFileDlg::OnButtonbrowser() 
{
	// ���ļ�����Ի���
	CFileDialog dlg(TRUE); 
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}
	// ��¼�û�ѡ����ļ�
	m_EditFile.SetWindowText(dlg.GetPathName());	
}

void CComFileDlg::OnButtonclose() 
{
	// ����˿��Ѿ��رգ��򷵻�
	if(!m_pComFile->IsOpen())
		return;

	// ����������߳��˳�
	m_bThreadExit = TRUE;

	DWORD dwExitCode;
	int n=0;
	while(!::GetExitCodeThread(m_hServerThread, &dwExitCode) && n++ < 20)
	{
		::Sleep(100);
	}
	::TerminateThread(m_hServerThread, 0);
	::CloseHandle(m_hServerThread);
	m_hServerThread = NULL;

	// �ر�COM�˿�
	m_pComFile->Close();
}

void CComFileDlg::OnButtonopen() 
{
	// ����˿��Ѿ��򿪣��򷵻�
	if(m_pComFile->IsOpen())
		return;

	// ��ȡ�û�ѡ��Ķ˿ں�
	int nPort = m_ComboPort.GetCurSel() + 1;

	// ��ȡ�û�ѡ��Ĳ�����	
	int nRate;
	CString str;
	m_BoudRate.GetWindowText(str);
	nRate = atoi(str);
	if(nRate == 0)	
		nRate = 9600;

	// ���Դ򿪶˿�
	if(!m_pComFile->Open(nPort, nRate))
	{
		m_Status.SetWindowText(" �򿪶˿ڳ���");
		return;
	}

	// �˿ڴ򿪳ɹ�����ʾ��ǰ״̬��Ϣ
	m_Status.SetWindowText(" �˿��Ѿ��� ");

	// ���������̣߳�����κε���������
	DWORD dwId;
	m_hServerThread = ::CreateThread(NULL, 0, ServerThreadProc, this, 0, &dwId);

	// ���ݶ˿��Ƿ�����ø�����ť��״̬
	BOOL bOpen = TRUE;
	m_Open.EnableWindow(!bOpen);
	m_Send.EnableWindow(bOpen);
	m_Close.EnableWindow(bOpen);
}

void CComFileDlg::OnButtonsend() 
{
	// ����˿��Ѿ��رգ��򷵻�
	if(!m_pComFile->IsOpen())
	{
		return;	
	}

	// ��鵱ǰ״̬�������Ϊ���У�IDLE�����򷵻�
	if(m_pComFile->m_nStatus != STATUS_IDLE)
	{
		MessageBox(" ���ڽ����ļ������ܷ��ͣ���\r\n");
		return;
	}

	// ����û��Ƿ�������Ҫ������ļ�
	CString strFileName;
	m_EditFile.GetWindowText(strFileName);
	if(strFileName.IsEmpty())
	{
		MessageBox(" ����Ҫ���͵��ļ�����");
		return;
	}

	// ��ͼ����Ҫ������ļ�
	if(!m_SendFile.Open(strFileName, CFile::modeRead|CFile::shareDenyWrite))
	{
		MessageBox(" ���ļ���������");
		return;
	}

	m_bNeedSend = TRUE;

	// ��Ч���Ͱ�ť����ֹ��ֹ�û���ε��
	m_Send.EnableWindow(FALSE);
}

void CComFileDlg::OnDestroy() 
{
	CRootDlg::OnDestroy();
	
	// �رն˿ڣ��ر��¼������ͷ���Դ
	OnButtonclose();	
	delete m_pComFile;
}

void CComFileDlg::OnBtnClear() 
{
	m_Log.SetWindowText("");
}

void CComFileDlg::AddLog(LPCTSTR lpszString)		// �������������������־
{
	// ���ַ���lpszString��ӵ���־������
	CString str;
	m_Log.GetWindowText(str);
	str = lpszString + str;
	m_Log.SetWindowText(str);
}

BYTE CComFileDlg::SendConnect()
{
    // ����
	CSerialFrame *pFrame;
	if(!m_pComFile->SendCommand(SERIAL_CONNECT))
	{
		AddLog(" ���ӷ�����ʧ�ܣ��û��رմ��ڣ� \r\n ");
		return 0xff;
	}

	// �ȴ����
	pFrame = m_pComFile->ReadData2();
	if(pFrame == NULL)
	{
		AddLog(" ���ӷ�����ʧ�ܣ��Է�û����Ӧ�� \r\n ");
		return 0xff;
	}
	if(pFrame->command == SERIAL_ACK)
	{
		m_pComFile->m_nStatus = STATUS_CONNECTED;
		AddLog(" �ɹ����ӵ������� \r\n");
		return 0;
	}
	else
		return pFrame->command;
}

BYTE CComFileDlg::SendEOF()
{
	// ����
	CSerialFrame *pFrame;
	if(!m_pComFile->SendCommand(SERIAL_EOF))
	{
		AddLog(" �û��رն˿ڣ��ļ�����ʧ�ܣ� \r\n ");
		return 0xff;
	}

	// �ȴ����
	pFrame = m_pComFile->ReadData2();
	if(pFrame == NULL)
	{
		AddLog(" �Է�û����Ӧ���ļ�����ʧ�ܣ� \r\n");
		return 0xff;
	}
	if(pFrame->command == SERIAL_ACK)
	{
		m_pComFile->m_nStatus = STATUS_IDLE;
		AddLog(" �ļ����ͳɹ���� \r\n");
		return 0;
	}
	else
		return pFrame->command;
}

BOOL CComFileDlg::SendFile(CFile &file, CSCSerial *pSerial)
{
    DWORD dwTick = ::GetTickCount();	// ��ǰ����ʱ��
	int n;								// ֡�ط�����
	CString str;						// ��ʱ�ַ�������

	//////////////////////////////////////////////////
	// ׼�������ļ�
	AddLog(" ����׼�������ļ�... \r\n");
	// �����ļ���С
	int nFileSize = file.GetLength(); 
	if(!pSerial->SendFileSize(nFileSize))
	{
		AddLog(" �����ļ���Сʧ�ܣ�\r\n ");
		return FALSE;
	}


	//////////////////////////////////////////////////
	// �����ļ�����
	char Buffer[1024];			// ���ݻ�����
	int nRead;					// ���ļ���ÿ�ζ�ȡ���ֽ���
	DWORD dwTotalSendLen = 0;	// �ܹ����͵��ֽ���
	// ��ʼ�����к�Ϊ0
	pSerial->m_sequence = 0;
	AddLog(" ���ڷ����ļ�����... \r\n ");

	while(TRUE)
	{
		// ���ļ��ж�ȡ���ݵ�������
		nRead = file.Read(Buffer, 1024);
		if(nRead <= 0)
		{
			break;
		}

		// ���ͻ������е�����
		int nLeft = nRead;	// ʣ����ֽ���
		int nSendLen;		// Ҫ���͵��ֽ���
		while(nLeft > 0)
		{
			// ����һ���꣬�������ʧ�ܣ�����5��
			nSendLen = nLeft > FRAME_SIZE ? FRAME_SIZE : nLeft;
			n=0;
			while(!pSerial->SendFileData(&Buffer[nRead-nLeft], nSendLen) && n++<5)
			{
				if(!pSerial->IsOpen()) // ����Ƿ��û�������ֹ
				{
					AddLog(" �����ļ������û��رմ���! \r\n");
					return FALSE;
				}
				str.Format(" ֡�ط� %d �� \r\n", n);
				AddLog(str);

			}
			if(n>=5)
			{
				m_Status.SetWindowText(" �����ļ���������");
				return FALSE;
			}
			
			// ֡���ͳɹ�������״̬����
			nLeft -= nSendLen;
			dwTotalSendLen += nSendLen;

			// ���û���ʾ��ǰ�ļ����ͽ���
			str.Format(" �ѷ��ͣ�%d KB��ʣ�ࣺ%d KB \r\n", 
							dwTotalSendLen/1024, (nFileSize - dwTotalSendLen)/1024);
			m_Status.SetWindowText(str);
		}
	}
	
	///////////////////////////////////////////////////////
	// ���ͽ�����־
	BYTE command = SendEOF();
	if(command != 0)	// ����
	{
		switch(command) // ������ԭ��
		{
		case SERIAL_TRANSFAILED:
			{
				m_Status.SetWindowText(" �ļ�������ϣ������д��� \r\n");
				AddLog(" �ļ�������ϣ������д��� \r\n");
			}
			break;
		}
		return FALSE;
	}

	// �ļ�������ɣ����û���ʾ��ʱ
	DWORD dwEsp = ::GetTickCount() - dwTick; 
	str.Format(" �ļ����ͳɹ���ɡ�[%d KB] ��ʱ: %d ��", nFileSize/1000, dwEsp/1000);
	AddLog(str);

	return TRUE;
}

DWORD WINAPI ServerThreadProc(LPVOID lpParam)	// �����߳�
{
	// ��ߵ�ǰ�߳����ȼ�
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

	// ������Ȩ������������
	CComFileDlg *pDlg = (CComFileDlg *)lpParam;
	pDlg->AddLog("    �������߳�����... \r\n");
	pDlg->m_bNeedSend = FALSE;
	DWORD dw = pDlg->ServerProc();
	pDlg->AddLog("    �������߳��˳�... \r\n");

	pDlg->m_pComFile->SendInit();

	return dw;
}

DWORD CComFileDlg::ServerProc()
{
	CSerialFrame *pFrame;				// ָ֡��
	CSCSerial *m_pServer = m_pComFile;	// CSCSerial����ָ�루��Ϊ�������ã�

	// ״̬��ʼ��ΪIDLE
	m_pServer->m_nStatus = STATUS_IDLE;

	// ��ֹ�߳��˳�
	m_bThreadExit = FALSE;
	
	// ��������ѭ���������COM�ڽ��յ�������
	int n=0;		// ֡�ط�����
	while(TRUE)
	{
		// �ȴ����ݵĵ���
		while((pFrame = m_pServer->ReadFrame()) == NULL)
		{
			if(m_bThreadExit || !m_pComFile->IsOpen())
			{
				AddLog(" �û������߳��˳� \r\n");
				return 0;
			}

			// ����Ƿ���Ҫ�����ļ�
			if(m_bNeedSend)
			{
				AddLog(" ����׼�������ļ�... \r\n");
				m_pServer->m_nStatus = STATUS_IDLE;
				if(Connect())
				{
					AddLog(" ���ӳɹ� \r\n");
					if(!SendFile(m_SendFile, m_pServer))
					{
						AddLog(" �����ļ�ʧ��\r\n");
						m_Status.SetWindowText(" �����ļ�ʧ��\r\n");
					}
					else
					{
						AddLog(" �����ļ��ɹ� \r\n");
						m_Status.SetWindowText(" �����ļ��ɹ� \r\n");
					}
				}
				m_pServer->m_nStatus = STATUS_IDLE;
				m_bNeedSend = FALSE;
				if(m_pComFile->IsOpen())
					m_Send.EnableWindow();
			}
		}
		
		// ������С
		if(pFrame->uDataLen > FRAME_SIZE)
		{
			AddLog(" CServerDlg::ThreadProc() ֡�������\r\n");
			m_pComFile->Purge();
			continue;
		}
		
		// ���������ֽ�
		if(pFrame->uDataLen > 0)
		{
			if(!m_pComFile->ReadData(pFrame + 1, pFrame->uDataLen, 500))
			{
				if(m_bThreadExit || !m_pComFile->IsOpen())
				{
					AddLog(" �û������߳��˳� \r\n");
					return 0;
				}
				m_pComFile->Purge();
				AddLog(" CServerDlg::ThreadProc()  ��ȡ���ݳ�ʱ \r\n");
				continue;  
			}
		}
		
		// ���Ч��λ
		int nChecksum = pFrame->uChecksum;
		pFrame->uChecksum = 0;
		if(nChecksum != checksum((USHORT*)pFrame, sizeof(CSerialFrame) + pFrame->uDataLen))
		{
			AddLog(" CServerDlg::ThreadProc()  ����Ч�����  �������´���\r\n");
			
			// �����ط�
			m_pServer->SendRetry();
			continue;
		}
		
		///////////////////////////////////////////////
		// ��������

		switch(pFrame->command)
		{
		case SERIAL_CONNECT:		// ���ӹ���
			{
				if(m_pServer->m_nStatus == STATUS_IDLE || m_pServer->m_nStatus == STATUS_CONNECTED)
				{
					m_pServer->SendAck();
					m_pServer->m_sequence = 0;
					m_pServer->m_nStatus = STATUS_CONNECTED;
					
					AddLog(" ���յ������ӣ���׼�������ļ�... \r\n");
					m_Status.SetWindowText(" ���յ������ӣ���׼�������ļ�... \r\n");
				}
				else
				{
					m_pServer->SendInit();
					m_pServer->m_nStatus = STATUS_IDLE;
					AddLog(" ���յ��Ƿ����ӣ�Ҫ���ʼ�� \r\n");
				}
			}
			break;
		case SERIAL_INIT:		// �Է��������³�ʼ��
			{
				AddLog(" �Է�Ҫ�����³�ʼ�� \r\n ");
				m_pServer->SendAck();
				m_pServer->m_nStatus = STATUS_IDLE;
			}
			break;
		case SERIAL_SIZE:	// �Է������ļ���С��Ϣ�����յ��������Ҫ�����ļ�����
			{
				if(m_pServer->m_nStatus == STATUS_CONNECTED)
				{
					m_dwFileLen = *(DWORD*)(pFrame+1);
					if(m_dwFileLen > 200*1024*1024)
					{
						m_pServer->SendCommand(SERIAL_TOOBIG);
						AddLog(" �ļ�̫�󣬾ܾ����գ�ӦС��200MB�� \r\n");
						continue;
					}
					
					// Ϊ�ļ����������ڴ�ռ�
					m_pData = new BYTE[m_dwFileLen];
					if(m_pData == NULL)
					{
						m_pServer->SendCommand(SERIAL_SYSFAILED);
						AddLog("  �ڴ治�㣡����ʧ�� \r\n");
						continue;
					}
					m_pServer->SendAck();
					m_dwRecvLen = 0;
					m_pServer->m_nStatus = STATUS_TRANSFERING;
				}
				else
				{
					m_pServer->SendInit();
					AddLog(" ���յ��Ƿ����Ҫ���ʼ�� \r\n");	
				}
			}
			break;
		case SERIAL_DATA:   // ������
			{
				if(m_pServer->m_nStatus != STATUS_TRANSFERING)
				{
					m_pServer->SendInit();
					AddLog(" ���յ��Ƿ����Ҫ���ʼ�� \r\n");	
					continue;
				}

				// ��λʵ������
				char *psz = (char*)(pFrame+1);
				if(pFrame->sequence == m_pServer->m_sequence)
				{
					m_pServer->m_sequence++;
					if(m_pData == NULL)  // ���ָ���Ƿ���Ч
					{
						AddLog(" �������⣬û��׼���ô��� \r\n");
						m_pServer->SendInit();
						continue;
					}
					if(!m_pServer->SendAck())
					{
						m_pServer->SendInit();
						m_Status.SetWindowText(" �û��رմ��ڣ�����Ƿ���ֹ!! ");

						AddLog(" �û��رմ��ڣ�����Ƿ���ֹ!! ");
						
						delete[] m_pData;
						m_pData = NULL;
						m_dwRecvLen = 0;
						m_dwFileLen = 0;
						continue;
					}

					// ��������
					memcpy(&m_pData[m_dwRecvLen], pFrame+1, pFrame->uDataLen);
					m_dwRecvLen += pFrame->uDataLen;

					// ��ʾ�ļ��������
					CString str;
					str.Format("�Ѿ����� ��%d KB����ʣ�ࡾ%d KB��", 
									m_dwRecvLen/1024, (m_dwFileLen-m_dwRecvLen)/1024);
					m_Status.SetWindowText(str);
				}
				else if(pFrame->sequence < m_pServer->m_sequence) // �ͻ��ط����꣬����
				{
					m_pServer->SendAck();
					AddLog(" �Է��ȴ�ʱ����̣��յ��ط��� \r\n");
					continue;
				}
				else  // ���кŴ���
				{
					AddLog(" �������кų��� Ҫ�����³�ʼ�� \r\n");
					// ���³�ʼ��
					m_pServer->SendInit();
					continue;
				}
			}
			break;
			
		case SERIAL_EOF:	// �����ļ���������ʼ��ӡ
			{
				if(m_pServer->m_nStatus == STATUS_TRANSFERING)
				{
					if(m_dwRecvLen == m_dwFileLen)
					{
						m_pServer->SendAck();

						// ������յ����ļ�����
						OnRecvCmp(m_pData, m_dwFileLen);

						delete m_pData;
						m_pData = NULL;
						m_dwFileLen = 0;
							
						AddLog(" �ļ�����ɹ���ɣ�\r\n");	
					}
					else
					{			
						// ����Է����´���
						m_pServer->SendCommand(SERIAL_TRANSFAILED);
						m_Status.SetWindowText(" �ļ�������ɣ������д���\r\n");
						AddLog(" �ļ��������\r\n");
					}
					m_pServer->m_nStatus = STATUS_IDLE;
				}
				else
				{
					AddLog(" ���յ��Ƿ����Ҫ���ʼ�� \r\n");
					m_pServer->SendInit();	
				}
			}
			break;
		default:
			m_Status.SetWindowText(" ���յ��Ƿ����� \r\n");
			AddLog(" ���յ��Ƿ����� \r\n");
			m_pServer->m_nStatus = STATUS_IDLE;
			break;;
		}
	}
	return 0;
}

BOOL CComFileDlg::Connect()
{
BYTE command;  // ����
	do
	{
		// ������������
		command = SendConnect();
		if(command != 0)		// ����ʧ��
		{
			switch(command)		// �鿴ʧ��ԭ��
			{
			case SERIAL_INIT:			// ��Ҫ��ʼ��������������
				{
					continue;
				}
				break;
			case SERIAL_VERIFYFAILED:	// �û���֤ʧ��
				{
					MessageBox(" �û���֤ʧ�ܣ������������\r\n");
					AddLog(" �û���֤ʧ�ܣ������������\r\n");
				}
				break;
			default:
				{
					MessageBox(" ���ӷ�����ʧ�ܣ��Է�û����Ӧ��\r\n ");
					AddLog(" ���ӷ�����ʧ�ܣ��Է�û����Ӧ��\r\n ");
				}
				break;
			}
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	while(TRUE);

	// �������е�����
	return FALSE;
}

void CComFileDlg::OnRecvCmp(PBYTE pData, DWORD dwFileLen)	// �ļ��������
{
	CFileDialog dlg(FALSE); 
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	// �����ݱ��浽�ļ�
	CFile file;
	if(!file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite))
	{
		m_Status.SetWindowText(" �����ļ�ʧ�ܣ�");
		return;
	}
	file.WriteHuge(pData, dwFileLen);
	file.Close();
}
