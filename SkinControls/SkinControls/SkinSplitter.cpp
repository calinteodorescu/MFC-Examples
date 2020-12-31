
#include "Stdafx.h"
#include "SkinSplitter.h"

BEGIN_MESSAGE_MAP(CSkinSplitter, CWnd)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// ���캯��
CSkinSplitter::CSkinSplitter()
{
	// ״̬����
	m_bMouseDown=false;
	m_rcDrawLine.SetRect(0,0,0,0);

	// ��ɫ����
	m_crBackColor=RGB(255,255,255);

	// ���Ա���
	m_nMaxDragPos=0;
	m_nMinDragPos=0;
	m_SplitterType=ST_VOR;

	// �������
	m_pISplitterEvent=NULL;

	return;
}

// ��������
CSkinSplitter::~CSkinSplitter()
{
}

// �󶨺���
void CSkinSplitter::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	// ��������
	ModifyStyle(0,SS_NOTIFY|SS_OWNERDRAW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	return;
}

// ������Ϣ
BOOL CSkinSplitter::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// ��Ϣת��
	GetParent()->SendMessage(WM_COMMAND,wParam,lParam);

	return TRUE;
}

// ��������
void CSkinSplitter::SetSplitterType(SplitterType st)
{
	// ���ñ���
	m_SplitterType=st;

	return;
}

// ���÷�Χ
void CSkinSplitter::SetSplitterRange(int nMinDragPos, int nMaxDragPos)
{
	// ���ñ���
	m_nMaxDragPos=nMaxDragPos;
	m_nMinDragPos=nMinDragPos;

	return;
}

// ���ýӿ�
void CSkinSplitter::SetSplitterEvent(ISplitterEvent * pISplitterEvent)
{
	// ���ñ���
	m_pISplitterEvent=pISplitterEvent;

	return;
}

// ������ɫ
void CSkinSplitter::SetSplitterColor(COLORREF crBackColor)
{
	// ���ñ���
	m_crBackColor=crBackColor;

	// �ػ�����
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_ERASENOW);

	return;
}

// �ػ���Ϣ
void CSkinSplitter::OnPaint()
{
	CPaintDC dc(this);

	// ��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	// �滭����
	dc.FillSolidRect(&rcClient,m_crBackColor);

	return;
}

// �滭����
BOOL CSkinSplitter::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

// ʧȥ����
void CSkinSplitter::OnKillFocus(CWnd * pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (m_bMouseDown==true)
	{
		// ��ȡ DC
		CDC* pDC=CDC::FromHandle(::GetDC(NULL));
		pDC->SelectObject(CDC::GetHalftoneBrush());

		// �滭����
		pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

		// �ͷ���Դ
		ReleaseCapture();
		::ReleaseDC(NULL,pDC->m_hDC);

		// ���ñ���
		m_bMouseDown=false;
		m_rcDrawLine.SetRectEmpty();
	}

	return;
}

// �����Ϣ
void CSkinSplitter::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags,Point);

	if (m_bMouseDown==true)
	{
		// ��ȡλ��
		CRect rcWindow;
		GetWindowRect(&rcWindow);

		// Ч��λ��
		CPoint PointParent=Point;
		MapWindowPoints(GetParent(),&PointParent,1);

		// �ؼ�λ��
		CSize SizeControl;
		SizeControl.SetSize(rcWindow.Width(),rcWindow.Height());

		// ���ù��
		if (m_SplitterType==ST_HOR)
		{
			if ((PointParent.y>(m_nMaxDragPos-SizeControl.cy/2))||(PointParent.y<(m_nMinDragPos+SizeControl.cy/2)))
			{
				// ���ù��
				SetCursor(LoadCursor(NULL,IDC_NO));

				// ����λ��
				PointParent.y=__min(m_nMaxDragPos-SizeControl.cy/2,PointParent.y);
				PointParent.y=__max(m_nMinDragPos+SizeControl.cy/2,PointParent.y);
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZENS));
		}
		else
		{
			if ((PointParent.x>(m_nMaxDragPos-SizeControl.cx/2))||(PointParent.x<(m_nMinDragPos+SizeControl.cx/2)))
			{
				// ���ù��
				SetCursor(LoadCursor(NULL,IDC_NO));

				// ����λ��
				PointParent.x=__min(m_nMaxDragPos-SizeControl.cx/2,PointParent.x);
				PointParent.x=__max(m_nMinDragPos+SizeControl.cx/2,PointParent.x);
			}
			else SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}

		// ת��λ��
		CPoint PointScreen=PointParent;
		GetParent()->ClientToScreen(&PointScreen);
		
		// ��������
		CRect rcCurrentDraw=m_rcDrawLine;
		if (m_SplitterType==ST_HOR)
		{
			rcCurrentDraw.top=PointScreen.y-SizeControl.cy/2;
			rcCurrentDraw.bottom=rcCurrentDraw.top+SizeControl.cy;
		}
		else
		{
			rcCurrentDraw.left=PointScreen.x-SizeControl.cx/2;
			rcCurrentDraw.right=rcCurrentDraw.left+SizeControl.cx;
		}
		
		// �滭����
		if (rcCurrentDraw!=m_rcDrawLine)
		{
			// ���� DC
			CDC * pDC=CDC::FromHandle(::GetDC(NULL));
			pDC->SelectObject(CDC::GetHalftoneBrush());

			// �滭���
			pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);
			pDC->PatBlt(rcCurrentDraw.left,rcCurrentDraw.top,rcCurrentDraw.Width(),rcCurrentDraw.Height(),PATINVERT);

			// �ͷ���Դ
			::ReleaseDC(NULL,pDC->m_hDC);

			// ���ñ���
			m_rcDrawLine=rcCurrentDraw;
		}
	}

	return;
}

