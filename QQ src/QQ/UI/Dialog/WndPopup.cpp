// WndPopup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WndPopup.h"


// CWndPopup

IMPLEMENT_DYNAMIC(CWndPopup, CWnd)

CWndPopup::CWndPopup()
{
	m_pImage = NULL;
	m_bTracking = false;
	m_bLButtonDown = false;
	m_bIsSetCapture = false;
	m_uMessageID = 0;

	VERIFY(m_font.CreateFont(
		12,							// ����ĸ߶�  
		0,							// ����Ŀ��  
		0,							// ������ʾ�ĽǶ�
		0,							// ����ĽǶ�
		FW_DONTCARE,				// ����İ���
		FALSE,						// б������
		FALSE,						// ���»��ߵ�����
		0,							// ��ɾ���ߵ�����
		GB2312_CHARSET,				// ������ַ���
		OUT_DEFAULT_PRECIS,			// ����ľ���
		CLIP_DEFAULT_PRECIS,		// �ü��ľ���
		DEFAULT_QUALITY,			// �߼�����������豸��ʵ��
		DEFAULT_PITCH | FF_SWISS,	// ����������弯
		"����"));					// ��������
}

CWndPopup::~CWndPopup()
{
}


BEGIN_MESSAGE_MAP(CWndPopup, CWnd)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CWndPopup::Create(CWnd *pParent, CRect rc, UINT uMessageID)
{
	CString sWindowClassName = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(NULL, IDC_ARROW));

	m_uMessageID = uMessageID;
	m_point.x = rc.left;
	m_point.y = rc.top;

	if(CWnd::CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,sWindowClassName ,NULL,WS_POPUP, rc, pParent, 0, NULL))
	{
		return	FALSE;
	}

	return TRUE;
}

int CWndPopup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ���ô��ڷ��
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
		| WS_SIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_EX_LAYERED;
	dwStyle &= ~(WS_CAPTION);

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	::SetWindowLong(m_hWnd,GWL_EXSTYLE, WS_EX_TOOLWINDOW); 

 	InitUI();
 	DrawWindow();

	SetWindowPos(NULL, m_point.x, m_point.y, 100, 100, SWP_SHOWWINDOW|SWP_NOSIZE);
	return TRUE;
}

void CWndPopup::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	CRect	rcWindow;

	if (bCalcValidRects && lpncsp->lppos)
	{
		rcWindow.SetRect(lpncsp->lppos->x, lpncsp->lppos->y,
			lpncsp->lppos->x + lpncsp->lppos->cx,
			lpncsp->lppos->y + lpncsp->lppos->cy);
	}
	else
	{
		GetWindowRect(rcWindow);
	}

	lpncsp->rgrc[0] = rcWindow;

}

LRESULT CWndPopup::OnNcHitTest(CPoint point)
{
	return	HTCLIENT;
}

BOOL CWndPopup::OnEraseBkgnd (CDC* pDC)
{
	return TRUE;
}

void CWndPopup::SetBackBitmap(UINT nResourceID)
{
	if(ImageFromIDResource(nResourceID, "PNG", m_pImage))
	{
		m_sizeBKImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
	}
}

void CWndPopup::SetBackBitmap(CString strImage)
{
	USES_CONVERSION; 
	m_pImage = Image::FromFile(A2W(strImage), TRUE);

	if(	m_pImage)
	{
		m_sizeBKImage.SetSize(m_pImage->GetWidth(), m_pImage->GetHeight());
	}
}

void CWndPopup::CloseWindow()
{
	PostMessage(WM_CLOSE);
}

