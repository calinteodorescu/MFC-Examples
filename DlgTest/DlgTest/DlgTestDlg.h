
// DlgTestDlg.h : ͷ�ļ�
//
#include "CDrawDlg.h"

#pragma once


// CDlgTestDlg �Ի���
class CDlgTestDlg : public CDrawDlg
{
// ����
public:
	CDlgTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DLGTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