// �����Ϣ
void CSkinSplitter::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	if (m_bMouseDown==true)
	{
		// �ָ�״̬
		ReleaseCapture();

		// ��ȡ DC
		CDC* pDC=CDC::FromHandle(::GetDC(NULL));
		pDC->SelectObject(CDC::GetHalftoneBrush());

		// �滭����
		pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

		// �ͷ���Դ
		::ReleaseDC(NULL,pDC->m_hDC);

		// ��������
		bool bRectify=false;

		// ��ȡλ��
		CRect rcControl;
		GetClientRect(&rcControl);
		MapWindowPoints(GetParent(),rcControl);

		// ת������
		CPoint PointParent=Point;
		MapWindowPoints(GetParent(),&PointParent,1);

		// �ؼ�λ��
		CSize SizeControl;
		SizeControl.SetSize(rcControl.Width(),rcControl.Height());

		// ����λ��
		if (m_SplitterType==ST_HOR)
		{
			// �ƶ�λ��
			PointParent.y=__min(m_nMaxDragPos-SizeControl.cy/2,PointParent.y);
			PointParent.y=__max(m_nMinDragPos+SizeControl.cy/2,PointParent.y);

			// �ƶ�����
			if (rcControl.top!=(PointParent.y-SizeControl.cy/2))
			{
				bRectify=true;
				SetWindowPos(NULL,rcControl.left,PointParent.y-SizeControl.cy/2,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS);
			}
		}
		else
		{
			// �ƶ�λ��
			PointParent.x=__min(m_nMaxDragPos-SizeControl.cx/2,PointParent.x);
			PointParent.x=__max(m_nMinDragPos+SizeControl.cx/2,PointParent.x);

			// �ƶ�����
			if (rcControl.left!=(PointParent.x-SizeControl.cx/2))
			{
				bRectify=true;
				SetWindowPos(NULL,PointParent.x-SizeControl.cx/2,rcControl.top,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOCOPYBITS);
			}
		}

		// ����֪ͨ
		if (bRectify==true)
		{
			// ����֪ͨ
			if (m_pISplitterEvent!=NULL) 
			{
				UINT uControlID=GetWindowLong(GetSafeHwnd(),GWL_ID);
				m_pISplitterEvent->OnSplitterEvent(uControlID,PointParent.x,PointParent.y);
			}
		}

		// ���ñ���
		m_bMouseDown=false;
		m_rcDrawLine.SetRectEmpty();
	}

	return;
}

// �����Ϣ
void CSkinSplitter::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	// ����״̬
	SetFocus();
	SetCapture();

	// ��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	// ���ñ���
	m_bMouseDown=true;
	m_rcDrawLine=rcWindow;

	// ���� DC
	CDC * pDC=CDC::FromHandle(::GetDC(NULL));
	pDC->SelectObject(CDC::GetHalftoneBrush());

	// �滭����
	pDC->PatBlt(m_rcDrawLine.left,m_rcDrawLine.top,m_rcDrawLine.Width(),m_rcDrawLine.Height(),PATINVERT);

	// �ͷ���Դ
	ReleaseDC(pDC);

	return;
}

// �����Ϣ
BOOL CSkinSplitter::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMmessage)
{
	// ��ȡλ��
	CPoint Point;
	GetCursorPos(&Point);

	// ���ù��
	if (pWnd==this)
	{
		SetCursor(LoadCursor(NULL,(m_SplitterType==ST_VOR)?IDC_SIZEWE:IDC_SIZENS));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMmessage);
}