void CWndPopup::DrawWindow()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC *pDC = GetDC();
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	CBitmap memBit;
	memBit.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

	CBitmap *pOldBit = MemDC.SelectObject(&memBit);

	MemDC.SetBkMode(TRANSPARENT);
	if(m_pImage)
	{
		Graphics graphics(MemDC);
		DrawImageFrame(graphics, m_pImage, rcClient,0, 0, m_sizeBKImage.cx, m_sizeBKImage.cy, 32);
	}
	else
	{
		MemDC.FillSolidRect(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), RGB(0, 147, 209));
		MemDC.FillSolidRect(rcClient.left + 1, rcClient.top + 1, rcClient.Width() - 2, rcClient.Height() - 2, RGB(255, 255, 255));
	}

	CDC TempMemDC;
	TempMemDC.CreateCompatibleDC(&MemDC);
	TempMemDC.SetBkMode(TRANSPARENT);

	CBitmap TempMemBit;
	TempMemBit.CreateCompatibleBitmap(&MemDC, rcClient.Width(), rcClient.Height());

	CBitmap *pTempOldBit = TempMemDC.SelectObject(&TempMemBit);

	TempMemDC.FillSolidRect(0, 0, rcClient.Width(), rcClient.Height(), RGB(255, 0, 255));

	DrawWindow(TempMemDC, rcClient);

	DWORD dwSize = TempMemBit.GetBitmapBits(0,NULL);
	unsigned char *lpBuffer = new unsigned char[dwSize];
	TempMemBit.GetBitmapBits(dwSize,lpBuffer);
	for( int i = 0 ; i+3 < dwSize ; i+=4 )
	{
		if(lpBuffer[i] == 255 && lpBuffer[i + 1] == 0 && lpBuffer[i + 2] == 255)
		{
			lpBuffer[i] = 0;
			lpBuffer[i + 1] = 0;
			lpBuffer[i + 2] = 0;
		}
		else 			
		{
			if(lpBuffer[i] == 0 && lpBuffer[i + 1] == 0 && lpBuffer[i + 2] == 0) lpBuffer[i] = 1;
			lpBuffer[i+3] = 255;
		}
	}
	TempMemBit.SetBitmapBits(dwSize, lpBuffer);
	delete[] lpBuffer;

	BLENDFUNCTION blendTemp = {AC_SRC_OVER,0,255,AC_SRC_ALPHA};
	MemDC.AlphaBlend(0, 0, rcClient.Width(), rcClient.Height(), &TempMemDC, 0, 0, rcClient.Width(), rcClient.Height(), blendTemp);

	TempMemDC.SelectObject(pTempOldBit);
	TempMemBit.DeleteObject();
	TempMemDC.DeleteDC();

	DrawWindowEx(MemDC, rcClient);
 	DWORD dwSizeMen = memBit.GetBitmapBits(0,NULL);
 	unsigned char *lpBufferMen = new unsigned char[dwSize];
 	memBit.GetBitmapBits(dwSizeMen,lpBufferMen);
 	for( int i = 0 ; i+3 < dwSizeMen ; i+=4 )
 	{
 		if(lpBufferMen[i] == 0 && lpBufferMen[i + 1] == 0 && lpBufferMen[i + 2] == 0) lpBufferMen[i] = 1;
 	}
 	memBit.SetBitmapBits(dwSizeMen, lpBufferMen);
 	delete[] lpBufferMen;


	ClientToScreen(&rcClient);
	POINT pointDes;
	pointDes.x = rcClient.left;
	pointDes.y = rcClient.top;
	POINT pointSrc;
	pointSrc.x = 0;
	pointSrc.y = 0;
	SIZE sizeDes;
	sizeDes.cx = rcClient.Width();
	sizeDes.cy = rcClient.Height();

	BLENDFUNCTION blend;
	memset( &blend, 0, sizeof( blend) );
	blend.AlphaFormat = AC_SRC_ALPHA ;
	blend.SourceConstantAlpha = 255;

	HWND hWnd = GetSafeHwnd();
	SetWindowLong(hWnd,GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	UpdateLayeredWindow(pDC, &pointDes, &sizeDes, &MemDC, &pointSrc, 0, &blend,m_pImage ? ULW_ALPHA : ULW_COLORKEY);

	MemDC.SelectObject(pOldBit);
	memBit.DeleteObject();
	MemDC.DeleteDC();

	ReleaseDC(pDC);
}

LRESULT CWndPopup::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	if (!m_bLButtonDown)
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		OnMouseMove(point);
	}

	return 0;
}

LRESULT CWndPopup::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	OnMouseMove(point);

	return 0;
}

void CWndPopup::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 50;
		m_bTracking = (bool)_TrackMouseEvent(&tme);
	}
	if(!m_bLButtonDown)
	{
		if(OnMouseMove(point))
		{
			DrawWindow();
		}
	}
}

void CWndPopup::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLButtonDown = true;
	SetCapture();
	m_bIsSetCapture = true;
	if(OnLButtonDown(point))
	{
		DrawWindow();		
		return;
	}

	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CWnd::OnLButtonDown(nFlags, point);
}

void CWndPopup::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsSetCapture)
	{
		ReleaseCapture();
		m_bIsSetCapture = false;
	}
	m_bLButtonDown = false;
	if(OnLButtonUp(point))
	{
		DrawWindow();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CWndPopup::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	CWnd::PostNcDestroy();
	delete this;
}

BOOL CWndPopup::OnNcActivate(BOOL bActive)
{
	if(!bActive)
	{
		CloseWindow();
	}
	return TRUE;	
}

// ����֪ͨ��Ϣ
bool CWndPopup::SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent = ::GetParent(m_hWnd);
	if (hParent)
	{
		::SendMessage(hParent, Msg, wParam, lParam);
		return true;
	}
	return false;
}

void CWndPopup::OnClose()
{
	SendMessage(WM_POPUP_CLOSE, 0, 0);

	CWnd::OnClose();
}
