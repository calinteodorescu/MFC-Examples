
#include "stdafx.h"
#include "SkinTreeCtrl.h"
#include "SkinHelper.h"

#define ITEM_HEIGHT 26 // ����߶�

IMPLEMENT_DYNAMIC(CSkinTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CSkinTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMLClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
END_MESSAGE_MAP()

CSkinTreeCtrl::CSkinTreeCtrl()
{
	// ���ñ���
	m_nXScroll=0;
	m_nYScroll=0;

	// ��������
	m_hItemMouseHover=NULL;
	m_hTreeClickExpand=NULL;

	m_crBack = RGB(255,255,255);
	m_crLine = RGB(223,223,223);
	m_crSelected = RGB(253,231,161);
	m_crHover = RGB(163, 219, 255);
}

CSkinTreeCtrl::~CSkinTreeCtrl()
{
}

// ������Ϣ
int CSkinTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	// ��������
	SetItemHeight(ITEM_HEIGHT);
	ModifyStyle(0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT|TVS_FULLROWSELECT|TVS_HASLINES);

	// ���ù���
	m_SkinScrollBar.InitScroolBar(this);

	return 0;
}

// �ؼ����໯
void CSkinTreeCtrl::PreSubclassWindow() 
{
	// ��������
	SetItemHeight(ITEM_HEIGHT);
	ModifyStyle(0,TVS_HASBUTTONS|TVS_HASLINES|TVS_SHOWSELALWAYS|TVS_TRACKSELECT|TVS_FULLROWSELECT|TVS_HASLINES);

	// ���ù���
	m_SkinScrollBar.InitScroolBar(this);

	CTreeCtrl::PreSubclassWindow();
}

// ���ں���
LRESULT CSkinTreeCtrl::DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	// ˫����Ϣ
	switch (uMessage)
	{
	case WM_LBUTTONDOWN: // �����Ϣ
		{
			// �������
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			// ���ñ���
			m_hTreeClickExpand=NULL;

			// �������
			HTREEITEM hTreeItem=HitTest(MousePoint);

			// ��Ϣ����
			if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			{
				// ѡ������
				SetFocus();
				Select(hTreeItem,TVGN_CARET);

				// ��ȡλ��
				CRect rcTreeItem;
				GetItemRect(hTreeItem,&rcTreeItem,TRUE);

				// ��Ϣ����
				if (rcTreeItem.PtInRect(MousePoint)==TRUE)
				{
					// չ���б�
					if (ExpandVerdict(hTreeItem)==false)
					{
						m_hTreeClickExpand=hTreeItem;
						Expand(m_hTreeClickExpand,TVE_EXPAND);
					}

					return 0;
				}
			}

			break;
		}
	case WM_LBUTTONDBLCLK: // �����Ϣ
		{
			// �������
			CPoint MousePoint;
			MousePoint.SetPoint(LOWORD(lParam),HIWORD(lParam));

			// �������
			HTREEITEM hTreeItem=HitTest(MousePoint);

			// ��Ϣ����
			if ((hTreeItem!=NULL)&&(GetChildItem(hTreeItem)!=NULL))
			{
				// ѡ������
				SetFocus();
				Select(hTreeItem,TVGN_CARET);

				// λ���ж�
				CRect rcTreeItem;
				GetItemRect(hTreeItem,&rcTreeItem,TRUE);
				if (rcTreeItem.PtInRect(MousePoint)==FALSE) break;

				// չ���ж�
				if ((m_hTreeClickExpand!=hTreeItem)&&(ExpandVerdict(hTreeItem)==true))
				{
					// ���ñ���
					m_hTreeClickExpand=NULL;

					// չ���б�
					Expand(hTreeItem,TVE_COLLAPSE);
				}

				return 0;
			}

			break;
		}
	}

	return __super::DefWindowProc(uMessage,wParam,lParam);
}

// ����Դ����ͼƬ���ü�ͷ
void CSkinTreeCtrl::SetArrowImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf)
{
	// ����λͼ
	m_ImageArrow.DestroyImage();
	m_ImageArrow.LoadFromResource(hInst, lpszResource, bf);
	ASSERT(m_ImageArrow.IsValid());
}

// ���ļ�����ͼƬ���ü�ͷ
void CSkinTreeCtrl::SetArrowImage(LPCTSTR pszFileName, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pszFileName);
	if (pszFileName == NULL) return ;

	// ����λͼ
	m_ImageArrow.DestroyImage();
	m_ImageArrow.LoadFromFile(pszFileName, bf);
	ASSERT(m_ImageArrow.IsValid());
}

