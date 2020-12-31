// HookFirewallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "main.h"
#include "HookDriver.h"
#include "HookFirewallDlg.h"
#include <winsock2.h>
#include <winioctl.h>
#include "shlwapi.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHookFirewallDlg dialog


IMPLEMENT_DYNCREATE(CHookFirewallDlg, CRootDlg)

CHookFirewallDlg::CHookFirewallDlg(CWnd* pParent /*=NULL*/)
	: CRootDlg(CHookFirewallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHookFirewallDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bStarted = false;
}


void CHookFirewallDlg::DoDataExchange(CDataExchange* pDX)
{
	CRootDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHookFirewallDlg)
	DDX_Control(pDX, IDC_LIST_RULES, m_lstRules);
	DDX_Control(pDX, IDC_BTN_MENU, m_btnMenu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHookFirewallDlg, CRootDlg)
	//{{AFX_MSG_MAP(CHookFirewallDlg)
	ON_BN_CLICKED(IDC_BTN_MENU, OnBtnMenu)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHookFirewallDlg message handlers

BOOL CHookFirewallDlg::OnInitDialog() 
{
	CRootDlg::OnInitDialog();
	
	menu.LoadMenu (IDR_MENU6);
	m_btnMenu.m_hMenu = menu.GetSubMenu (0)->GetSafeHmenu ();
	m_btnMenu.SizeToContent ();
	
	RECT rc;
	m_lstRules.GetClientRect(&rc);

	int nWidth = rc.right - rc.left - 110;
	int nLength = nWidth/6;

	m_lstRules.InsertColumn(0, "ԴIP", LVCFMT_LEFT, nLength, 0);
	m_lstRules.InsertColumn(1, "Դ����λ", LVCFMT_LEFT, nLength, 1);
	m_lstRules.InsertColumn(2, "Դ�˿ں�", LVCFMT_LEFT, nLength, 2);
	m_lstRules.InsertColumn(3, "Ŀ��IP", LVCFMT_LEFT, nLength, 3);
	m_lstRules.InsertColumn(4, "Ŀ������λ", LVCFMT_LEFT, nLength, 4);
	m_lstRules.InsertColumn(5, "Ŀ�Ķ˿ں�", LVCFMT_LEFT, nLength, 5);
	m_lstRules.InsertColumn(6, "Э��", LVCFMT_LEFT, 60, 6);
	m_lstRules.InsertColumn(7, "����", LVCFMT_LEFT, 50, 7);

	m_lstRules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// ����IP���˹�������
	char szPath[MAX_PATH];
	char* p;
	ZeroMemory(szPath, MAX_PATH);
    GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	sprintf(szPath, "%s%s", szPath, "\\MyHookDrv.sys");
	char sz[MAX_PATH];
	GetFullPathName("MyHookDrv.sys", MAX_PATH, sz, &p);
	HMODULE hDll = LoadLibrary(szPath);
	m_pDriver = new CHookDriver(szPath, (char*)("MyHookDrv"));
	if(!m_pDriver->StartDriver() || !m_pDriver->OpenDevice())
		MessageBox("��������ʧ�ܣ�");
	m_bStarted = FALSE;
	return TRUE;  
}

void CHookFirewallDlg::OnBtnMenu() 
{
	UpdateData();

	switch (m_btnMenu.m_nMenuResult)
	{
	case IDM_SAVE_RULES:
		SaveRulesToFile();
		break;
	case IDM_IMPORT_RULES:
		ImportRules();
		break;
	case IDM_START_HOOK:
		StartHook();
		break;
	case IDM_STOP_HOOK:
		StopHook();
		break;	
	case IDM_CLEAR_RULES:
		m_pDriver->IoControl(CLEAR_FILTER, NULL, 0, NULL, 0);
		break;
	case IDM_ADD_RULE:
		AddRule();
		break;
	case IDM_INSTALL_RULES:
		InstallRules();
		break;
	case IDM_DEL_RULE:
		DeleteRule();
		break;
	}
}

void CHookFirewallDlg::SaveRulesToFile()
{
    if (m_aRules.size() == 0)
	{
		AfxMessageBox("û�й���", MB_OK, MB_ICONERROR);
		return;
	}
	CFileDialog dlg(FALSE, "rul", NULL, OFN_HIDEREADONLY|OFN_CREATEPROMPT, "�����ļ�(*.rul)|*.rul||", this);
    if (dlg.DoModal() == IDCANCEL)
        return;
	CFile file;
	if (!file.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite))
		AfxMessageBox("���������ļ�ʧ��", MB_OK, MB_ICONERROR);
	else
	{
		for(int i=0; i<m_aRules.size(); i++)
			file.Write(&m_aRules[i], sizeof(IPFilterRule));
        file.Close();
	}
}

