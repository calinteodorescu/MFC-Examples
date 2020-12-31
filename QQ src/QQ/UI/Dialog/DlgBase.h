

#ifndef __DLG_MY_BASE_X_H__
#define __DLG_MY_BASE_X_H__
#include <vector>
#include "..\Base\ControlBase.h"
#include ".\WndPopup.h"


using namespace  std;
// �����ӶԻ���
class  CDlgBase : public CDialog
{
	DECLARE_DYNAMIC(CDlgBase)

public:

	CDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CDlgBase();

public:

	void SetMinSize(int iWidth, int iHeight);	// ������С���ڴ�С
	CSize GetMinSize();

protected:

	CSize			m_MinSize;					// �����޶���С��С
	CRgn			m_Rgn;						// �����򴰿�����
	COLORREF		m_clrBK;					// �Զ���ǰ����ɫ

	CFont			m_TitleFont;				// ���Ʊ�����������
	HICON			m_hIcon;		

	CBitmap			m_BKImage;					// ��ܱ���ͼƬ
	CSize			m_sizeBKImage;
	CDC				m_MemBKDC;					//����dc
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;


	int				m_nFrameTopBottomSpace;
	int				m_nFrameLeftRightSpace;

	int				m_nOverRegio;				//���ȵĴ�С
	bool			m_bNCActive;

	bool			m_bTracking;
	bool			m_bIsLButtonDown;
	CRect			m_rcLButtonDown;
	bool			m_bIsLButtonDblClk;
	bool			m_bIsSetCapture;

	CPoint			m_pt;
	vector<CControlBase *>	m_vecControl;
	vector<CControlBase *>	m_vecBaseControl;
	vector<CControlBase *>	m_vecArea;
	vector<CControlBase *>	m_vecBaseArea;
	CControlBase *m_pControl;
	CControlBase *m_pFocusControl;
	CControlBase *m_pEditControl;
	CEdit			*m_pEdit;

	CWndPopup		*m_pWndPopup;
		


	CString m_strPath;
protected:

	// ���ò�����������
	void SetupRegion(int border_offset[], int nSize);
	void DrawImageStyle(CDC &dc, const CRect &rcClient);
	
	// ����ͼƬ
	void LoadImage(UINT nIDResource);
	void LoadImage(CString strFileName);
	// ��ʼ�����ڿؼ���Ĭ��ֵ
	void InitUIState();
	// ���ò�����������
	void SetupRegion(int nSize);
	// ������ͼƬ
	void DrawBackground(CBitmap &bitBackground);
	// ǰ��ͼƬ
	virtual void DrawControl(CDC &dc, const CRect &rcClient);	

	void InitBaseUI(CRect rcClient);
	virtual void InitUI(CRect rcClient) = 0;
	virtual void OnSize(CRect rcClient) = 0;
	CControlBase *GetControl(UINT uControlID);

	// �򿪵����Ի���
	void OpenDlgPopup(CWndPopup *pWndPopup, CRect rc, UINT uMessageID);
	// �رյ����Ի���
	void CloseDlgPopup();

	// ���ػ��ƴ�������	
	virtual void OnBaseSize(CRect rcFrame);
	virtual void OnClose();
	virtual void OnMinimize();
	virtual BOOL OnMaximize();

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);


	afx_msg LRESULT OnMessageButtomMin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageButtomClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessagePopupClose(WPARAM wParam, LPARAM lParam);
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif __DLG_MY_BASE_X_H__

