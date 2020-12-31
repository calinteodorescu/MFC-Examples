// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "MyListCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAGLIST, DragMoveItem) 	
END_MESSAGE_MAP()





void CMyListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonDown(nFlags, point);

	//���ý���
	if (::GetFocus()!=m_hWnd )
		::SetFocus(m_hWnd);

	//���ListCtrl�ǵ�ѡ�ģ��������ַ�ʽ�ж�ѡ�е���
	int index=::SendMessage(m_hWnd,LVM_GETNEXTITEM,-1,MAKELPARAM(LVNI_SELECTED,0));

	trace(index);
	if (index!=-1)
	{
		//һ��Ҫ����CListCtrl::OnLButtonDown(nFlags, point);����
		CDragList::BeginDrag(index,m_hWnd);  //һ��Ҫ����OnLButtonDown(nFlags, point);����	
	}
}

void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDragList::EndDrag(m_hWnd);
	::InvalidateRect(m_hWnd,NULL,true);
	::SetFocus(m_hWnd);

	CListCtrl::OnLButtonUp(nFlags, point);
}

void CMyListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if ( nFlags&MK_LBUTTON )
	{
		CDragList::Dragging(m_hWnd);
		return;
	}
	
	CListCtrl::OnMouseMove(nFlags, point);
}




struct DRAGITEMDATA
{
	TCHAR ItemText0[_MAX_FNAME];
	TCHAR ItemText1[_MAX_FNAME];
	TCHAR ItemText2[_MAX_FNAME];
};


LRESULT CMyListCtrl::DragMoveItem(WPARAM wParam, LPARAM lParam) 	
{
	
	switch (wParam)
	{
	case GET_LIST_RECT:	//get all items rect
		{
			//get the count of listctrl
			int count=(int)::SendMessage(this->m_hWnd, LVM_GETITEMCOUNT, 0, 0);

			vector<RECT>& ItemRect=*(vector<RECT>*)lParam;
			ItemRect.resize(count);

			for (int i=0;i<count;i++)
			{
				//save item rect
				ItemRect[i].left=LVIR_SELECTBOUNDS ;
				::SendMessage(this->m_hWnd,  LVM_GETITEMRECT , i,(LPARAM)&ItemRect[i]); 
			}
		}
		break;

	case DRAG_SAME_WND:	//if drag item in the same window
		{
			int SrcIndex=CDragList::GetSrcIndex();
			int DesIndex=CDragList::GetTargetIndex();
			if(DesIndex==SELECT_NONE) 
				DesIndex=::SendMessage(this->m_hWnd, LVM_GETITEMCOUNT, 0, 0);

			//---------------------------����1.��ȡԭ�������Ϣ----------------------------------

			//��ȡҪ�ƶ�����
			LVITEM lvi;
			lvi.cchTextMax=_MAX_FNAME;

			//get the first column text
			TCHAR ItemText0[_MAX_FNAME];
			lvi.pszText=ItemText0;
			lvi.iSubItem=0;
			::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

			//get the second column text
			TCHAR ItemText1[_MAX_FNAME];
			lvi.pszText=ItemText1;
			lvi.iSubItem=1;
			::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

			//get the third column text
			TCHAR ItemText2[_MAX_FNAME];
			lvi.pszText=ItemText2;
			lvi.iSubItem=2;
			::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);


			//---------------------------����2.ɾ��ԭ����----------------------------------------
			this->DeleteItem(SrcIndex);


			//---------------------------����3.����ɾ��һ�����Ŀ��λ���п�����Ҫ����---------------
			if(DesIndex>SrcIndex)
				DesIndex--;

			//---------------------------����4.��Ŀ��λ��des������------------------------------------

			//��item�Ƶ�Ҫ�����λ��
			int nRow = this->InsertItem(DesIndex, ItemText0,0);//����1��"11"�����0�е����� ����(����,����,ͼ������)
			this->SetItemText(nRow, 1, ItemText1);	//���õ�һ������
			this->SetItemText(nRow, 2, ItemText2);	//���õ�һ������


			//�����ƶ����item
			this->SetItemState(DesIndex,LVIS_SELECTED,LVIS_SELECTED);
		}
		break;


	case DRAG_BETWEEN_TWO_WND:	//if drag item between two windows
		{
			HWND SrcWnd=(HWND)lParam;

			int DesIndex=CDragList::GetTargetIndex();
			if(DesIndex==SELECT_NONE) 
				DesIndex=::SendMessage(this->m_hWnd, LVM_GETITEMCOUNT, 0, 0);

			//send message to the original window to get the data of the 
			//item that is to be moved and then delete it.
			DRAGITEMDATA TempItem;
			::SendMessage(SrcWnd,WM_DRAGLIST,GET_AND_DELETE_ITEM,(LPARAM)&TempItem); 


			//��item�Ƶ�Ҫ�����λ��
			int nRow = this->InsertItem(DesIndex, TempItem.ItemText0,0);//����1��"11"�����0�е����� ����(����,����,ͼ������)
			this->SetItemText(nRow, 1, TempItem.ItemText1);	//���õ�һ������
			this->SetItemText(nRow, 2, TempItem.ItemText2);	//���õ�һ������


			//�����ƶ����item
			this->SetItemState(DesIndex,LVIS_SELECTED,LVIS_SELECTED);
		}
		break;

	case GET_AND_DELETE_ITEM:	//when drag between two widnows,we should get the original item data and delete it 
		{
			int SrcIndex=CDragList::GetSrcIndex();

			DRAGITEMDATA& TempItem=*((DRAGITEMDATA*)lParam);
	

			//��ȡҪ�ƶ�����
			LVITEM lvi;
			lvi.cchTextMax=_MAX_FNAME;

			//get the first column text
			lvi.pszText=TempItem.ItemText0;
			lvi.iSubItem=0;
			::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

			//get the second column text
			lvi.pszText=TempItem.ItemText1;
			lvi.iSubItem=1;
			::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

			//get the third column text
			lvi.pszText=TempItem.ItemText2;
			lvi.iSubItem=2;
			::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

			//ɾ��ԭ������
			this->DeleteItem(SrcIndex);
		}
		break;

/*
	case DRAG_ITEM_CHANGED:
		{
				
		}
		break;
*/

	}

	return 0;
}







