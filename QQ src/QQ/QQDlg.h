
// QQDlg.h : ͷ�ļ�
//

#pragma once
#include ".\UI\Dialog\DlgBase.h"

// CQQDlg �Ի���
class CQQDlg : public CDlgBase
{
// ����
public:
	CQQDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(CRect rcClient);

	afx_msg LRESULT OnMessageLoginList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageLoginMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageButtomAccount(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageButtomStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageCheckPassWord(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageCheckAutoLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageEditAccount(WPARAM wParam, LPARAM lParam);
};
