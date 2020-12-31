#ifndef CDRAW_DLG_H
#define CDRAW_DLG_H

#define ID_DOWN_INVALID	-1
#define ID_DOWN_MAX		0
#define ID_DOWN_MIN		1
#define ID_DOWN_CLOSE	2

//��ť��λͼ��С
#define BMP_WIDTH	31
#define BMP_HEIGHT	23


class CDrawDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawDlg)
public:
	CDrawDlg(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CDrawDlg();

	DECLARE_MESSAGE_MAP()
protected:
/**���غ���**/
	virtual void PreSubclassWindow();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

/**�Զ��庯��**/

	// ��ȡ������
	void GetWindowStyle();
	// ������С������󻯡��رհ�ť��λ������
	void SetButtonRegion();
	// �жϵ�ǰ��������Ƿ�λ��ָ���ľ���������
	BOOL PtInRect(CRect &rtWin, CRect &rtClientBtn, CPoint &pt);


/**������Ϣ������**/

	// ���ڷ��ı�ʱ���͵���Ϣ
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg	void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg HRESULT OnNcHitTest(CPoint point);

private:
	// �Ƿ������С�����ť��ϵͳ�˵������������
	int m_MaxStyle;
	int m_MinStyle;
	int m_SysMenuStyle;
	int m_CaptionStyle;

	// ��󻯡���С�����رհ�ť
	CRect m_rtMaxBox;
	CRect m_rtMinBox;
	CRect m_rtCloseBox;

	// ��ǰ��µİ�ť
	int m_nIDBtn;
	// �������ĸ߶�
	int m_cyCaption;
	// �Ի���߿�ĸ߶ȺͿ��
	CPoint m_ptDlgFrame;

	// �Ի���ͼ��
	HICON m_hIcon;

	// ��󻯰�ť������״̬���������������״̬������
	CBitmap m_bmMaxHover;
	CBitmap m_bmMaxNormal;
	CBitmap m_bmMaxDown;

	// ��ԭ��ť������״̬���������������״̬������
	CBitmap m_bmRestoreHover;
	CBitmap m_bmRestoreNormal;
	CBitmap m_bmRestoreDown;

	// ��С����ť������״̬���������������״̬������
	CBitmap m_bmMinHover;
	CBitmap m_bmMinNormal;
	CBitmap m_bmMinDown;

	// �رհ�ť������״̬���������������״̬������
	CBitmap m_bmCloseHover;
	CBitmap m_bmCloseNormal;
	CBitmap m_bmCloseDown;
};


#endif //CDRAW_DLG_H