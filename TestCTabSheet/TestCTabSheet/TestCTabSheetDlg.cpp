// TestCTabSheetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestCTabSheet.h"
#include "TestCTabSheetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestCTabSheetDlg �Ի���




CTestCTabSheetDlg::CTestCTabSheetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestCTabSheetDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestCTabSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabSheet);
}

BEGIN_MESSAGE_MAP(CTestCTabSheetDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestCTabSheetDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestCTabSheetDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO1, &CTestCTabSheetDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTestCTabSheetDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CTestCTabSheetDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CTestCTabSheetDlg::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CTestCTabSheetDlg ��Ϣ�������

BOOL CTestCTabSheetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//ΪTabCtrl�ؼ����ͼ��
	m_imageList.Create(16, 16, ILC_COLOR32, 1, 1);
	CBitmap bitmap1,bitmap2;
	bitmap1.LoadBitmap(IDB_BITMAP1);
	bitmap2.LoadBitmap(IDB_BITMAP2);
	m_imageList.Add(&bitmap1, RGB(0,0,0));
	m_imageList.Add(&bitmap2, RGB(0,0,0));
	m_TabSheet.SetImageList(&m_imageList);
	
	//��TabCtrl���ҳ
	m_TabSheet.AddPage(_T("Page1"), &m_Page1, m_Page1.IDD, 0);
	m_TabSheet.AddPage(_T("�ڶ�ҳ"), &m_Page2, m_Page2.IDD, 1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestCTabSheetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestCTabSheetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CTestCTabSheetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestCTabSheetDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TabSheet.EnableTab(1, !m_TabSheet.IsTabEnabled(1));
}

void CTestCTabSheetDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TabSheet.HideTab(!m_TabSheet.IsTabHided());
}

void CTestCTabSheetDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TabSheet.SetItemPos(m_TabSheet.TOP);
}

void CTestCTabSheetDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TabSheet.SetItemPos(m_TabSheet.LEFT);
}

void CTestCTabSheetDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TabSheet.SetItemPos(m_TabSheet.RIGHT);
}

void CTestCTabSheetDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TabSheet.SetItemPos(m_TabSheet.BOTTOM);
}
