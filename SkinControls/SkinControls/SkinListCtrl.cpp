
#include "Stdafx.h"
#include "Resource.h"
#include "SkinListCtrl.h"
#include "SkinHelper.h"

// ������Ϣ
struct tagSortInfo
{
	bool bAscendSort; // �����־
	WORD wColumnIndex; // �б�����
	CSkinListCtrl* pSkinListCtrl; // �б�ؼ�
};

BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

// Ĭ������
#define LIST_STYTE LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP|LVS_EX_ONECLICKACTIVATE

// ���캯��
CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	// ���ñ���
	m_uLockCount = 0L;
	m_uItemHeight = 130;
	m_crTitle = RGB(10,10,10);
}

// ��������
CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}

// �ؼ���
void CSkinHeaderCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	// ��������
	CFont Font;
	Font.CreatePointFont(m_uItemHeight, TEXT("����"));

	// ��������
	SetFont(&Font);

	return;
}

// �ؼ���Ϣ
BOOL CSkinHeaderCtrl::OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
	// ��������
	NMHEADER * pNMHearder=(NMHEADER*)lParam;

	// �϶���Ϣ
	if ((pNMHearder->hdr.code==HDN_BEGINTRACKA)||(pNMHearder->hdr.code==HDN_BEGINTRACKW))
	{
		// ��ֹ�϶�
		if (pNMHearder->iItem<(int)m_uLockCount)
		{
			*pLResult=TRUE;
			return TRUE;
		}
	}

	return __super::OnChildNotify(uMessage,wParam,lParam,pLResult);
}

// ��������
void CSkinHeaderCtrl::SetLockCount(UINT uLockCount)
{
	// ���ñ���
	m_uLockCount=uLockCount;

	return;
}

// �����и�
void CSkinHeaderCtrl::SetItemHeight(UINT uItemHeight)
{
	// ���ñ���
	m_uItemHeight = uItemHeight;

	// ���ÿؼ�
	if (m_hWnd != NULL)
	{
		// ��������
		CFont Font;
		Font.CreatePointFont(m_uItemHeight, TEXT("����"));

		// ��������
		SetFont(&Font);
	}

	return;
}

// ������Դ
void CSkinHeaderCtrl::SetHeaderImage(LPCTSTR pszFileName, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pszFileName);
	if (pszFileName == NULL) return;

	// ����ͼ
	m_ImageHead.DestroyImage();
	m_ImageHead.LoadFromFile(pszFileName, bf);
	ASSERT(m_ImageHead.IsValid());

	return;
}

// ������Դ
void CSkinHeaderCtrl::SetHeaderImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pBuffer && dwBufferSize);
	if (pBuffer==NULL || dwBufferSize == 0) return;

	// ����ͼ
	m_ImageHead.DestroyImage();
	m_ImageHead.LoadFromMemory(pBuffer, dwBufferSize, bf);
	ASSERT(m_ImageHead.IsValid());

	return;
}

// ������Դ
void CSkinHeaderCtrl::SetHeaderImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf)
{
	m_ImageHead.DestroyImage();
	m_ImageHead.LoadFromResource(hInst, lpszResource, bf);
	ASSERT(m_ImageHead.IsValid());

	return;
}

// �ػ�����
void CSkinHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

	// ��ȡλ��
	CRect rcRect;
	GetClientRect(&rcRect);

	// ��������
	CDC BufferDC;
	CBitmap BufferBmp;
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,rcRect.Width(),rcRect.Height());

	// ���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&BufferBmp);
	BufferDC.SetTextColor(m_crTitle);
	BufferDC.SelectObject(&CSkinHelper::GetDefaultFont());

	// �滭����
	m_ImageHead.DrawImage(&BufferDC, 0, 0, rcRect.Width(), rcRect.Height(), 1, 0, 1, m_ImageHead.GetHeight());

	// �滭����
	CRect rcItem;
	HDITEM HDItem;
	TCHAR szBuffer[64];
	for (int i = 0; i < GetItemCount(); i++)
	{
		// �������
		HDItem.mask=HDI_TEXT;
		HDItem.pszText=szBuffer;
		HDItem.cchTextMax=CountArray(szBuffer);

		// ��ȡ��Ϣ
		GetItem(i,&HDItem);
		GetItemRect(i,&rcItem);

		// �滭����
		m_ImageHead.DrawImage(&BufferDC, rcItem.left, rcItem.top, 1, rcItem.Height(), 0, 0, 1, rcItem.Height());
		m_ImageHead.DrawImage(&BufferDC, rcItem.left + 1, rcItem.top, rcItem.Width() - 2, rcItem.Height(), 1, 0, 1, m_ImageHead.GetHeight());
		m_ImageHead.DrawImage(&BufferDC, rcItem.right - 1, rcItem.top, 1, rcItem.Height(), 2, 0, 1, rcItem.Height());
	
		// �滭����
		rcItem.DeflateRect(3,1,3,1);
		BufferDC.DrawText(szBuffer, lstrlen(szBuffer), &rcItem, DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}

	// �滭����
	dc.BitBlt(0, 0, rcRect.Width(), rcRect.Height(),&BufferDC, 0, 0, SRCCOPY);

	// ������Դ
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	
	return;
}

