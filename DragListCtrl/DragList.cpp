/****************************************************************************
1.�Ƿ�����dragging������Ҫ��m_SrcItem

2.һ��ʼ����ʱ���������BeginDrag ��ȷ�� m_SrcItem ��ֵ���п�����SELECT_NONE

3.when finish dragging,must set m_SrcItem=SELECT_NONE;

****************************************************************************/

#include "stdafx.h"
#include "DragList.h"

vector<RECT>CDragList::ItemRect;
int			CDragList::m_ActiveItem=SELECT_NONE;
int			CDragList::m_SrcItem=SELECT_NONE;
HWND		CDragList::DragWnd=NULL;
HWND		CDragList::SrcWnd=NULL;
UINT		CDragList::part=-1;


void CDragList::BeginDrag(int SelectItem,HWND hWnd) 
{
	trace(_T("BeginDrag"));
	
	m_SrcItem=SelectItem;
	m_ActiveItem=SelectItem;    //����������û���ƶ��͵�����ôm_SrcItem==m_ActiveItem����˵����ͬһ��item����
	part=-10;					//������Dragging�и�֪�ǵ�һ�ΰ��£���û���ƶ�
	DragWnd=NULL; 
	SrcWnd=hWnd;
}


/****************************************************************************
����ֵ��	DRAG_INVALID		-- ��Ч���ƶ�
			DRAG_IN_SAMEITEM	-- �ƶ���Ŀ��λ����ԭ��item��λ��
			DRAG_SUCCESSFUL		-- �ɹ��ƶ�
****************************************************************************/
int CDragList::EndDrag(HWND ListWnd) 
{
	trace(_T("EndDrag"));

	//������������������������������Ч���ƶ�����������������������������
	if ( m_SrcItem==SELECT_NONE )
		return DRAG_INVALID;

	//�����������������������������ƶ���Ŀ��λ����ԭ��item��λ�é���������
	if (SrcWnd==ListWnd && ( GetTargetIndex()==m_SrcItem || GetTargetIndex()-m_SrcItem==1 ) )
	{
		trace("�ƶ���Ŀ��λ����ԭ��item��λ��");
		return DRAG_IN_SAMEITEM;  
	}

	if(SrcWnd==ListWnd)
		::SendMessage(ListWnd,WM_DRAGLIST,DRAG_SAME_WND,NULL); 
	else
		::SendMessage(ListWnd,WM_DRAGLIST,DRAG_BETWEEN_TWO_WND,(LPARAM)SrcWnd); 

	m_SrcItem=SELECT_NONE;

	return DRAG_SUCCESSFUL;
}




void CDragList::GetItemRect()
{
	::SendMessage(DragWnd,WM_DRAGLIST,GET_LIST_RECT,(LPARAM)&ItemRect); 
}


VOID CALLBACK CDragList::TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{	
	POINT point;
	::GetCursorPos(&point);
	RECT rect; 
	::GetWindowRect(DragWnd,&rect);

	//����м��������û�а���ʱȡ��ʱ�� ������϶��ļ���list��ӣ�
	if (GetKeyState(VK_MBUTTON)>=0 && GetKeyState(VK_LBUTTON)>=0 )
	{
		::KillTimer(NULL,idEvent);	trace("mouse boutton up and stop timer");

		//�ͷſռ�
		vector<RECT>().swap(ItemRect); 

		m_SrcItem=SELECT_NONE;

		return;
	}

	//�жϴ����Ƿ�ֱ������
	if((GetWindowLong(DragWnd,GWL_STYLE)&WS_VSCROLL)==WS_VSCROLL)
	{
		if (point.y-rect.top<10 ) 
		{
			if(m_ActiveItem!=ItemRect.size()-1 && m_ActiveItem!=0) { EraseLine(); }
			::SendMessage(DragWnd,WM_VSCROLL,SB_LINEUP,0);  //trace("��list����");
			GetItemRect(); 
		}
		else if (rect.bottom-point.y<10 )	
		{
			if(m_ActiveItem!=ItemRect.size()-1 && m_ActiveItem!=0) { EraseLine(); }
			::SendMessage(DragWnd,WM_VSCROLL,SB_LINEDOWN,0);  //trace("��list�ײ�");
			GetItemRect(); 
		}
	}

	//ˮƽ������
	if( (GetWindowLong(DragWnd,GWL_STYLE)&WS_HSCROLL)==WS_HSCROLL)
	{
		if ( point.x-rect.left<10 ) //��list���
		{
			if(m_ActiveItem!=ItemRect.size()-1 && m_ActiveItem!=0) { EraseLine(); }
			::SendMessage(DragWnd,WM_HSCROLL,SB_LINELEFT,0);  //trace("��list���");
			GetItemRect(); 
		}
		else if ( rect.right-point.x<10 )	//��list�ұ�
		{
			if(m_ActiveItem!=ItemRect.size()-1 && m_ActiveItem!=0) { EraseLine(); }
			::SendMessage(DragWnd,WM_HSCROLL,SB_LINERIGHT,0);  //trace("��list�ұ�");
			GetItemRect(); 
		}
	}	

	//mouse outside window and erase the old line
	if (!::PtInRect(&rect,point))
	{
		trace("mouse outside window");
		EraseLine();
		m_ActiveItem=SELECT_NONE;
	}

}