// ���ڴ����ͼƬ���ü�ͷ
void CSkinTreeCtrl::SetArrowImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf)
{
	// Ч�����
	ASSERT(pBuffer && dwBufferSize);
	if (pBuffer == NULL || dwBufferSize == 0) return;

	// ����ͼ
	m_ImageArrow.DestroyImage();
	m_ImageArrow.LoadFromMemory(pBuffer, dwBufferSize, bf);
	ASSERT(m_ImageArrow.IsValid());
}

// ��ȡѡ��
HTREEITEM CSkinTreeCtrl::GetCurrentSelectItem(bool bOnlyText)
{
	// ��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	// ����ж�
	HTREEITEM hTreeItem=HitTest(MousePoint);

	if (bOnlyText==true)
	{
		// ��ȡλ��
		CRect rcTreeItem;
		GetItemRect(hTreeItem,&rcTreeItem,TRUE);

		// ѡ���ж�
		if (rcTreeItem.PtInRect(MousePoint)==FALSE) return NULL;
	}

	return hTreeItem;
}

// չ��״̬
bool CSkinTreeCtrl::ExpandVerdict(HTREEITEM hTreeItem)
{
	if (hTreeItem!=NULL)
	{
		UINT uState=GetItemState(hTreeItem,TVIS_EXPANDED);
		return ((uState&TVIS_EXPANDED)!=0);
	}

	return false;
}

// չ���б�
bool CSkinTreeCtrl::ExpandListItem(HTREEITEM hTreeItem)
{
	// Ч�����
	ASSERT(hTreeItem!=NULL);
	if (hTreeItem==NULL) return false;

	// չ���б�
	HTREEITEM hCurrentItem=hTreeItem;
	do
	{
		Expand(hCurrentItem,TVE_EXPAND);
		hCurrentItem=GetParentItem(hCurrentItem);
	} while (hCurrentItem!=NULL);

	return true;
}

// �滭����
void CSkinTreeCtrl::DrawTreeItem(CDC* pDC, CRect& rcClient, CRect& rcClipBox)
{
	// �����ж�
	HTREEITEM hItemCurrent=GetFirstVisibleItem();
	if (hItemCurrent==NULL) return;

	// ��ȡ����
	UINT uTreeStyte=GetWindowLong(m_hWnd,GWL_STYLE);

	// �滭����
	do
	{
		// ��������
		CRect rcItem;
		CRect rcRect;

		// ��ȡ״̬
		HTREEITEM hParent=GetParentItem(hItemCurrent);
		UINT uItemState=GetItemState(hItemCurrent,TVIF_STATE);

		// ��ȡ����
		bool bDrawChildren=(ItemHasChildren(hItemCurrent)==TRUE);
		bool bDrawSelected=(uItemState&TVIS_SELECTED)&&((this==GetFocus())||(uTreeStyte&TVS_SHOWSELALWAYS));

		// ��ȡ����
		if (GetItemRect(hItemCurrent,rcItem,TRUE))
		{
			// �滭����
			if (rcItem.top>=rcClient.bottom) break;
			if (rcItem.top>=rcClipBox.bottom) continue;

			// ����λ��
			rcRect.left=0;
			rcRect.top=rcItem.top+1;
			rcRect.bottom=rcItem.bottom;
			rcRect.right=rcClient.Width();

			// �滭ѡ��
			if (bDrawSelected==true)
			{
				pDC->FillSolidRect(&rcRect, m_crSelected);
			}

			// �滭����
			if ((bDrawSelected==false)&&(m_hItemMouseHover==hItemCurrent))
			{
				pDC->FillSolidRect(&rcRect, m_crHover);
			}

			// ���Ƽ�ͷ
			if (bDrawChildren==true)
			{
				// ����λ��
				//int nXPos=rcItem.left-m_ImageArrow.GetWidth()/2 - 25;
				INT nXPos=rcItem.left-m_ImageArrow.GetWidth()/2 - 5;
				INT nYPos=rcItem.top+1+(rcItem.Height()-m_ImageArrow.GetHeight())/2;

				// �滭ͼ��
				int nIndex=(uItemState&TVIS_EXPANDED)?1L:0L;
				m_ImageArrow.DrawImage(pDC,nXPos,nYPos,m_ImageArrow.GetWidth()/2,m_ImageArrow.GetHeight(),
					nIndex*m_ImageArrow.GetWidth()/2,0,m_ImageArrow.GetWidth()/2,m_ImageArrow.GetHeight(),MASK_COLOR);
			}

			// �����б�
			//DrawListImage(pDC,rcItem,hItemCurrent);	

			// �����ı�
			DrawItemString(pDC,rcItem,hItemCurrent,bDrawSelected);
		}
	} while ((hItemCurrent=GetNextVisibleItem(hItemCurrent))!= NULL);

	return;
}