// ��������
BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC * pDC) 
{
	return TRUE;
}

// ���캯��
CSkinListCtrl::CSkinListCtrl()
{
	// ���ñ���
	m_bAscendSort = false;
	m_crTitle = RGB(10,10,10);
	m_crListTX = RGB(10,10,10);
	m_crListBK = RGB(229,249,255);
}

// ��������
CSkinListCtrl::~CSkinListCtrl()
{
}

// �ؼ���
void CSkinListCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	// ���ù���
	m_SkinScrollBar.InitScroolBar(this);

	// ������ɫ
	SetBkColor(m_crListBK);
	SetTextColor(m_crListTX);
	SetTextBkColor(m_crListBK);

	// ��������
	SetExtendedStyle(LIST_STYTE);
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return;
}

// �滭����
void CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// ��������
	CRect rcItem = lpDrawItemStruct->rcItem;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	// ��ȡ����
	int nItemID = lpDrawItemStruct->itemID;
	int nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	// ������ɫ
	COLORREF rcTextColor = RGB(10,10,10);
	COLORREF rcBackColor = RGB(229,249,255);
	GetItemColor(lpDrawItemStruct,rcTextColor,rcBackColor);

	// �滭����
	CRect rcClipBox;
	pDC->GetClipBox(&rcClipBox);

	// ���û���
	pDC->SetBkColor(rcBackColor);
	pDC->SetTextColor(rcTextColor);

	// �滭����
	for (int i=0;i<nColumnCount;i++)
	{
		// ��ȡλ��
		CRect rcSubItem;
		GetSubItemRect(nItemID,i,LVIR_BOUNDS,rcSubItem);

		// �滭�ж�
		if (rcSubItem.left>rcClipBox.right) break;
		if (rcSubItem.right<rcClipBox.left) continue;

		// �滭����
		pDC->FillSolidRect(&rcSubItem,rcBackColor);

		// �滭����
		DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,i);
	}

	// �滭����
	if (lpDrawItemStruct->itemState&ODS_FOCUS)
	{
		pDC->DrawFocusRect(&rcItem);
	}

	return;
}

// ������Ϣ
int CSkinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	// ���ù���
	m_SkinScrollBar.InitScroolBar(this);

	// ������ɫ
	SetBkColor(m_crListBK);
	SetTextColor(m_crListTX);
	SetTextBkColor(m_crListBK);

	// ��������
	SetExtendedStyle(LIST_STYTE);
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return 0;
}

// ��ȡλ��
int CSkinListCtrl::GetInsertIndex(void* pItemData)
{
	return GetItemCount();
}

// ��������
int CSkinListCtrl::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort)
{
	// ��ȡ����
	TCHAR szBuffer1[256]=TEXT("");
	TCHAR szBuffer2[256]=TEXT("");
	GetItemText((int)lParam1,wColumnIndex,szBuffer1,CountArray(szBuffer1));
	GetItemText((int)lParam2,wColumnIndex,szBuffer2,CountArray(szBuffer2));

	// �Ա�����
	int nResult=lstrcmp(szBuffer1,szBuffer2);
	return (bAscendSort==true)?nResult:-nResult;
}

// �����ַ�
void CSkinListCtrl::ConstructString(void* pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	// ����Ч��
	ASSERT(pszString!=NULL);
	if (pszString==NULL) return;

	// ���ñ���
	pszString[0]=0;

	return;
}

// ��ȡ��ɫ
void CSkinListCtrl::GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crColorText, COLORREF& crColorBack)
{
	// ������ɫ
	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		// ѡ����ɫ
		crColorText=RGB(10,10,10);
		crColorBack=RGB(212,208,200);
	}
	else
	{
		// ���ñ���
		crColorText=RGB(10,10,10);
		crColorBack=RGB(229,249,255);
	}

	return;
}