/****************************************************************************
ListWnd ������MouseMove���ݽ�����hWnd����Ҫ������WindowFromPoint�������п��ܻ����ͬ��EndDraggingҲ�ǡ�
****************************************************************************/
void CDragList::Dragging(HWND ListWnd,int ListStyle) 
{
	if(m_SrcItem==SELECT_NONE)
		return;

	POINT point;
	::GetCursorPos(&point);
	::ScreenToClient(ListWnd,&point);
	
	if (part==-10)
	{
		part=-1; trace("���յ�һ��WM_MOUSEMOVE��������ʱ��");

		//the timer is on until button up
		::SetTimer(NULL,0,100,TimerProc);

		return;
	}

	if (DragWnd!=ListWnd)
	{
		trace(_T("the first time move in window"));

/*
		trace(DragWnd);
		trace(ListWnd);
*/

		if ( IsWindow(DragWnd) )
		{
			trace(ListWnd);
			EraseLine();
		}

		//important ����������ֲ���Ԥ�ϵĴ�������Ĵ������������m_ActiveItem��ֵ
		m_ActiveItem=SELECT_NONE; 

		//����DragWnd
		DragWnd=ListWnd;  

		//Get all List item rect;
		GetItemRect();

		//�����Լ�
		::BringWindowToTop(DragWnd); trace("BringWindowToTop");
	}

	//�ı������״
	//����������������ʱ������Ƿ�����������ԴIDC_MOVEITEMCUR��������StdAfx.h����� #include "Resource.h"
	::SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_MOVEITEMCUR)));	
	
	//begin find item
	UINT i;
	UINT count=ItemRect.size();
	for(i=0; i<count && !::PtInRect(&ItemRect[i],point) ; i++)	;	//ע�����еķֺ�
	
	//if found item(�������List����)
	if(i<count)  
	{
		//�������������������������������������������ItemRect���ϲ������²�������������������
		int TempPart;
		if (VERTICAL_LIST ==ListStyle)
			TempPart= (  point.y < (ItemRect[i].top + (ItemRect[i].bottom-ItemRect[i].top)/2 )  ) ? 1 : 2;
		else
			TempPart= (  point.x < (ItemRect[i].left+ (ItemRect[i].right-ItemRect[i].left)/2)   ) ? 1 : 2;
		//������������������������������������������������������������������

		//�Ƿ��һ�������Item
		if (m_ActiveItem!=i || part!=TempPart)	
		{
			//����ɵ�line����
			int Ox1,Oy1,Ox2,Oy2;
			CountLinePos(Ox1,Oy1,Ox2,Oy2,ListStyle);

			//���浱ǰ����İ�ť����
			m_ActiveItem=i;
			part=TempPart;
		//	trace(m_ActiveItem,part);
			
			//�����µ�line����
			int Nx1,Ny1,Nx2,Ny2;
			CountLinePos(Nx1,Ny1,Nx2,Ny2,ListStyle);

			//���������;����겻һ�����������ǰ��line
			if (Ox1 != Nx1 || Oy1 != Ny1 || Ox2 != Nx2 || Oy2 != Ny2 )
			{
	//			trace("�¾����겻һ��");
				RECT rect={Ox1-3,Oy1-3,Ox2+3,Oy2+3};
				::InvalidateRect(DragWnd,&rect,true);
				::UpdateWindow(DragWnd);
			}

			//��֪���ڣ��������һ���µ�item
			::SendMessage(ListWnd,WM_DRAGLIST,DRAG_ITEM_CHANGED,m_ActiveItem);

			//�����µ�line
			DrawLine(DragWnd,Nx1,Ny1,Nx2,Ny2);

			return;
		}		
	}
