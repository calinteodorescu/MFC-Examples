// TabSheet.cpp : implementation file
//

#include "stdafx.h"
#include "TabSheet.h"
//#include "TestCTabSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSheet

CTabSheet::CTabSheet()
{
	//m_nNumOfPages = 0;
	m_nCurrentPage = 0;
	m_bHideTab=FALSE;
}

CTabSheet::~CTabSheet()
{
	m_arrayStatusTab.RemoveAll();
	m_pPages.RemoveAll();
	m_IDD.RemoveAll();
	m_Title.RemoveAll();
}


BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
	//{{AFX_MSG_MAP(CTabSheet)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSheet message handlers

//���ַ����Ӱ��ת��ȫ�ǣ�bDBC=FALSE�����ȫ��ת����ǣ�bDBC=TRUE��
CString strConv(const CString& strIn, BOOL bDBC /* = TRUE*/)
{   
	CString strTmp = strIn;   
	LPTSTR szText;   
	if(!strTmp.IsEmpty())
	{   
		szText = strTmp.GetBuffer(0);   
		size_t nLen = _tcslen(szText);   
		if(bDBC)
		{   
			for(size_t i=0;i<nLen;i++)
			{   
				if(12288==szText[i])   
					szText[i] = 32;   
				else
				{   
					if(szText[i]>65280 && szText[i]<65375)   
						szText[i] -= 65248;   
				}   
			}   
		}   
		else
		{   
			for(size_t i=0;i<nLen;i++)
			{   
				if(32==szText[i])   
					szText[i] = 12288;   
				else
				{   
					if(szText[i]<127)   
						szText[i] += 65248;   
				}   
			}   
		}   
		strTmp.ReleaseBuffer();   
	}   
	return strTmp;   
}

BOOL CTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT ID, int nImage)
{
	int iIndex = static_cast<int>( m_pPages.Add(pDialog) );
	m_IDD.Add(ID);
	//m_Title.Add(title);
	m_Title.Add(strConv(title,FALSE));//����ǩ�ı����ַ���ȫ��ת����ȫ���ַ����Ա��ǩ�������ʱ�����ܹ���ֱ����

	m_pPages[iIndex]->Create( m_IDD[iIndex], this );
	InsertItem( iIndex, m_Title[iIndex], nImage );
	SetRect(iIndex);
	m_pPages[iIndex]->ShowWindow(iIndex ? SW_HIDE : SW_SHOW);

	//** the initial status is enabled
	m_arrayStatusTab.Add(TRUE); 

	return TRUE;
}

void CTabSheet::SetRect(int iIndex)
{
	ASSERT(iIndex < m_pPages.GetCount());

	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;//�󡢶�������
	GetClientRect(&tabRect);//��ȡ����TAB�ؼ���λ�ô�С��
	GetItemRect(0, &itemRect);//itemָ��ֻ�Ǳ�ǩҳ����������ǻ�ñ�ǩҳ��λ�ô�С

	if (m_bHideTab)
	{
		nX=tabRect.left;//ʹ��tabRect.left��tabRect.top���ɽ���ҳ��ʾ���ӶԻ�����סTAB�ؼ��ı�ǩ��
		nY=tabRect.top;
		nXc=tabRect.Width();
		nYc=tabRect.Height();
	}
	else
	{
		DWORD style = GetStyle();
#define offset 2
		if(style & TCS_VERTICAL)
		{//ѡ���TAB�ؼ��Ĳ��
			nY = tabRect.top + offset;
			nYc = tabRect.bottom -nY - (offset + 1);
			if (style & TCS_BOTTOM)
			{//ѡ����ұ�
				nX = tabRect.left + offset;
				nXc = itemRect.left - nX - (offset + 1);
			}
			else
			{//ѡ������
				nX = itemRect.right + offset;
				nXc = tabRect.right - nX - (offset + 1);
			}
		}
		else
		{//ѡ���TAB�ؼ��Ķ�����ײ�
			nX = tabRect.left + offset;
			nXc = tabRect.right - nX - (offset + 1);
			if (style & TCS_BOTTOM)
			{//ѡ��ڵײ�
				nY = tabRect.top + offset;
				nYc = itemRect.top - nY - (offset + 1);
			}
			else
			{//ѡ��ڶ���
				nY = itemRect.bottom + offset;
				nYc = tabRect.bottom - nY - (offset + 1);
			}
		}
	}
	m_pPages[iIndex]->MoveWindow(nX, nY, nXc, nYc);
}

void CTabSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	int selectedPage = GetCurFocus();
	if (!m_arrayStatusTab[selectedPage])
	{
		CTabCtrl::SetCurSel(m_nCurrentPage);
		return;
	}

	if(m_nCurrentPage != selectedPage)
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage=selectedPage;
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	}
}

int CTabSheet::SetCurSel(int nItem)
{
	if( nItem < 0 || nItem >= m_pPages.GetCount())
		return -1;

	int ret = m_nCurrentPage;

	if(m_nCurrentPage != nItem )
	{
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = nItem;
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		CTabCtrl::SetCurSel(nItem);
	}

	return ret;
}

int CTabSheet::GetCurSel()
{
	return CTabCtrl::GetCurSel();
}

void CTabSheet::EnableTab(int iIndex, BOOL bEnable)
{
	ASSERT(iIndex < m_arrayStatusTab.GetSize());

	//** if it should change the status ----
	if (m_arrayStatusTab[iIndex] != bEnable)
	{
		m_arrayStatusTab[iIndex] = bEnable;
		m_pPages[iIndex]->EnableWindow(bEnable);

		//** redraw the item -------
		CRect rect;

		GetItemRect(iIndex, &rect);
		InvalidateRect(rect);
	}
}

void CTabSheet::EnableAllTabs(BOOL bEnable)
{
	for (int i=0; i<m_arrayStatusTab.GetCount(); i++)//CArray�������GetCount()��GetSize()����ͬ�ģ����Ƿ��������Ԫ�ظ�����
	{
		EnableTab(i, bEnable);
	}
}

void CTabSheet::DeleteAllTabs()
{
	m_arrayStatusTab.RemoveAll();

	DeleteAllItems();

	m_pPages.RemoveAll();
	m_IDD.RemoveAll();
	m_Title.RemoveAll();

}

void CTabSheet::DeleteTab(int iIndex)
{
	ASSERT(iIndex < m_pPages.GetCount());

	m_arrayStatusTab.RemoveAt(iIndex);

	DeleteItem(iIndex);

	m_pPages.RemoveAt(iIndex);
	m_IDD.RemoveAt(iIndex);
	m_Title.RemoveAt(iIndex);

}

BOOL CTabSheet::IsTabEnabled(int iIndex)
{
	ASSERT(iIndex < m_pPages.GetCount());

	return m_arrayStatusTab[iIndex];
}

//////////////////
// Draw the tab: mimic SysTabControl32, except use gray if tab is disabled
//
void CTabSheet::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	DRAWITEMSTRUCT& ds = *lpDrawItemStruct;

	int iItem = ds.itemID;

	// Get tab item info
	TCHAR text[128];
	TCITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = text;
	tci.cchTextMax = sizeof(text);
	GetItem(iItem, &tci);

	// use draw item DC
	CDC dc;
	dc.Attach(ds.hDC);

	DWORD style = GetStyle();//ȡTAB�ؼ���style�����ж��Ƿ�������TCS_VERTICAL����

	//**  Draw the image
	CRect rect = ds.rcItem;
	rect.top += ::GetSystemMetrics(SM_CYEDGE);
	dc.SetBkMode(TRANSPARENT);
	//dc.FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
	CImageList* pImageList = GetImageList();
	if (pImageList && tci.iImage >= 0) 
	{
		//rect.left += dc.GetTextExtent(_T(" ")).cx;
		rect.left += 2;
		rect.top += 1;

		IMAGEINFO info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect ImageRect(info.rcImage);

		pImageList->Draw(&dc, tci.iImage, CPoint(rect.left, rect.top), ILD_TRANSPARENT);

		if(style & TCS_VERTICAL)
		{//ѡ���TAB�ؼ��Ĳ��
			rect.top += ImageRect.Height();
		}
		else
		{//ѡ���TAB�ؼ��Ķ�����ײ�
			rect.left += ImageRect.Width();
		}
	}
