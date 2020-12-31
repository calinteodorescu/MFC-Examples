
#include "StdAfx.h"
#include "Resource.h"
#include "SkinTabCtrl.h"
#include "SkinHelper.h"
#include "DrawHelpers.h"

CSkinImage CSkinTabCtrl::m_ImageDefTab; // Ĭ��ͼƬ

BEGIN_MESSAGE_MAP(CSkinTabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// ���캯��
CSkinTabCtrl::CSkinTabCtrl()
{
	// ���ñ���
	m_crBackGround = RGB(207, 229, 245);
	m_crBorder = RGB(76, 162, 225);

	m_nItempadding = 1;

	if (!m_ImageDefTab.IsValid())
	{
		HMODULE hModule = GetModuleHandle(SKIN_CONTROLS_DLL_NAME);
		m_ImageDefTab.LoadFromResource(hModule, MAKEINTRESOURCE(IDB_TAB));
	}
}

// ��������
CSkinTabCtrl::~CSkinTabCtrl()
{
}

// ���ð�ť���
void CSkinTabCtrl::SetItemPadding(int nPadding)
{
	m_nItempadding = nPadding;

	// ���½���
	if (m_hWnd != NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}
}

// ������ɫ
void CSkinTabCtrl::SetTabCtrlColor(COLORREF crBackGround, COLORREF crBorder)
{
	// ���ñ���
	m_crBackGround = crBackGround;
	m_crBorder = crBorder;

	// ���½���
	if (m_hWnd != NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return;
}

// ����TABͼ
bool CSkinTabCtrl::SetTabImage(LPCTSTR lpszFileName, BitmapFormat bf)
{
	// Ч�����
	ASSERT(lpszFileName);
	if (lpszFileName == NULL) return false;

	// ����ͼ
	m_ImageTab.DestroyImage();
	m_ImageTab.LoadFromFile(lpszFileName, bf);
	ASSERT(m_ImageTab.IsValid());

	if (GetSafeHwnd()) 
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return true;
}

// ����TABͼ
bool CSkinTabCtrl::SetTabImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pBuffer && dwBufferSize);
	if (pBuffer==NULL || dwBufferSize == 0) return false;

	// ����ͼ
	m_ImageTab.DestroyImage();
	m_ImageTab.LoadFromMemory(pBuffer, dwBufferSize, bf);
	ASSERT(m_ImageTab.IsValid());

	if (GetSafeHwnd()) 
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return true;
}

// ����TABͼ
bool CSkinTabCtrl::SetTabImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf)
{
	m_ImageTab.DestroyImage();
	m_ImageTab.LoadFromResource(hInst, lpszResource, bf);
	ASSERT(m_ImageTab.IsValid());

	if (GetSafeHwnd()) 
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return true;
}


// ����TAB����ͼ
bool CSkinTabCtrl::SetTabBgImage(LPCTSTR pszFileName, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pszFileName);
	if (pszFileName == NULL) return false;

	// ����ͼ
	m_ImageTabBg.DestroyImage();
	m_ImageTabBg.LoadFromFile(pszFileName, bf);
	ASSERT(m_ImageTabBg.IsValid());

	if (GetSafeHwnd()) 
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return true;
}

// ����TAB����ͼ
bool CSkinTabCtrl::SetTabBgImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pBuffer && dwBufferSize);
	if (pBuffer==NULL || dwBufferSize == 0) return false;

	// ����ͼ
	m_ImageTabBg.DestroyImage();
	m_ImageTabBg.LoadFromMemory(pBuffer, dwBufferSize, bf);
	ASSERT(m_ImageTabBg.IsValid());

	if (GetSafeHwnd()) 
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return true;
}

// ����TAB����ͼ
bool CSkinTabCtrl::SetTabBgImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf)
{
	m_ImageTabBg.DestroyImage();
	m_ImageTabBg.LoadFromResource(hInst, lpszResource, bf);
	ASSERT(m_ImageTabBg.IsValid());

	if (GetSafeHwnd()) 
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return true;
}

// ���󸽼ӵ����д���
void CSkinTabCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	CRect rcClient;
	GetWindowRect(&rcClient);
	if (m_ImageTab.IsValid())
	{
		SetWindowPos(NULL, 0, 0, rcClient.Width(), m_ImageTab.GetHeight(), SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rcClient.Width(), m_ImageDefTab.GetHeight(), SWP_NOMOVE);
	}

	return;
}