/*
	//if not found item(��겻��List����)
	else
	{
		//�����һ���Ƴ�List
		if (m_ActiveItem!=SELECT_NONE)
		{
			EraseLine();

			//��ʾ�����List��
			m_ActiveItem=SELECT_NONE;	
		}
	}	
*/
}	


int CDragList::GetTargetIndex()	
{
	if(m_ActiveItem==SELECT_NONE)
		return SELECT_NONE;
	return (part==2 ? m_ActiveItem+1 : m_ActiveItem) ; 
}


//������ǰ��line
void CDragList::EraseLine()
{
	if (0<=m_ActiveItem && m_ActiveItem<(int)ItemRect.size())
	{
		RECT rect={	ItemRect[m_ActiveItem].left-3,
					ItemRect[m_ActiveItem].top-3,
					ItemRect[m_ActiveItem].right+3,
					ItemRect[m_ActiveItem].bottom+3};
		::InvalidateRect(DragWnd,&rect,true);
	}
}


/****************************************************************************

****************************************************************************/
void CDragList::CountLinePos(int& x1,int& y1,int& x2,int& y2,int ListStyle)
{
	if (0<=m_ActiveItem && m_ActiveItem<(int)ItemRect.size())
	{
		if (VERTICAL_LIST ==ListStyle)
		{
			x1=ItemRect[m_ActiveItem].left+2;
			x2=ItemRect[m_ActiveItem].right-2;
			if (part==1)
				y1=y2=ItemRect[m_ActiveItem].top;
			else if (part==2)
				y1=y2=ItemRect[m_ActiveItem].bottom;
		}
		else
		{
			y1=ItemRect[m_ActiveItem].top+2;
			y2=ItemRect[m_ActiveItem].bottom-2;
			if (part==1)
				x1=x2=ItemRect[m_ActiveItem].left;
			else if (part==2)
				x1=x2=ItemRect[m_ActiveItem].right;
		}
	}
	else
	{
		y1=y2=x1=x2=0;
	}
}


void CDragList::DrawLine(HWND hWnd,int x1,int y1,int x2,int y2)
{
	HDC hdc=::GetDC(hWnd);

	if (x1==x2)
	{
		::MoveToEx(hdc, x1-3, y1+0	,NULL);		::LineTo(hdc, x1+3, y1+0);
		::MoveToEx(hdc, x1-2, y1+1	,NULL);		::LineTo(hdc, x1+2, y1+1);
		//����������������������������������
		::MoveToEx(hdc, x1-1, y1+2	,NULL);		::LineTo(hdc, x1-1, y2-1);
		::MoveToEx(hdc, x1+0, y1+2	,NULL);		::LineTo(hdc, x1+0, y2-1);
		//����������������������������������
		::MoveToEx(hdc, x2-3, y2-0	,NULL);		::LineTo(hdc, x2+3, y2-0);
		::MoveToEx(hdc, x2-2, y2-1	,NULL);		::LineTo(hdc, x2+2, y2-1);
	}
	else if (y1==y2)
	{
		::MoveToEx(hdc, x1+0, y1-3	,NULL);		::LineTo(hdc, x1+0, y1+3);
		::MoveToEx(hdc, x1+1, y1-2	,NULL);		::LineTo(hdc, x1+1, y1+2);
		//����������������������������������
		::MoveToEx(hdc, x1+2, y1-1	,NULL);		::LineTo(hdc, x2-1, y1-1);
		::MoveToEx(hdc, x1+2, y1+0	,NULL);		::LineTo(hdc, x2-1, y1+0);
		//����������������������������������
		::MoveToEx(hdc, x2-0, y2-3	,NULL);		::LineTo(hdc, x2-0, y2+3);
		::MoveToEx(hdc, x2-1, y2-2	,NULL);		::LineTo(hdc, x2-1, y2+2);
	}

	::ReleaseDC(hWnd,hdc) ;	
}