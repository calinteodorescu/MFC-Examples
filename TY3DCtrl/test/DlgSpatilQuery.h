#pragma once
#include "afxwin.h"


// CDlgSpatilQuery �Ի���
#include "mxobject1.h"
class CDlgSpatilQuery : public CDialog
{
	DECLARE_DYNAMIC(CDlgSpatilQuery)

public:
	CDlgSpatilQuery(IMXObjectPtr& pMap, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSpatilQuery();

// �Ի�������
	enum { IDD = IDD_DIALOG_SPATIALQUERY };
    IMXObjectPtr m_pMap;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbSrcSet;//��ѯ���ݼ�
	CComboBox m_cmbOpSet;//�������ݼ�
	CComboBox m_cmbOpType;//��ѯ��ʽ
	double m_dis;//����
	CComboBox m_cmbUnit;//��λ
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboOpset();
	afx_msg void OnCbnSelchangeComboOptype();
};
