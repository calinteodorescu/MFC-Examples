#include "stdafx.h"
#include "CCtl.h"
#include "CCtlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCCtlApp, CWinApp)
	//{{AFX_MSG_MAP(CCCtlApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CCCtlApp::CCCtlApp()
{
}

CCCtlApp theApp;

BOOL CCCtlApp::InitInstance()
{
#ifdef _AFXDLL
#else
	Enable3dControlsStatic();
#endif

	CCCtlDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
