// TranslucentDialog.h : ͷ�ļ�
//

#pragma once

#include "ImgWnd.h"
#include "TranslucentWnd.h"
#include <list>



typedef std::list<CTranslucentWnd*> TranslucentWndList; 

// CTranslucentDialog �Ի���
class CTranslucentDialog : public CDialog, public IRenderListener
{
// ����
public:
	CTranslucentDialog(UINT nID, const wchar_t* wszFile, CImgWnd* pImgWnd, CWnd* pParent=NULL);
	virtual ~CTranslucentDialog();

	void	SetEnableDrag(bool bEnableDrag);
	void	SetCenterAligned(bool bCenterAligned);

	void RegisterTranslucentWnd(CTranslucentWnd* translucentWnd);
	void UnregisterTranslucentWnd(CTranslucentWnd* translucentWnd);
	
	virtual void OnRenderEvent(CTranslucentWnd* translucentWnd);

	void UpdateView();

// ʵ��
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	Gdiplus::Image*		m_pImage;
	BLENDFUNCTION		m_BlendInfo;
	bool				m_bEnableDrag;
	bool				m_bCenterAligned;

	CImgWnd*			m_pImgWnd;
	TranslucentWndList	m_translucentWndList;
};
