// MyListBox.cpp : implementation file
//

#include "stdafx.h"
#include "MyListBox.h"
#include "Resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyListBox

CMyListBox::CMyListBox()
{

}

CMyListBox::~CMyListBox()
{

}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	//{{AFX_MSG_MAP(CMyListBox)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(WM_DRAGLIST, DragMoveItem) 	

END_MESSAGE_MAP()


void CMyListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);

	//һ��Ҫ��ReleaseCapture����������
	::ReleaseCapture();

	int index=GetCurSel();;
	if (index!=LB_ERR )
	{
		//һ��Ҫ����OnLButtonDown(nFlags, point);����
		CDragList::BeginDrag(index,m_hWnd);  //һ��Ҫ����OnLButtonDown(nFlags, point);����	
	}
}

void CMyListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDragList::EndDrag(m_hWnd);
	::InvalidateRect(m_hWnd,NULL,true);
	::SetFocus(m_hWnd);

	CListBox::OnLButtonUp(nFlags, point);
}

void CMyListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( nFlags&MK_LBUTTON )
	{
		CDragList::Dragging(m_hWnd);
		return;
	}

	CListBox::OnMouseMove(nFlags, point);
}





LRESULT CMyListBox::DragMoveItem(WPARAM wParam, LPARAM lParam) 	
{
	
	switch (wParam)
	{
	case GET_LIST_RECT:	//get all items rect
		{
			//get the count of listctrl
			int count=GetCount();

			vector<RECT>& ItemRect=*(vector<RECT>*)lParam;
			ItemRect.resize(count);

			for (int i=0;i<count;i++)
			{
				//save item rect
				GetItemRect(i,&ItemRect[i]);
			}
		}
		break;

	case DRAG_SAME_WND:	//if drag item in the same window
		{
			int SrcIndex=CDragList::GetSrcIndex();
			int DesIndex=CDragList::GetTargetIndex();
			if(DesIndex==SELECT_NONE) 
				DesIndex=GetCount();

			//---------------------------����1.��ȡԭ�������Ϣ----------------------------------

			//��ȡҪ�ƶ�����
			TCHAR text[1000];
			this->GetText(SrcIndex,text);

			//---------------------------����2.ɾ��ԭ����----------------------------------------
			this->DeleteString(SrcIndex);

			//---------------------------����3.����ɾ��һ�����Ŀ��λ���п�����Ҫ����---------------
			if(DesIndex>SrcIndex)
				DesIndex--;

			//---------------------------����4.��Ŀ��λ��des������------------------------------------

			//��item�Ƶ�Ҫ�����λ��
			this->InsertString(DesIndex,text);
			this->SetCurSel(DesIndex);
		}
		break;


	case DRAG_BETWEEN_TWO_WND:	//if drag item between two windows
		{
			HWND SrcWnd=(HWND)lParam;

			int DesIndex=CDragList::GetTargetIndex();
			if(DesIndex==SELECT_NONE) 
				DesIndex=GetCount();

			//send message to the original window to get the data of the 
			//item that is to be moved and then delete it.
			TCHAR text[1000];;
			::SendMessage(SrcWnd,WM_DRAGLIST,GET_AND_DELETE_ITEM,(LPARAM)text); 

			//��item�Ƶ�Ҫ�����λ��
			this->InsertString(DesIndex,text);
			this->SetCurSel(DesIndex);
		}
		break;

	case GET_AND_DELETE_ITEM:	//when drag between two widnows,we should get the original item data and delete it 
		{
			int SrcIndex=CDragList::GetSrcIndex();

			TCHAR* text=(TCHAR*)lParam;
	
			//��ȡҪ�ƶ�����
			this->GetText(SrcIndex,text);

			//ɾ��ԭ������
			this->DeleteString(SrcIndex);
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