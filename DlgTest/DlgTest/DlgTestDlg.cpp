
// DlgTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgTest.h"
#include "DlgTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDlgTestDlg �Ի���




CDlgTestDlg::CDlgTestDlg(CWnd* pParent /*=NULL*/)
	: CDrawDlg(CDlgTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDrawDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTestDlg, CDrawDlg)
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgTestDlg ��Ϣ�������

BOOL CDlgTestDlg::OnInitDialog()
{
	CDrawDlg::OnInitDialog();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
