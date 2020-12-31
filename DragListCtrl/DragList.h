#ifndef _DRAG_LIST_H_
#define _DRAG_LIST_H_


/****************************************************************************
һ���ڳ������2���ļ� DragList.cpp �� DragList.h

���������괦��
������������������������������������������������������������������
	1.�����Լ���������ClistCtrl�������Ϣ������������´���

	//��갴�£�
	void CMyListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
	{
		CListCtrl::OnLButtonDown(nFlags, point);

		CDragList::BeginDrag(SelectIndex,m_hWnd);  //һ��Ҫ����OnLButtonDown(nFlags, point);����
	}

	//��굯��
	void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
	{
		CDragList::EndDrag(m_hWnd);

		CListCtrl::OnLButtonUp(nFlags, point);
	}

	//����ƶ�
	void CMyListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
	{
		if ( nFlags&MK_MBUTTON )
		{
			CDragList::Dragging(m_hWnd,point,VERTICAL_LIST);
			return;
		}
		
		CListCtrl::OnMouseMove(nFlags, point);
	}

�����������ݽ���
������������������������������������������������������������������
	ȷ�����������CDragList����ListCtrl����WM_DRAGLIST��Ϣ��
	ListCtrl������Զ�����Ϣ����������item�Ľ���������ListCtrl
	����Ҫ�޸ĵĴ󲿷ִ�����������忴ʾ������
  
	LRESULT CMyListCtrl::DragMoveItem(WPARAM wParam, LPARAM lParam) 	
	{
		
		switch (wParam)
		{
		case GET_LIST_RECT:	//get all items rect
			{
				vector<RECT>& ItemRect=*(vector<RECT>*)lParam;
				//��ListCtrl������items��rect�ŵ�ItemRect��
			}
			break;

		case DRAG_SAME_WND:	//if drag item in the same window
			{
				int SrcIndex=CDragList::GetSrcIndex();
				int DesIndex=CDragList::GetTargetIndex();
				if(DesIndex==SELECT_NONE) 
					DesIndex=::SendMessage(this->m_hWnd, LVM_GETITEMCOUNT, 0, 0);

				//---------------------------����1.��ȡԭ�������Ϣ----------------------------------
				...........
				::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

				//---------------------------����2.ɾ��ԭ����----------------------------------------
				this->DeleteItem(SrcIndex);

				//---------------------------����3.����ɾ��һ�����Ŀ��λ���п�����Ҫ����---------------
				if(DesIndex>SrcIndex)
					DesIndex--;

				//---------------------------����4.��Ŀ��λ��des������------------------------------------
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
			}
			break;

		case GET_AND_DELETE_ITEM:	//when drag between two widnows,we should get the original item data and delete it 
			{
				int SrcIndex=CDragList::GetSrcIndex();

				DRAGITEMDATA& TempItem=*((DRAGITEMDATA*)lParam);
		
				//��ȡҪ�ƶ�����
				::SendMessage(this->m_hWnd, LVM_GETITEMTEXT, (WPARAM)SrcIndex, (LPARAM)&lvi);

				//ɾ��ԭ������
				this->DeleteItem(SrcIndex);
			}
			break;

		//��draggingʱ���������һ���������յ�DRAG_ITEM_CHANGED��Ϣ�����������Ӧ�Ĵ���
		case DRAG_ITEM_CHANGED:
			{
					
			}
			break;
		}

		return 0;
	}

�ġ������Լ�ʾ����������
������������������������������������������������������������������
	����: http://hi.baidu.com/qiujiejia/blog/item/4912a3fba3e77b44034f562d.html

	1.�ɵ�QQ����(vc-code@qq.com,����code8888)����
	2.�����http://cid-3ba16e78a53d2d3d.office.live.com/self.aspx/VC/DragList2.zip

										Jacky
										qiujiejia@gmail.com
										2012-05-25
****************************************************************************/


#include <vector>
using namespace std;

//send message to the parent window
#define WM_DRAGLIST				WM_USER+435
#define GET_LIST_RECT			1
#define DRAG_ITEM_CHANGED		2
#define DRAG_SAME_WND			3
#define DRAG_BETWEEN_TWO_WND	4
#define GET_AND_DELETE_ITEM		5

#define SELECT_NONE				-1

//List Style
#define VERTICAL_LIST			0
#define HORIZONTAL_LIST			1

//drag result
#define DRAG_INVALID			1
#define DRAG_IN_SAMEITEM		2
#define DRAG_SUCCESSFUL			3


class CDragList  
{
public:
	static void BeginDrag(int SelectItem,HWND hWnd) ;
	static int  EndDrag(HWND ListWnd) ;
	static void Dragging(HWND ListWnd,int Style=VERTICAL_LIST) ;
	static int  GetTargetIndex();	
	static int  GetSrcIndex()		{return m_SrcItem;}

private:
	static UINT		part;				//�����ItemRect���ϲ������²�
	static vector	<RECT> ItemRect;	//��������item��rect
	static HWND		DragWnd;			//item���ڵĴ���
	static HWND		SrcWnd;				//��갴��ʱ�Ĵ���
	static int		m_ActiveItem;		//�϶�ʱ������ڵ�item
	static int		m_SrcItem;			//��Ҫ�ƶ���item��num

	static VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);
	static void	CountLinePos(int& x1,int& y1,int& x2,int& y2,int ListStyle);
	static void GetItemRect();
	static void DrawLine(HWND hWnd,int x,int y,int a,int b);
	static void EraseLine();
};


#endif // #define _DRAG_LIST_H_
