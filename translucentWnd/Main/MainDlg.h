
// MainDlg.h : ͷ�ļ�
//

#pragma once

#include "ImgWnd.h"



// CMainDlg �Ի���
class CMainDlg : public CImgWnd
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DLG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();

// ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
};
