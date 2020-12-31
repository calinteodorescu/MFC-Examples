
// MetroUIDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MetroUIDemo.h"
#include "MetroUIDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDl g �Ի���             
  
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


// CMetroUIDemoDlg �Ի���




CMetroUIDemoDlg::CMetroUIDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMetroUIDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMetroUIDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMetroUIDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMetroUIDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMetroUIDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMetroUIDemoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMetroUIDemoDlg ��Ϣ�������
#include <uxtheme.h>
#pragma comment(lib,"uxtheme.lib")
BOOL CMetroUIDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�  

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ ������   
	int a = GetSystemMetrics( SM_CYCAPTION );
	int b = GetSystemMetrics( SM_CXSIZEFRAME );
	int c = GetSystemMetrics( SM_CYSIZEFRAME );
	int d = GetSystemMetrics( SM_CXFIXEDFRAME );
	int e = GetSystemMetrics( SM_CYFIXEDFRAME );

	GetDlgItem( IDC_STATIC1 )->ModifyStyle( SS_LEFT,0 );

	CTabCtrl * pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB1);
	pTabCtrl->InsertItem( 0,_T("����ʡ") );
	pTabCtrl->InsertItem( 1,_T("����ʡ") );
	pTabCtrl->InsertItem( 2,_T("����ʡ") );

	//EnableThemeDialogTexture(m_hWnd,ETDT_ENABLE | ETDT_USETABTEXTURE);

	HBRUSH hBrush = ::CreateSolidBrush( 0x0000ff );
	int ret = SetClassLong( GetDlgItem(IDC_STATIC1)->m_hWnd,GCL_HBRBACKGROUND,(LONG)hBrush);
	GetDlgItem(IDC_STATIC1)->Invalidate();
	hBrush = (HBRUSH)GetClassLong( GetDlgItem(IDC_STATIC1)->m_hWnd,GCL_HBRBACKGROUND );
	DWORD error =GetLastError();

 
	return TRUE;  // ���ǽ��������õ��ؼ������� �� TRUE    
}

void CMetroUIDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMetroUIDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMetroUIDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMetroUIDemoDlg::OnBnClickedButton1()
{
	CFileDialog dlg( TRUE,0,0,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("�����ļ� (*.*)|*.*||"),this );
	if( dlg.DoModal() == IDOK )
	{
		CString strFile = dlg.GetPathName();

		CFile file( strFile,CFile::modeRead );
		unsigned char buf[1];
		int len = file.GetLength();
		int i = 0;
		CString str,strText;
		while( i < len )
		{
			file.Read( buf,1 );
			i++;
			str.Format( _T("0x%02x,"),buf[0] );
			strText += str;
			if( i % 100 == 0 )
				strText += _T("\r\n");
		}
		SetDlgItemText( IDC_EDIT1,strText );
	}

}

void CMetroUIDemoDlg::OnBnClickedButton2()
{
	MessageBox( _T("��Һã�����ϵͳȷ�Ͽ�MessageBox���á�\r\n\r\n�ҸմӺ���������ϣ����һ��ϵ��ҡ�"),_T("��ʾ"),MB_YESNOCANCEL | MB_ICONINFORMATION );  
} 

void CMetroUIDemoDlg::OnBnClickedButton3()
{
	BROWSEINFO   bf;  

    LPITEMIDLIST   lpitem;  

    memset(&bf,0,sizeof   BROWSEINFO);  

    bf.hwndOwner=m_hWnd;  

    bf.lpszTitle=_T("ѡ��·��");  

    bf.ulFlags=BIF_RETURNONLYFSDIRS;     //��������Լ�ѡ��  

    lpitem=SHBrowseForFolder(&bf);  

    if(lpitem==NULL)     //���û��ѡ��·���򷵻�   0   

        return  ;  


    //���ѡ����·������·��,����·������   

	TCHAR buf[MAX_PATH] = { 0 };

    SHGetPathFromIDList(lpitem,buf); 

}
       