// �滭����
void CSkinListCtrl::DrawCustomItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect& rcSubItem, int nColumnIndex)
{
	// ��ȡ����
	TCHAR szString[256]=TEXT("");
	GetItemText(lpDrawItemStruct->itemID,nColumnIndex,szString,CountArray(szString));

	// �滭����
	rcSubItem.left+=5;
	pDC->DrawText(szString,lstrlen(szString),&rcSubItem,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

// ��������
void CSkinListCtrl::InsertDataItem(void* pItemData)
{
	// Ч��״̬
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return;
	
	// ��������
	int nItemInsert=0;
	int nColumnCount=m_SkinHeaderCtrl.GetItemCount();

	// ��������
	for (int i=0;i<nColumnCount;i++)
	{
		// ��ȡ����
		TCHAR szString[256]=TEXT("");
		ConstructString(pItemData,i,szString,CountArray(szString));

		// �����б�
		if (i==0)
		{
			nItemInsert=InsertItem(LVIF_TEXT|LVIF_PARAM,GetInsertIndex(pItemData),szString,0,0,0,(LPARAM)pItemData);
		}
		else
		{
			SetItem(nItemInsert,i,LVIF_TEXT,szString,0,0,0,0);
		}
	}

	return;
}

// ��������
void CSkinListCtrl::UpdateDataItem(void* pItemData)
{
	// Ч��״̬
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return;
	
	// ��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	// ���ñ���
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pItemData;

	// ��������
	int nItem=FindItem(&FindInfo);
	int nColumnCount=m_SkinHeaderCtrl.GetItemCount();

	// ��������
	if (nItem!=-1L)
	{
		// ��������
		for (int i=0;i<nColumnCount;i++)
		{
			// ��ȡ����
			TCHAR szString[256]=TEXT("");
			ConstructString(pItemData,i,szString,CountArray(szString));

			// �����б�
			SetItem(nItem,i,LVIF_TEXT,szString,0,0,0,0);
		}

		// ��������
		RedrawItems(nItem,nItem);
	}

	return;
}

// ɾ������
void CSkinListCtrl::DeleteDataItem(void* pItemData)
{
	// Ч��״̬
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return;
	
	// ��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	// �������
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pItemData;

	// ɾ������
	int nItem=FindItem(&FindInfo);
	if (nItem!=-1L) DeleteItem(nItem);

	return;
}

// �滭����
BOOL CSkinListCtrl::OnEraseBkgnd(CDC * pDC)
{
	// ��ȡ����
	CWnd* pHeader=GetHeaderCtrl();

	// �����ж�
	if (pHeader->GetSafeHwnd()!=NULL)
	{
		// ��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		// ��ȡλ��
		CRect rcHeader;
		pHeader->GetWindowRect(&rcHeader);

		// �滭����
		rcClient.top=rcHeader.Height();
		pDC->FillSolidRect(&rcClient,GetBkColor());

		return TRUE;
	}

	return __super::OnEraseBkgnd(pDC);
}

// ���к���
int CALLBACK CSkinListCtrl::SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	// ��������
    tagSortInfo* pSortInfo=(tagSortInfo*)lParamList;
	CSkinListCtrl* pSkinListCtrl=pSortInfo->pSkinListCtrl;

	// ��������
	return pSkinListCtrl->SortItemData(lParam1,lParam2,pSortInfo->wColumnIndex,pSortInfo->bAscendSort);
}

// �����Ϣ
void CSkinListCtrl::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult)
{
	// ��������
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	// ��������
	tagSortInfo SortInfo;
	ZeroMemory(&SortInfo,sizeof(SortInfo));

	// ���ñ���
	SortInfo.pSkinListCtrl=this;
	SortInfo.bAscendSort=m_bAscendSort;
	SortInfo.wColumnIndex=pNMListView->iSubItem;

	// ���ñ���
	m_bAscendSort=!m_bAscendSort;

	// �����б�
	SortItems(SortFunction,(LPARAM)&SortInfo);

	return;
}

void CSkinListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	/*LVHITTESTINFO lht;
	lht.pt = point;
	lht.flags = LVHT_ABOVE;

	int nItem = SubItemHitTest(&lht);
	if(m_nMoveItem != nItem)
	{
		CRect rc;
		if (m_nMoveItem != -1)
			GetItemRect(m_nMoveItem, &rc,LVIR_BOUNDS);
		m_nMoveItem = lht.iItem;
		InvalidateRect(&rc, FALSE);
		if (m_nMoveItem != -1)
			GetItemRect(m_nMoveItem, &rc,LVIR_BOUNDS);
		InvalidateRect(&rc, FALSE);
	}*/

	return __super::OnMouseMove(nFlags, point);
}

// ��������
void CSkinListCtrl::MeasureItem(LPMEASUREITEMSTRUCT   lpMeasureItemStruct) 
{ 
	lpMeasureItemStruct->itemHeight  = 20; // Ҫ���õĸ߶�
} 