// ������Ϣ
int CSkinTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	return 0;
}

// ��С��Ϣ
void CSkinTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (m_ImageTab.IsValid())
	{
		SetWindowPos(NULL, 0, 0, cx, m_ImageTab.GetHeight(), SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, cx, m_ImageDefTab.GetHeight(), SWP_NOMOVE);
	}
}

// �������
void CSkinTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcItem, rcItemTemp;
	for (INT i = 0; i < GetItemCount(); i++)
	{
		GetItemRect(i ,&rcItem);
		rcItemTemp = rcItem;
		rcItemTemp.OffsetRect(0, -2);
		rcItemTemp.left = rcItemTemp.right - m_nItempadding;
		if (rcItemTemp.PtInRect(point))
		{
			return;
		}
	}

	__super::OnLButtonDown(nFlags, point);
}

// �ػ���Ϣ
void CSkinTabCtrl::OnPaint()
{
	// ��������
	CPaintDC dc(this);

	// ��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	// �ڴ�DC
	CBufferDC BufferDC(dc.m_hDC, rcClient);

	CSkinImage* pImage = NULL;
	if (m_ImageTab.IsValid()) pImage = &m_ImageTab;
	else pImage = &m_ImageDefTab;
	INT nImageHeight = pImage->GetHeight();

	// �滭����
	if (m_ImageTabBg.IsValid())
	{
		INT nBgWidth = m_ImageTabBg.GetWidth();
		for (int xpos = 0; xpos < rcClient.Width(); xpos += nBgWidth)
		{
			m_ImageTabBg.DrawImage(&BufferDC, xpos, 0);
		}
		
	}
	else
	{
		BufferDC.FillSolidRect(&rcClient, m_crBackGround);
		// �滭�߿�
		BufferDC.FillSolidRect(2, pImage->GetHeight()-1, rcClient.Width()-2, 1, m_crBorder);
	}

	// ���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&CSkinHelper::GetDefaultFont());

	// ��������
	TCITEM ItemInfo;
	TCHAR szBuffer[100] = { 0 };
	CRect rcItem;
	for (INT i = 0; i < GetItemCount(); i++)
	{
		ZeroMemory(&ItemInfo,sizeof(ItemInfo));
		ZeroMemory(szBuffer, sizeof(szBuffer));

		ItemInfo.mask = TCIF_TEXT|TCIF_IMAGE;
		ItemInfo.pszText = szBuffer;
		ItemInfo.cchTextMax = sizeof(szBuffer);

		// ��ȡ��Ϣ
		GetItem(i, &ItemInfo);
		GetItemRect(i, &rcItem);

		rcItem.OffsetRect(0, -2);
		rcItem.right -= m_nItempadding;
		
		// �滭��ť
		if (i!=GetCurSel())
		{
			pImage->DrawImage(&BufferDC, rcItem.left, rcItem.top, 5, nImageHeight, 0, 0, 5, nImageHeight, MASK_COLOR);
			for (INT xpos = rcItem.left + 5; xpos < rcItem.right - 5; xpos++)
			{
				pImage->DrawImage(&BufferDC, xpos, rcItem.top, 1, nImageHeight, 5, 0, 1, nImageHeight, MASK_COLOR);
			}
			pImage->DrawImage(&BufferDC, rcItem.right - 5, rcItem.top, 5, nImageHeight, 6, 0, 5, nImageHeight, MASK_COLOR);

			// ����
			rcItem.OffsetRect(0, 2);
			BufferDC.DrawText(szBuffer, lstrlen(szBuffer), &rcItem, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
		else 
		{
			pImage->DrawImage(&BufferDC, rcItem.left, rcItem.top, 5, nImageHeight, 11, 0, 5, nImageHeight, MASK_COLOR);
			for (INT xpos = rcItem.left + 5; xpos < rcItem.right - 5; xpos++)
			{
				pImage->DrawImage(&BufferDC, xpos, rcItem.top, 1, nImageHeight, 16, 0, 1, nImageHeight, MASK_COLOR);
			}
			pImage->DrawImage(&BufferDC, rcItem.right - 5, rcItem.top, 5, nImageHeight, 17, 0, 5, nImageHeight, MASK_COLOR);

			// ����
			rcItem.OffsetRect(0, 2);
			BufferDC.DrawText(szBuffer, lstrlen(szBuffer), &rcItem, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		}
	}

	return;
}

// �滭����
BOOL CSkinTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
