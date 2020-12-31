#pragma once



class CTranslucentDialog;

class CImgWnd : public CDialog
{
public:
	CImgWnd(const wchar_t* wszFile, UINT nID, CWnd* pParentWnd=NULL);
	virtual ~CImgWnd();

public:
	virtual INT_PTR	DoModal();

protected:
	virtual void	OnOK();
	virtual void	OnCancel();
	virtual BOOL	OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	void			DoCreate();

private:
	CTranslucentDialog*		m_pTranslucentWnd;	// ��͸������
	COLORREF				m_ColorKey;			// �㼶���������õ�͸��ɫ��ͨ����͸��ɫ��Ϊm_Brush�����οձ���
	CBrush					m_Brush;
};