// �滭����
void CSkinTreeCtrl::DrawTreeBack(CDC* pDC, CRect& rcClient, CRect& rcClipBox)
{
	// �滭����
	pDC->FillSolidRect(&rcClient, m_crBack);

	// ���ƺ���
	for (int nYPos=m_nYScroll/ITEM_HEIGHT*ITEM_HEIGHT;nYPos<rcClient.Height();nYPos+=ITEM_HEIGHT)
	{
		pDC->FillSolidRect(0,nYPos,rcClient.Width(),1,m_crLine);
	}

	return;
}

// �滭ͼ��
void CSkinTreeCtrl::DrawListImage(CDC* pDC, CRect rcRect, HTREEITEM hTreeItem)
{
	// ��ȡ����
	int nImage,nSelectedImage;
	GetItemImage(hTreeItem,nImage,nSelectedImage);

	//// ��ȡ��Ϣ
	//IMAGEINFO ImageInfo;
	//m_ImageList.GetImageInfo(nImage,&ImageInfo);

	//// �滭ͼ��
	//int nImageWidth=ImageInfo.rcImage.right-ImageInfo.rcImage.left;
	//int nImageHeight=ImageInfo.rcImage.bottom-ImageInfo.rcImage.top;
	//m_ImageList.Draw(pDC,nImage,CPoint(rcRect.left-nImageWidth-3,rcRect.top+(rcRect.Height()-nImageHeight)/2+1),ILD_TRANSPARENT);

	return;
}

// �����ı�
void CSkinTreeCtrl::DrawItemString(CDC* pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected)
{
	// ������ɫ
	COLORREF crString=RGB(0,0,0);

	// ��������
	pDC->SelectObject(&CSkinHelper::GetDefaultFont());

	// ���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crString);

	// �滭����
	CString strString=GetItemText(hTreeItem);
	pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

// �ػ���Ϣ
void CSkinTreeCtrl::OnPaint()
{
	CPaintDC dc(this);

	// ����λ��
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	// ��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	// ��������
	CDC BufferDC;
	CBitmap BufferImage;
	BufferDC.CreateCompatibleDC(&dc);
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	// ���� DC
	BufferDC.SelectObject(&BufferImage);

	// �滭�ؼ�
	DrawTreeBack(&BufferDC,rcClient,rcClip);
	DrawTreeItem(&BufferDC,rcClient,rcClip);

	// �滭����
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	// ɾ����Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

// �滭����
BOOL CSkinTreeCtrl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

// λ����Ϣ
void CSkinTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// ��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	// ��ȡ��Ϣ
	SCROLLINFO ScrollInfoH;
	SCROLLINFO ScrollInfoV;
	ZeroMemory(&ScrollInfoH,sizeof(ScrollInfoH));
	ZeroMemory(&ScrollInfoV,sizeof(ScrollInfoV));

	// ��ȡ��Ϣ
	GetScrollInfo(SB_HORZ,&ScrollInfoH,SIF_POS|SIF_RANGE);
	GetScrollInfo(SB_VERT,&ScrollInfoV,SIF_POS|SIF_RANGE);

	// ���ñ���
	m_nXScroll=-ScrollInfoH.nPos;
	m_nYScroll=-ScrollInfoV.nPos;

	return;
}

// ����ƶ�
void CSkinTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// �������
	HTREEITEM hItemMouseHover=HitTest(point);

	// �ػ��ж�
	if ((hItemMouseHover!=NULL)&&(hItemMouseHover!=m_hItemMouseHover))
	{
		// ���ñ���
		m_hItemMouseHover=hItemMouseHover;

		// �ػ�����
		Invalidate(FALSE);
	}

	return __super::OnMouseMove(nFlags,point);
}

// �����Ϣ
BOOL CSkinTreeCtrl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	return TRUE;
}

// �Ҽ��б�
void CSkinTreeCtrl::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ��ȡѡ��
	HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	// ѡ���ж�
	if (hTreeItem==NULL) return;

	// ����ѡ��
	Select(hTreeItem,TVGN_CARET);

	return;
}

// ����б�
void CSkinTreeCtrl::OnNMLClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ��ȡѡ��
	HTREEITEM hTreeItem=GetCurrentSelectItem(false);

	// ѡ���ж�
	if (hTreeItem==NULL) return;

	// ����ѡ��
	Select(hTreeItem,TVGN_CARET);

	return;
}

// �б�ı�
void CSkinTreeCtrl::OnTvnSelchanged(NMHDR * pNMHDR, LRESULT * pResult)
{
	// ��ȡѡ��
	HTREEITEM hTreeItem=GetSelectedItem();

	// ѡ���ж�
	if (hTreeItem==NULL) return;

	return;
}

// �б�չ��
void CSkinTreeCtrl::OnTvnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ��������
	LPNMTREEVIEW pNMTreeView=reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// ��������
	if (pNMTreeView->action==TVE_EXPAND)
	{
	}

	return;
}
