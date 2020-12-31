// DlgInputDB.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mapdemo.h"
#include "DlgNetDB.h"

// CDlgNetDB �Ի���

IMPLEMENT_DYNAMIC(CDlgNetDB, CDialog)

CDlgNetDB::CDlgNetDB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNetDB::IDD, pParent)
	, m_strServer(_T("192.168.1.1"))
	, m_strDB(_T("postgis"))
	, m_strUser(_T("postgres"))
	, m_strPwd(_T("123456"))
	, m_strPort(_T("5432"))
{

}

CDlgNetDB::~CDlgNetDB()
{
}

void CDlgNetDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strServer);
	DDX_Text(pDX, IDC_EDIT2, m_strDB);
	DDX_Text(pDX, IDC_EDIT3, m_strUser);
	DDX_Text(pDX, IDC_EDIT4, m_strPwd);
	DDX_Text(pDX, IDC_EDIT5, m_strPort);
    DDX_Text(pDX, IDC_EDIT6, m_strTable);
	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbType);
	DDX_CBString(pDX, IDC_CMB_TYPE, m_strServType);
}


BEGIN_MESSAGE_MAP(CDlgNetDB, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNetDB::OnBnClickedOk)
 
	ON_CBN_SELCHANGE(IDC_CMB_TYPE, &CDlgNetDB::OnCbnSelchangeCmbType)
END_MESSAGE_MAP()


// CDlgNetDB ��Ϣ�������

void CDlgNetDB::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

BOOL CDlgNetDB::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_cmbType.AddString(_T("POSTGIS"));
	m_cmbType.AddString(_T("SQLServer"));
    m_cmbType.AddString(_T("Oracle"));
	m_cmbType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgNetDB::OnBnClickedTest()
{
 
}

void CDlgNetDB::OnCbnSelchangeCmbType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_cmbType.GetCurSel() == 0)
	{
		SetDlgItemText(IDC_EDIT3,_T("postgres"));
		SetDlgItemText(IDC_EDIT5,_T("5432"));
	}
	else if(m_cmbType.GetCurSel() == 1)
	{
		SetDlgItemText(IDC_EDIT3,_T("sa"));
		SetDlgItemText(IDC_EDIT5,_T("1433"));
	}
	else if (m_cmbType.GetCurSel() == 2)
	{
        SetDlgItemText(IDC_EDIT1,_T("orcl"));
        SetDlgItemText(IDC_EDIT2,_T(""));
        SetDlgItemText(IDC_EDIT5,_T(""));
	}
}