void CHookFirewallDlg::ImportRules()
{
   // �����򿪶Ի���
	CFileDialog dlg(TRUE, "rul", NULL, OFN_HIDEREADONLY, "�����ļ�(*.rul)|*.rul||", this);
	if (dlg.DoModal() == IDCANCEL)
		return;
	// ��ȡ���˹���
	CFile file;
	if(file.Open(dlg.GetPathName(), CFile::modeRead))
	{	
		ClearRules();
		IPFilterRule rule;
		do
		{
			if(file.Read(&rule, sizeof(rule)) == 0)
				break;
			if(!AddRule(rule.sourceIP, rule.sourceMask, rule.sourcePort, rule.destIP, 
					   rule.destMask, rule.destPort, rule.protocol, rule.bDrop))
			{
				AfxMessageBox("��ӹ������");
				break;
			}
		}while(1);
		
		file.Close();
		UpdateRuleList();
	}
	else
		AfxMessageBox("���ļ�����");
}

void CHookFirewallDlg::UpdateRuleList()
{
	m_lstRules.DeleteAllItems();
    char szTemp[6];
	int nCurrent;
	
	for(int i=0; i<m_aRules.size(); i++)
	{
		IPFilterRule& rule = m_aRules[i];
		nCurrent = i;
		// ԴIP��ַ
		ULONG ip = ntohl(rule.sourceIP);
		CString s = (ip == 0) ? "����" : ::inet_ntoa(*((in_addr*)&ip));
		m_lstRules.InsertItem(nCurrent, s, 0);
		// ԴIPλ����
		ip = ntohl(rule.sourceMask);
		s = (ip == 0) ? "����" : ::inet_ntoa(*((in_addr*)&ip));
		m_lstRules.SetItemText(nCurrent, 1, s);
		// Դ�˿ں�
		s = (rule.sourcePort == 0) ? "����" : ::itoa(rule.sourcePort, szTemp, 10);
		m_lstRules.SetItemText(nCurrent, 2, s);
		// Ŀ��IP��ַ
		ip = ntohl(rule.destIP);
		s = (ip == 0) ? "����" : ::inet_ntoa(*((in_addr*)&ip));
		m_lstRules.SetItemText(nCurrent, 3, s);
		// Ŀ��IPλ����
		ip = ntohl(rule.destMask);
		s = (ip == 0) ? "����" : ::inet_ntoa(*((in_addr*)&ip));
		m_lstRules.SetItemText(nCurrent, 4, s);
		// Ŀ�Ķ˿ں�
		s = (rule.destPort == 0) ? "����" : ::itoa(rule.destPort, szTemp, 10);
		m_lstRules.SetItemText(nCurrent, 5, s);
		// Э��
		if(rule.protocol == 1)
			m_lstRules.SetItemText(nCurrent, 6, "ICMP");
		else if(rule.protocol == 6)
			m_lstRules.SetItemText(nCurrent, 6, "TCP");
		else if(rule.protocol == 17)
			m_lstRules.SetItemText(nCurrent, 6, "UDP");
		else
			m_lstRules.SetItemText(nCurrent, 6, "����");
		// ����
		s = rule.bDrop ? "����" : "ͨ��";
		m_lstRules.SetItemText(nCurrent, 7, s);
	}
}

void CHookFirewallDlg::ClearRules()
{
    m_aRules.clear();
	UpdateRuleList();
}

bool CHookFirewallDlg::AddRule(unsigned long sourceIP, unsigned long sourceMask, unsigned short sourcePort, unsigned long destIP, unsigned long destMask, unsigned short destPort, int protocol, BOOL bDrop)
{
	if (m_aRules.size() > MAX_RULE_NUM)
		return false;
    IPFilterRule rule;
	rule.sourceIP	  = htonl(sourceIP);
	rule.sourceMask	  = htonl(sourceMask);
	rule.sourcePort	  = sourcePort;
	rule.destIP   = htonl(destIP);
	rule.destMask = htonl(destMask);
	rule.destPort = destPort;
	rule.protocol	  = protocol;
	rule.bDrop 	  = bDrop;
	m_aRules.push_back(rule);
	return true;
}

void CHookFirewallDlg::OnDestroy() 
{
	CRootDlg::OnDestroy();
	if (m_bStarted)
		m_pDriver->IoControl(STOP_IP_HOOK, NULL, 0, NULL, 0);
	if (m_pDriver)
		delete m_pDriver;
}

void CHookFirewallDlg::StartHook()
{
	if (m_bStarted)
		return;
	if (m_pDriver->IoControl(START_IP_HOOK, NULL, 0, NULL, 0) == -1)
		MessageBox("��������ʧ��");
	else
		m_bStarted = TRUE;
}

void CHookFirewallDlg::StopHook()
{
   if (!m_bStarted)
		return;
	if (m_pDriver->IoControl(STOP_IP_HOOK, NULL, 0, NULL, 0) == -1)
		MessageBox("��������ʧ��");
	else
		m_bStarted = TRUE;
}

