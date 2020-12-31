
#ifndef SKIN_LAYERED_WND_H_
#define SKIN_LAYERED_WND_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinImage.h"

/*
�÷�(�ԶԻ���Ϊ��):
����һ�� CSkinLayeredWnd m_SkinLayeredWnd;
����ON_WM_SHOWWINDOW �� ON_WM_WINDOWPOSCHANGED() ��2����Ϣ
BOOL OnInitDialog()
{
// �ֲ㴰��
m_SkinLayeredWnd.CreateLayered(...);
m_SkinLayeredWnd.InitLayeredArea(...);
}
VOID OnShowWindow(BOOL bShow, UINT nStatus)
{
__super::OnShowWindow(bShow, nStatus);

// ��ʾ�ֲ�
if (m_SkinLayeredWnd.m_hWnd != NULL)
{
m_SkinLayeredWnd.ShowWindow((bShow == FALSE) ? SW_HIDE : SW_SHOW);
}

return;
}
VOID CDlgStatus::OnWindowPosChanged(WINDOWPOS * lpWndPos)
{
__super::OnWindowPosChanging(lpWndPos);

// �ƶ��ֲ�
if ((m_SkinLayeredWnd.m_hWnd != NULL) && (lpWndPos->cx >= 0) && (lpWndPos->cy > 0))
{
m_SkinLayeredWnd.SetWindowPos(NULL, lpWndPos->x, lpWndPos->y, lpWndPos->cx, lpWndPos->cy, SWP_NOZORDER);
}

return;
}
*/

// �ֲ㴰��
class SKIN_CONTROLS_EXPORT CSkinLayeredWnd : public CWnd
{
public:
	// ���캯��
	CSkinLayeredWnd();
	// ��������
	virtual ~CSkinLayeredWnd();

	// ���ܺ���
public:
	// ��������
	VOID CreateLayered(CWnd* pWndControl, CRect& rcWindow);
	// ��������
	VOID InitLayeredArea(CDC* pDCImage, BYTE cbAlpha, CRect& rcUnLayered, CPoint& PointRound, bool bUnLayeredChild);
	// ��������
	VOID InitLayeredArea(CSkinImage& Image, BYTE cbAlpha, CRect& rcUnLayered, CPoint& PointRound, bool bUnLayeredChild);

	// ��Ϣӳ��
protected:
	// �ر���Ϣ
	VOID OnClose();
	// ������Ϣ
	VOID OnSetFocus(CWnd* pOldWnd);

	// ��̬����
protected:
	// ö�ٺ���
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pWndControl; // �ؼ�����
};

#endif // SKIN_LAYERED_WND_H_