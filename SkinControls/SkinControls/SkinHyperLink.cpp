
#include "stdafx.h"
#include "Resource.h"
#include "SkinHyperLink.h"
#include "SkinHelper.h"

#define DEF_HOVERTEXT_COLOR			RGB(255,0,255)
#define DEF_VISITEDTEXT_COLOR		RGB(0,0,100)
#define DEF_NORMALTEXT_COLOR		RGB(0,0,230)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkinHyperLink, CStatic)

BEGIN_MESSAGE_MAP(CSkinHyperLink, CStatic)
	ON_WM_PAINT()
	ON_WM_CREATE()
    ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE(WM_SETTEXT,OnSetTextMesage)
	ON_CONTROL_REFLECT(STN_CLICKED, OnStnClicked)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// ���캯��
CSkinHyperLink::CSkinHyperLink()
{
	m_bVisited=false;
	m_bHovering=false;
	m_bUnderline=true;
	m_bAutoAdjust=true;
	m_crBack = CLR_NONE;
	m_crHoverText=RGB(250,0,0);
	m_crNormalText=RGB(0,0,255);
	m_crVisitedText=RGB(0,0,100);
}

// ��������
CSkinHyperLink::~CSkinHyperLink()
{
}


// �ؼ����໯
void CSkinHyperLink::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	// ��ʼ��
	InitHyperLink();

	return;
}

// ������Ϣ
int CSkinHyperLink::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	// ��ʼ��
	InitHyperLink();

	return 0;
}

HBRUSH CSkinHyperLink::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

// �滭��Ϣ
void CSkinHyperLink::OnPaint()
{
	CPaintDC dc(this);

	// ��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);

	// ���� DC
	dc.SetBkMode(TRANSPARENT);
	if (m_crBack != CLR_NONE) dc.FillSolidRect(&ClientRect, m_crBack);
	CFont* pOldFont = dc.SelectObject(&CSkinHelper::GetDefaultFont());
	if (m_bHovering) dc.SetTextColor(m_crHoverText);
	else if (m_bVisited) dc.SetTextColor(m_crVisitedText);
	else dc.SetTextColor(m_crNormalText);

	// �滭����
	CString strText;
	GetWindowText(strText);
	LONG dwStyte=GetWindowLong(m_hWnd,GWL_STYLE);
	UINT uFormat=DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
	if (dwStyte&SS_RIGHT) uFormat|=DT_RIGHT;
	else if (dwStyte&SS_CENTER) uFormat|=DT_CENTER;
	else uFormat|=DT_LEFT;
	dc.DrawText(strText,&ClientRect,uFormat);

	dc.SelectObject(pOldFont);

	return;
}

// �����Ϣ
BOOL CSkinHyperLink::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(CSkinHelper::GetSysHandCursor());

	return TRUE;
}

// �����ؼ�
void CSkinHyperLink::OnStnClicked()
{
	// �ж�״̬
	if ((m_strURL.IsEmpty())) return;

	// ����ҳ
	ShellExecute(NULL,TEXT("open"), m_strURL,NULL,NULL,SW_SHOW);

	// �ػ��ؼ�
	if (m_bVisited == false)
	{
		m_bVisited=true;
		Invalidate();
	}

	return;
}

// ����ƶ���Ϣ
void CSkinHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bHovering==false)
	{
		// ע����Ϣ
		m_bHovering=true;

		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);

		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	__super::OnMouseMove(nFlags, point);
}

// ����뿪��Ϣ
LRESULT CSkinHyperLink::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHovering=false;
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	return 0;
}

// ������Ϣ
LRESULT	CSkinHyperLink::OnSetTextMesage(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult=Default();
	if (m_bAutoAdjust) AdjustHyperLink();
	return lResult;
}

// ���÷���
void CSkinHyperLink::SetVisited(bool bVisited)
{
	if (m_bVisited!=bVisited)
	{
		m_bVisited=bVisited;
		if (GetSafeHwnd()) RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}
	return;
}

// �����»���
void CSkinHyperLink::SetUnderline(bool bUnderline)
{
	if (m_bUnderline!=bUnderline)
	{
		m_bUnderline=bUnderline;
		if (GetSafeHwnd()) RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}
	return;
}

// ���� URL
void CSkinHyperLink::SetHyperLinkUrl(LPCTSTR pszUrl)
{
	// ���ñ���
	m_strURL=pszUrl;

	return;
}

// ���õ���
void CSkinHyperLink::SetAutoAdjust(bool bAutoAdjust)
{
	if (m_bAutoAdjust!=bAutoAdjust)
	{
		m_bAutoAdjust=bAutoAdjust;
		if (m_bAutoAdjust) AdjustHyperLink();
	}
	return;
}

// ������ɫ
void CSkinHyperLink::SetBackColor(COLORREF crBackColor)
{
	// ���ñ���
	m_crBack = crBackColor;

	// �ػ�����
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return;
}

// ������ɫ
void CSkinHyperLink::SetHyperLinkTextColor(COLORREF crHoverText, COLORREF crNormalText, COLORREF crVisitedText)
{
	// ���ñ���
	m_crHoverText = crHoverText;
	m_crNormalText = crNormalText;
	m_crVisitedText = crVisitedText;

	// �ػ�����
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return;
}

// ��ʼ��
void CSkinHyperLink::InitHyperLink()
{
	SetWindowLong(m_hWnd,GWL_STYLE,GetStyle()|SS_NOTIFY);
	if (m_bAutoAdjust) AdjustHyperLink();

	return;
}

// ����λ��
void CSkinHyperLink::AdjustHyperLink()
{
	// ��ȡ����
	CString strText;
	GetWindowText(strText);

	// ��ȡλ��
	CRect ControlRect;
	GetWindowRect(&ControlRect);
	GetParent()->ScreenToClient(&ControlRect);

	// ���㳤��
	CDC* pDC=GetDC();
	CSize Size=pDC->GetTextExtent(strText);
	ReleaseDC(pDC);

	// �ƶ�λ��
	MoveWindow(ControlRect.left,ControlRect.top,Size.cx,Size.cy);

	return;
}