void CHookFirewallDlg::AddRule()
{
    if(m_aRules.size() >= MAX_RULE_NUM)
	{
		MessageBox(" ��������ӹ���");
		return;
	}

	CRuleDlg dlg;
	// ������ӹ���Ի���
	if(dlg.DoModal() == IDOK)
	{
		// ���ĵ�����ӹ���
		BOOL bOK = AddRule(dlg.m_srcIP, dlg.m_srcMask, dlg.m_srcPort, 
				           dlg.m_destIP, dlg.m_destMask, dlg.m_destPort, dlg.m_nProtocol, dlg.m_bDrop);
		if(bOK)
		    UpdateRuleList();
		else
			MessageBox("��ӹ������");
	}
}

void CHookFirewallDlg::InstallRules()
{
    DWORD nRet;
	if (m_aRules.size() == 0)
	{
		AfxMessageBox("û�й��˹���", MB_OK, MB_ICONERROR);
		return;
	}
    m_pDriver->IoControl(CLEAR_FILTER, NULL, 0, NULL, 0);
	for(int i=0; i<m_aRules.size(); i++)
	{
		IPFilterRule& rule = m_aRules[i];
		nRet = m_pDriver->IoControl(ADD_FILTER, (PVOID)&rule, sizeof(rule), NULL, 0);
		if (nRet == -1)
		{
			AfxMessageBox("��ӹ��˹���ʧ��");
			break;
		}
	}
}

void CHookFirewallDlg::DeleteRule()
{
	POSITION pos = m_lstRules.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("��ѡ��һ�����˹���");
		return;
	}

    int i = m_lstRules.GetNextSelectedItem(pos);
	m_aRules.erase(m_aRules.begin() + i);
	UpdateRuleList();
}

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg dialog


CRuleDlg::CRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleDlg)
	m_strSourceIP = _T("0.0.0.0");
	m_strDestIP = _T("0.0.0.0");
	m_strDestMask = _T("255.255.255.255");
	m_strSourceMask = _T("255.255.255.255");
	m_nDestPort = 0;
	m_nSourcePort = 0;
	//}}AFX_DATA_INIT
}


void CRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleDlg)
	DDX_Control(pDX, IDC_PROTOCOL, m_cmbProtocol);
	DDX_Control(pDX, IDC_ACTION, m_cmbAction);
	DDX_Text(pDX, IDC_SOURCE_IP, m_strSourceIP);
	DDX_Text(pDX, IDC_DEST_IP, m_strDestIP);
	DDX_Text(pDX, IDC_DEST_MASK, m_strDestMask);
	DDX_Text(pDX, IDC_SOURCE_MASK, m_strSourceMask);
	DDX_Text(pDX, IDC_DEST_PORT, m_nDestPort);
	DDX_Text(pDX, IDC_SOURCE_PORT, m_nSourcePort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuleDlg, CDialog)
	//{{AFX_MSG_MAP(CRuleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleDlg message handlers

BOOL CRuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// ��ʼ���Ӵ��ڿؼ�
	m_cmbProtocol.InsertString(0, "����");
	m_cmbProtocol.InsertString(1, "TCP");
	m_cmbProtocol.InsertString(2, "UDP");
	m_cmbProtocol.InsertString(3, "ICMP");
	m_cmbProtocol.SetCurSel(0);

	m_cmbAction.InsertString(0, "����");
	m_cmbAction.InsertString(1, "����");
	m_cmbAction.SetCurSel(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRuleDlg::OnOK() 
{	
	UpdateData(TRUE);

	// ԴIP��ַ
	m_srcIP = ::inet_addr(m_strSourceIP);
	if(m_srcIP == INADDR_NONE)
	{
		MessageBox("ԴIP��ַ����");
		return;
	}
	// Դ������
	m_srcMask = ::inet_addr(m_strSourceMask);
	// Ŀ��IP��ַ
	m_destIP = ::inet_addr(m_strDestIP);
	if(m_destIP == INADDR_NONE)
	{
		MessageBox("Ŀ��IP��ַ����");
		return;
	}
	// Ŀ��������
	m_destMask = ::inet_addr(m_strDestMask);
	// Э��
	CString s;
	m_cmbProtocol.GetWindowText(s);
	if(s == "TCP")
	{
		m_nProtocol = 6;
	}
	else if(s == "UDP")
	{
		m_nProtocol = 17;
	}
	else if( s == "ICMP")
	{
		m_nProtocol = 1;
	}
	else
	{
		m_nProtocol = 0;
	}
	// �ж�
	m_cmbAction.GetWindowText(s);
	if(s == "����")
		m_bDrop = TRUE;
	else
		m_bDrop = FALSE;

	// �����˿ں�
	m_srcPort = m_nSourcePort;
	m_destPort = m_nDestPort;

	CDialog::OnOK();
}