/*
	typedef struct tagLOGFONT {
		LONG lfHeight;                 // �߶�
		LONG lfWidth;                  // ���
		LONG lfEscapement;             // ��ӡ�Ƕ�,��λ��0.1��,900��ֱ��ӡ,0ˮƽ��ӡ
		LONG lfOrientation;            // �����ӡ�Ƕ�,1800���µ���,900���ҵ���.
		LONG lfWeight;                 // �����ϸ,Ĭ����0,������400,700
		BYTE lfItalic;                 // б����,Ĭ��0��б��,1б��.
		BYTE lfUnderline;              // �»���,Ĭ��0��.
		BYTE lfStrikeOut;              // ���屻ֱ�ߴ���,Ĭ��0��.
		BYTE lfCharSet;                // �ַ���,��������,һ������ΪDEFAUL_CHARSET.
		BYTE lfOutPrecision;           // ���϶�,��������?һ������ΪOUT_DEFAUL_PRECIS
		BYTE lfClipPrecision;          // ����,һ������ΪCLIP_DEAFAUL_PRECIS
		BYTE lfQuality;                // ����ͼ������,����,��ΪDEFAUL_QUALITY
		BYTE lfPitchAndFamily;         // �ּ��,����,��ΪDEFAUL_PITCH+FF_DONTCARE
		TCHAR lfFaceName[LF_FACESIZE]; // ��������ʽ������,������ص���������
	} LOGFONT, *PLOGFONT;*/

	//Draw Text����������ʹ��LOGFONT���������л�ѡ���λ��ʱѡ���������ʾ���������⡣���Ƕ���Ӣ���ַ���������Ϊ���š�
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	//logFont.lfEscapement = 0;
	//logFont.lfOrientation = 900;//���ע�͵����У�����Ҳû�����кβ�ͬ
	//logFont.lfHeight = 16;//���ע�͵����У�����Ҳû�����кβ�ͬ
	CFont  newFont;
	newFont.CreateFontIndirect(&logFont);
	CFont *pOldFont = dc.SelectObject(&newFont);

//  	CFont vertFont;
// 	vertFont.CreateFont(16, 0, 900, 900, 0/*FW_BOLD*/, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial"));
// 	//vertFont.CreateFont(-20, 0, 900, 0, 400, FALSE, TRUE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("����"));
// 	//dc.SetTextAlign(   TA_TOP   |   TA_RIGHT   );
// 	CFont *pOldFont = dc.SelectObject(&vertFont);

	if(style & TCS_VERTICAL)
	{//ѡ���TAB�ؼ��Ĳ��
		OnDrawText(dc, rect, text, !IsTabEnabled(iItem), DT_WORDBREAK|DT_CENTER|DT_VCENTER);
	}
	else
	{//ѡ���TAB�ؼ��Ķ�����ײ�
		OnDrawText(dc, rect, text, !IsTabEnabled(iItem), DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}
	dc.SelectObject(pOldFont);

	dc.Detach();
}

void CTabSheet::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

//////////////////
// Draw tab text. You can override to use different color/font.
//
void CTabSheet::OnDrawText(CDC& dc, CRect rc, CString sText, BOOL bDisabled, UINT format)
{
	dc.SetTextColor(GetSysColor(bDisabled ? COLOR_3DHILIGHT : COLOR_BTNTEXT));
	dc.DrawText(sText, &rc, format);

	if (bDisabled)
	{// disabled: draw again shifted northwest for shadow effect
		rc += CPoint(-1,-1);
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		dc.DrawText(sText, &rc, format);
	}
}

BOOL CTabSheet::HideTab(BOOL bHide)
{
	m_bHideTab = bHide;

	for (int iIndex=0; iIndex<m_pPages.GetCount(); iIndex++)
	{
		SetRect(iIndex);
	}

	return m_bHideTab;
}

BOOL CTabSheet::IsTabHided()
{
	return m_bHideTab;
}

void CTabSheet::SetItemPos(ITEMPOS nItemPos)
{
	switch (nItemPos)
	{
	case TOP://Top
		ModifyStyle(TCS_VERTICAL|TCS_BOTTOM, 0);
		break;
	case BOTTOM://Bottom
		ModifyStyle(TCS_VERTICAL, TCS_BOTTOM);
		break;
	case LEFT://Left
		ModifyStyle(TCS_BOTTOM, TCS_VERTICAL);
		break;
	case RIGHT://Right
		ModifyStyle(0, TCS_BOTTOM|TCS_VERTICAL);
		break;
	default:
		break;
	};
	for (int iIndex=0; iIndex<m_pPages.GetCount(); iIndex++)
	{
		SetRect(iIndex);
	}
}