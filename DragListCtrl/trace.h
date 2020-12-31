/*==================trace ��������ַ���====================
(һ)���ܣ�
	������Ա���(������TRACE)

(��)�ص�:
	1.�Զ���Ӧ����������(char��char*��string��CString��HWND��POINT��RECT....)
	2.�Զ���Ӧ��������ĸ�����(�ر��˱������� #pragma warning(disable: 4003) )
	3.��������ı���ֵǰ���Զ���ӱ��������ƣ�����鿴
	4.�����Զ�����Edit�����������ת������ַ�����
	5.�����˳�ʱ�Ὣ����ַ������浽����Ŀ¼�µ�DebugData.txt������鿴
	6.���ֱ���ģʽ���������DEBUG��Release�汾����������Ա����������߶������
	7.֧��UNICODE,WIN32,Dll,MFC

(��)ʹ��˵����
	1.��trace.h���Ƶ�����Ŀ¼�£����Բ���ӵ����̣���
	2.���ļ�"stdafx.h"��ļ����·������ #include "trace.h"��֮��Ϳ���ʹ��trace()���ˡ�
	3.����������ַ����ᱣ���ڹ���Ŀ¼�µ�"DebugData.txt"�ļ����Է���鿴
	4.�Ұ����д��붼����һ��ͷ�ļ����Ȼ���Ϸ��淶��������ʹ�������ܷ��㡣
	5.trace(x,y,z,w)��ԭ����4������������������ʱ����������������棬
	  ����ʹ��	#pragma warning(disable: 4003) ��������뾯����ص��ˡ�

(��)����ʹ�õĺ꣺
	trace(X,Y,Z,W)	//����������ͱ�������int��double��short��POINT��RECT��string
					//���Զ���Ӧ�����ĸ�����������Ϊ1-4����
	tracef()		//��ʽ���ַ���������sprintf
	traceLastError()//���ϵͳ������룬������GetLastError()
	traceRel(X,Y)	 //��X=true�����"Y: successful" ; x=false�����"Y: failed"
	CEasyTrace::clear();			//��մ���
	

(��)����trace��ʹ�����ã�
	1.Ĭ������� NO_TRACE_WINDOW �� TRACE_WINDOW��û���壬��
	  DEBUG�汾����������ַ�������Release�汾����
	2.�����ͷ������#define NO_TRACE_WINDOW
	  DEBUG�汾��Release�汾�����������������ַ���
	3.�����ͷ������#define TRACE_WINDOW
	  DEBUG�汾��Release�汾���������������ַ���
	4.ÿ���޸�����2�������Ҫȫ�����±���Ż���Ч

(��)����ʾ����
	void CDemoDlg::OnButton1() 
	{
		int a=45; double b=67.45; double c=847.424;short d=324;
		RECT rect;
		::GetWindowRect(m_hWnd,&rect);
		HWND hWnd=::GetForegroundWindow();
		POINT point;
		GetCursorPos(&point);
		bool ak=true;
		CPoint cpoint(88,421);
		CRect crect(19,423,56,522);
		CString mfcstr=_T("mfc CString");
		TCHAR buf[100]=_T("c style string"); 

	#ifdef _UNICODE
		wstring stlstr=_T("stl string");
	#else
		string stlstr=_T("stl string");
	#endif

		tracef(_T("tracef ������ %s"),_T("sprintf")); //��ʽ���ַ���������sprintf
		trace(_T("Impossible is nothing"));
		trace(stlstr);   //����ַ���
		trace(mfcstr);   //����ַ���
		trace(buf);   //����ַ���
		trace(a);		//���int
		trace(a,b);		//���int��double
		trace(a,b,c);
		trace(a,b,c,d);
		trace(point);	
		trace(point,cpoint);	//���POINT �� CPoint	
		trace(rect);
		trace(rect,crect);		//���RECT �� CRect
		trace(hWnd);			//���������Ϣ
		traceLastError();		//���ϵͳ�������		
		traceRel(ak==true,"func()");  //��X=true�����"Y: successful" ; x=false�����"Y: failed"
	}

(��)���ɣ�
	1.��ʱ������ֻ��۲�ĳһ�������ı仯�����������кܶ�trace��������������ڵ�
	  ����Ӱ�������ǵĹ۲졣�������ǿ���������������������trace����Ч�ˡ�

	  traceStart();		//��������
	  trace(_T("ֻ�� traceStart �� traceStop ֮���trace����Ч"));
	  traceStop();		//��ֹ��������


��Ȩ����:
	û�а�Ȩ������κε�λ,��������ʹ��,����,�޸ģ������߲��е���
	�˴���������κ���ʧ����������ˮƽ���ޣ����������֮������
	�����⣬�ɴ�Ϊ������Ĳ��㣬����������������κ�BUG������ϵ���ߣ�
	���һ�����������Ժ��и���ͳһ����������ַ�ϣ��������������
	�����Լ���ϸ��ʹ��˵����
	��ǰ�������ڣ�2011-10-21
	http://blog.csdn.net/jacky_qiu/archive/2010/11/04/5986089.aspx

���:
	����˷ݴ���Ϊ������˰���������ʹ��������л֮�⡣���ҿɷ�����
	���Ĺ�������10�顰��(��:��na)��(��:Ħmo)�����ӷ𡱣�
	Ը�÷����ӳ֣�ʹ�㹤��˳�����ϼ��Ҹ���
	
											Jacky
											2011-5-25
											qiujiejia@gmail.com

========================================================================*/


#ifndef _EASY_TRACE_H_
#define _EASY_TRACE_H_


//trace����뿪�ة�����������������
//  #define NO_TRACE_WINDOW		//DEBUG�汾��Release�汾�����������������ַ���
//  #define TRACE_WINDOW		//DEBUG�汾��Release�汾���������������ַ���	
#ifdef NO_TRACE_WINDOW
#else
#ifdef _DEBUG
#define TRACE_WINDOW
#else
#ifdef	RELEASE_TRACE
#define TRACE_WINDOW
#endif
#endif
#endif

//trace��Ĳ�����4��������������4������������������棬�����ֶ��ر����
//����(disable warning C4003: not enough actual parameters for macro)
#pragma warning(disable: 4003)
#pragma warning(disable: 4002)


#define	TRACE_WND_TEXT _T("TraceWnd")	//���ڱ���
#define WM_CREATE_WND	WM_USER+888		//�Զ�����Ϣ
#define AUTO_HIDE_TIME	3000			//������ٺ�����Զ�����
#include <Windows.h> //��Ҫ��ͷ�ļ�
#include <tchar.h>
#include <sstream>


//Not #define TRACE_WINDOW
#ifndef TRACE_WINDOW 
#define trace(X)
#define tracef	__noop
#define traceLastError()	
#define traceRel(X)			
#define traceClear(X)				
#define traceStop(X)		
#define traceStart(X)		
#endif

//#define TRACE_WINDOW
#ifdef TRACE_WINDOW 

//����������������������������������������������������������trace�ꩥ������
#define trace(X,Y,Z,W) {CEasyTrace obj;	\
		obj.name(#X);	obj.add(X);	\
		obj.name(#Y);	obj.add(Y);	\
		obj.name(#Z);	obj.add(Z);	\
		obj.name(#W);	obj.add(W); }

#define tracef				  CEasyTrace::Tracef	
#define traceLastError()	{ CEasyTrace().TraceSysError();		}
#define traceRel(X)			{ CEasyTrace().TraceResult(#X,X);	}
#define traceClear(X)		{ ::SendMessage(::FindWindow(NULL,TRACE_WND_TEXT),WM_COMMAND,3,NULL); }				
#define traceStart(X)		{ CEasyTrace::IsReceive(TRUE);		}
#define traceStop(X)		{ CEasyTrace::IsReceive(FALSE);		}
//��������������������������������������������������������������������������������





class CTraceWnd
{
public:
	~CTraceWnd(){ ::DestroyWindow(::FindWindow(NULL,TRACE_WND_TEXT)); }
	
	/****************************************************************************
	�������к�
	-1 ---- ���������
	1   --- ��������ֵΪ1 
	0    ---��ȡ����
	****************************************************************************/
	static TCHAR* GetIndex(int opt=0)
	{
#define INDDEX_NUM 4  //�����λ��

		static int num=1;
		static TCHAR buf[11]={0};
		static TCHAR buf2[11]={0};
		static BOOL IsOutIndex=-10;
		if (IsOutIndex==-10) IsOutIndex=GetProfileInt(TRACE_WND_TEXT,_T("m_IsIndex"),TRUE);

		if (opt==-1) IsOutIndex=FALSE;
		if (opt== 1) {IsOutIndex=1; num=1;}
		if (!IsOutIndex) {  buf[0]=NULL; return buf;}

		
		_itot(num++,buf,10);//int ת�ı�
		int len=_tcslen(buf);
		int count=INDDEX_NUM-len;
		if (count>0)
		{
			for (int i=0; i< count;i++)	{buf2[i]=' ';}
			_tcscpy(buf2+count,buf);
			return buf2;
		}
		else
			return buf;
	}


	static BOOL IsName(BOOL var=-1)
	{
		static int state=-10;
		if (state==-10)		state=GetProfileInt(TRACE_WND_TEXT,_T("m_IsName"),TRUE);
		if(var!=-1) state=var;
		return state;
	}
	
	/****************************************************************************
	������ַ���������
	****************************************************************************/
	static void CTraceWnd::PrintString(const TCHAR* OutputStr)
	{
		//Ѱ���Ѵ������������
		HWND hWnd=::FindWindow(NULL,TRACE_WND_TEXT);
		
		//��������ڣ��򴴽����ڲ���ʼ������
		if ( !hWnd )
		{
			//�������������Ϊ�˳����˳��ǵ�������������
			//�����ļ��͹رմ��ڣ�����֮��û�������ô���
			static CTraceWnd ForGetCloseMessage;
			
			//��������
			hWnd=(HWND)WindowProc(NULL,WM_CREATE_WND,NULL,NULL);
			if (!hWnd)	return;
		}
		
		//���ַ������������
		::SendMessage(hWnd,EM_REPLACESEL,FALSE,(LPARAM)OutputStr);	
	}

	/****************************************************************************
	//���洰������
	****************************************************************************/
	static void SaveConfig(BOOL	m_IsAutoWidth,BOOL m_IsAutoHide,BOOL m_IsHide,BOOL m_IsContinue,BOOL m_IsName,BOOL m_IsIndex,HWND hWnd)
	{
		TCHAR buf[11];
		_itot(m_IsAutoWidth,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("m_IsAutoWidth"),buf);
		_itot(m_IsAutoHide ,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("m_IsAutoHide"), buf);
		_itot(m_IsHide	   ,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("m_IsHide"),	  buf);
		_itot(m_IsContinue ,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("m_IsContinue"), buf);
		_itot(m_IsName	   ,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("m_IsName"),     buf);
		_itot(m_IsIndex	   ,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("m_IsIndex"),    buf);
		if (!m_IsHide)
		{
			RECT rect; 
			::GetWindowRect(hWnd,&rect);
			_itot(rect.left,buf,10);			WriteProfileString(TRACE_WND_TEXT,_T("x"),buf);
			_itot(rect.top,buf,10);				WriteProfileString(TRACE_WND_TEXT,_T("y"),buf);
			_itot(rect.right-rect.left,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("cx"),buf);
			_itot(rect.bottom-rect.top,buf,10);	WriteProfileString(TRACE_WND_TEXT,_T("cy"),buf);
		}
	}
	
	static void ReShowWindow(HWND hWnd)
	{
		int x=GetProfileInt(TRACE_WND_TEXT,_T("x"),0);
		int y=GetProfileInt(TRACE_WND_TEXT,_T("y"),0);
		int cx=GetProfileInt(TRACE_WND_TEXT,_T("cx"),200);
		int cy=GetProfileInt(TRACE_WND_TEXT,_T("cy"),600);
		::SetWindowPos(hWnd,HWND_TOPMOST,x,y,cx,cy,SWP_NOACTIVATE);
	}

	static void HideWindowToLeftScreen(HWND hWnd)
	{
		::SetWindowPos(hWnd,HWND_TOPMOST,-100,0,104,GetSystemMetrics(SM_CYSCREEN),NULL);
	}


	/****************************************************************************
	Edit Window�Ĵ��ڹ���
	****************************************************************************/
	static LRESULT CALLBACK CTraceWnd::WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
	{
		static WNDPROC	m_OldWndProc	=	NULL;
		static int		m_WindowWidth	=	200;	//���ڿ��
		static HWND		StaticWnd		=	NULL;	//
		static BOOL		m_IsReceive		=	TRUE;	//�Ƿ��������
		static BOOL		m_IsContinue	=	TRUE;	//������ַ���ʱ�Ƿ����֮ǰ������
		static BOOL		m_IsAutoWidth	=	TRUE;	//�Ƿ�����ַ��������Զ��������ڿ��
		static BOOL		m_IsAutoHide	=	FALSE;	//�Ƿ��Զ�����
		static BOOL		m_IsHide		=	FALSE;	//�Ƿ�����
		static BOOL		m_IsName		=	TRUE;	//�Ƿ��Զ����ϱ�����
		static BOOL		m_IsIndex		=	TRUE;	//����к�����
	
		switch(message)
		{
		case WM_CREATE_WND: //����Edit���� ,�����Զ�����Ϣ
			{
				//��ȡ��������
				int x=GetProfileInt(TRACE_WND_TEXT,_T("x"),0);
				int y=GetProfileInt(TRACE_WND_TEXT,_T("y"),0);
				int cx=GetProfileInt(TRACE_WND_TEXT,_T("cx"),200);
				int cy=GetProfileInt(TRACE_WND_TEXT,_T("cy"),600);
				m_IsAutoWidth	=GetProfileInt(TRACE_WND_TEXT,_T("m_IsAutoWidth"),1);
				m_IsAutoHide	=GetProfileInt(TRACE_WND_TEXT,_T("m_IsAutoHide"),0);
				m_IsHide		=GetProfileInt(TRACE_WND_TEXT,_T("m_IsHide"),FALSE);
				m_IsContinue	=GetProfileInt(TRACE_WND_TEXT,_T("m_IsContinue"),TRUE);
				m_IsName		=GetProfileInt(TRACE_WND_TEXT,_T("m_IsName"),TRUE);
				IsName(m_IsName);
				m_IsIndex		=GetProfileInt(TRACE_WND_TEXT,_T("m_IsIndex"),TRUE);
				GetIndex( m_IsIndex ? 1 : -1);
				m_IsReceive		=TRUE; //����Ҫ�����������ļ�

				//������Ļ���µ���
				int SrcCx=GetSystemMetrics(SM_CXSCREEN);
				int SrcCy=GetSystemMetrics(SM_CYSCREEN);
				if (x<0 || x>=SrcCx) x=0;
				if (y<0 || y>=SrcCy) y=0;
				if (cx<100) cx=100;
				if (cy<40)	cy=40;
				if (x+cx>SrcCx) x=SrcCx-cx;
				if (y+cy>SrcCy) y=SrcCy-cy;

				//�������������״̬���޸�x��ֵ
				if (m_IsHide)
				{
					x=-cx+4;
					y=0;
					cy=GetSystemMetrics(SM_CYSCREEN);
				}

				HWND hWnd=CreateWindowEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW,TEXT("Edit"),TRACE_WND_TEXT ,
					WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL|
					ES_AUTOHSCROLL|ES_WANTRETURN|ES_MULTILINE , 
					x,y,cx,cy,
					NULL,NULL,NULL,NULL);
				
				if (hWnd==NULL)	return NULL;
				
				//�����˵�,����ӵ�����
				HMENU hMenu = CreateMenu() ;
				AppendMenu(hMenu,MF_STRING, 5,	_T("���ص���Ļ���") )	;
				AppendMenu(hMenu,MF_STRING, 3,	_T("��մ���"));
				//-----------------sub menu
				HMENU hSubMenu=CreateMenu() ;
				AppendMenu(hSubMenu,MF_STRING, 6,  _T("�Զ�����") );
				AppendMenu(hSubMenu,MF_STRING, 7,  _T("��������") );
				AppendMenu(hSubMenu,MF_STRING, 4,  _T("�����ַ��������Զ��������ڿ��" ));
				AppendMenu(hSubMenu,MF_STRING, 9,  _T("�����������"));
				AppendMenu(hSubMenu,MF_STRING, 10, _T("���ʱ�Զ���ӱ�����"));
				AppendMenu(hSubMenu,MF_STRING, 11, _T("���ʱ����к�����"));
				AppendMenu(hSubMenu,MF_STRING, 8,  _T("����"));
				//-----------------sub menu
				AppendMenu(hMenu, MF_POPUP,UINT(hSubMenu), _T("ѡ��")) ;
				::SetMenu(hWnd,hMenu);
			

				//�޸�edit��������ַ�����
				::SendMessage(hWnd,EM_SETLIMITTEXT,1000000, 0);
				
				//���Edit
				::SendMessage(hWnd,WM_SETTEXT,NULL,(LPARAM)_T(""));	
				
				// �����������
				LOGFONT LogFont;
				::memset(&LogFont, 0, sizeof(LOGFONT));
				lstrcpy(LogFont.lfFaceName,_T("Fixedsys"));
				LogFont.lfHeight = -12; // �����С
				LogFont.lfCharSet = GB2312_CHARSET;
				// ��������
				HFONT hFont=CreateFontIndirect(&LogFont); 
				// ��������
				::SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);
				
				//��hWnd��Ĭ�ϴ��ڹ����滻ΪWindowProc,����Ĭ�Ϻ������̵ĺ���ָ��
				m_OldWndProc=(WNDPROC)SetWindowLong(hWnd,GWL_WNDPROC, (LONG)WindowProc);
				if (!m_OldWndProc)
					return NULL;
				
				//��ʾʱ�޽���
				::ShowWindow(hWnd,SW_SHOWNOACTIVATE);
				
				//���û�п����Զ����ز����ִ�������״̬���򵯳���ʾ����
				if (!m_IsAutoHide && m_IsHide)
				{
					StaticWnd=CreateWindowEx(WS_EX_TOPMOST,_T("Static"),
						_T("Trace������������Ļ���\n�����Ļ��߿ɻָ�����"),
						WS_POPUP|WS_VISIBLE|WS_BORDER
						|SS_CENTER ,
						0,y,200,40,NULL,NULL,NULL,NULL); 

					SetTimer(hWnd,2,2000,NULL);
				}

				return (LRESULT)hWnd;
			}
			break;

		case WM_TIMER:
			{
				if (wParam==2) //�ر���ʾ����
				{
					::DestroyWindow(StaticWnd); 
					break;
				}

				if ( !m_IsHide && ::GetForegroundWindow()!=hWnd)
				{
					//���ش���
					m_IsHide=true;
					HideWindowToLeftScreen(hWnd);
				}
			}
			break;

		case WM_ACTIVATE:
			{
				if (LOWORD(wParam)==WA_INACTIVE ) //����ʧȥ����
				{
					if (m_IsAutoHide)
					{
						::SetTimer(hWnd,1,200,NULL); //�ӳ�200ms��hide
					}
				}
			}
			break;

		case WM_NCLBUTTONDOWN:
			{
				if (m_IsHide)
				{
					m_IsHide=false;
					ReShowWindow(hWnd); //������ʾ����
					// save m_IsHide 
					SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
				}
				else
				{
					//����Ĭ�ϲ�������ı䴰�ڵ�λ�ô�С
					int rel=::CallWindowProc(m_OldWndProc, hWnd, message, wParam, lParam); 

					if (wParam==HTLEFT	 ||wParam==HTTOP		||wParam==HTRIGHT		||wParam==HTBOTTOM || wParam==HTCAPTION ||
						wParam==HTTOPLEFT||wParam==HTTOPRIGHT	||wParam==HTBOTTOMLEFT	||wParam==HTBOTTOMRIGHT)
					{
						//���洰������
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}

					return rel;
				}
			}
			break;

		case EM_REPLACESEL:
			{
				if(!m_IsReceive)	return 0;

				if (m_IsContinue)
				{
					//���ָ�����
					::SendMessage(hWnd,EM_SETSEL,4294967290, 4294967290);	

					//����Ĭ�Ϲ�������ַ�����edit ����
					::CallWindowProc(m_OldWndProc, hWnd, message, wParam, lParam); 
				}
				else
				{
					::SendMessage(hWnd,WM_SETTEXT,NULL,lParam);
				}
				
				
				//�������Զ����ع���
				if (m_IsAutoHide)
				{
					if (m_IsHide)
					{
						m_IsHide=false;
						ReShowWindow(hWnd);
					}

					//����AUTO_HIDE_TIME�����ش���
					::SetTimer(hWnd,1,AUTO_HIDE_TIME,NULL);
				}

				//�������Զ��������ڵĿ�ȹ���
				if (m_IsAutoWidth)
				{
					if (!m_IsHide)
					{
						int MinPos,MaxPos;
						GetScrollRange(hWnd,SB_HORZ, &MinPos,&MaxPos);
						if (m_WindowWidth<MaxPos+40)
						{
							m_WindowWidth=MaxPos+40;
							RECT rect; 
							::GetWindowRect(hWnd,&rect);
							::SetWindowPos(hWnd,NULL,0,0,m_WindowWidth,rect.bottom-rect.top,SWP_NOMOVE|SWP_NOACTIVATE);
						}
					}
				}
			}
			break;

		case WM_DESTROY:
			{
				//ɾ������
				DeleteObject( (HFONT)::SendMessage(hWnd,WM_GETFONT,0,0) );
				//ɾ���˵�
				DestroyMenu(GetMenu(hWnd));

				//���ó������е�Ŀ¼�������ļ�����Ŀ¼��ͬ
				//�����ǵĳ������������ʱ�����Ĺ���Ŀ¼�ʹ������ǡ�
				//�������Ŀ¼����ͬ�ġ�����������Ҫ��Ŀ¼�Ļ���
				TCHAR AppPath[MAX_PATH];
				int nlen=GetModuleFileName(NULL,AppPath,MAX_PATH);
				while(AppPath[--nlen]!='\\');  
				AppPath[nlen]='\0';
				SetCurrentDirectory(AppPath);	
				
				//��ȡ���ڵ��ַ���
				int len=::SendMessage(hWnd,WM_GETTEXTLENGTH,0,0);
				TCHAR* WindowText=new TCHAR[(len+1)*sizeof(TCHAR)];
				::GetWindowText(hWnd,WindowText,len+1);  //GetWindowText���Զ����NULL������
				
				//�����ڱ��������ļ�DebugData.txt,	
				FILE* fp=fopen("DebugData.txt","wb");	
				
#ifdef	_UNICODE//���������unicode������תΪansi�ٱ���
				//len��Դ�ַ����ĳ��� len=_tcslen(WindowText)
				//(len+1)*2��Դ�ַ���ռ�õ��ڴ�ռ䣬����NULL������
				//nChars����ת�����ַ�������������NULL����nChars=_tcslen(buffer)
				//wcstombs���Զ����NULL������
				char* buffer = new char[(len+1)*2];
				setlocale(LC_ALL,".936");
				int nChars = wcstombs(buffer,WindowText,(len+1)*2);
				setlocale(LC_ALL,"C");
				fwrite(buffer,1,nChars,fp);
				delete buffer;
#else	
				fwrite(WindowText,1,len,fp);
#endif
				delete[] WindowText;
				fclose(fp);
			}
			break;

		case WM_CLOSE:
			if (m_IsIndex) 
				GetIndex(TRUE);
			::DestroyWindow(hWnd);
			break;

		case WM_KEYDOWN:
			{
				//select all
				if (wParam=='A' && GetKeyState(VK_CONTROL)<0)
					::SendMessage(hWnd,EM_SETSEL,0,-1);
			}
			break;

		case WM_INITMENUPOPUP:
			{
				HMENU hSubMenu=(HMENU)wParam;
				CheckMenuItem(hSubMenu,6, MF_BYCOMMAND | m_IsAutoHide ? MF_CHECKED : MF_UNCHECKED );
				CheckMenuItem(hSubMenu,7, MF_BYCOMMAND | m_IsReceive  ? MF_CHECKED : MF_UNCHECKED);
				CheckMenuItem(hSubMenu,4, MF_BYCOMMAND | m_IsAutoWidth? MF_CHECKED : MF_UNCHECKED);
				CheckMenuItem(hSubMenu,9, MF_BYCOMMAND | m_IsContinue ? MF_CHECKED : MF_UNCHECKED);
				CheckMenuItem(hSubMenu,10,MF_BYCOMMAND | m_IsName     ? MF_CHECKED : MF_UNCHECKED);
				CheckMenuItem(hSubMenu,11,MF_BYCOMMAND | m_IsIndex    ? MF_CHECKED : MF_UNCHECKED);
			}
			break;

		case WM_COMMAND:
			{
				switch(wParam)
				{
				case 3:  //�������
					{
						if (m_IsIndex) 
							GetIndex(1);
						::SendMessage(hWnd,WM_SETTEXT,NULL,(LPARAM)_T(""));	
					}
					break;
				case 5:  //hide window int the left of the screen
					{
						//���ش���
						m_IsHide=true;
						HideWindowToLeftScreen(hWnd);

						//���ش��ں�Ҫ��ֹ�Զ�����
						m_IsAutoHide=false;
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				case 4:  //modify the Auto Adjust window width option
					{
						m_IsAutoWidth=!m_IsAutoWidth;
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				case 6:  //modify the Auto hide window option
					{
						m_IsAutoHide=!m_IsAutoHide;
						if(!m_IsAutoHide) ::KillTimer(hWnd,1);
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				case 7:  //modify the receivd date option
					{
						if(m_IsReceive)
							::SendMessage(hWnd,EM_REPLACESEL,FALSE,(LPARAM)_T("\r\nע�⣺��ǰ���ڽ�ֹ��������״̬\r\n"));	

						m_IsReceive=!m_IsReceive;
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				case 8:  
					::SendMessage(hWnd,WM_SETTEXT,NULL,(LPARAM)_T("���������£�\r\nhttp://blog.csdn.net/jacky_qiu/article/details/5986089\r\nEmail: qiujiejia@gmail.com"));	
					break;
				case 9:  //modify the receivd date option
					{
						m_IsContinue=!m_IsContinue;
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				case 10:  
					{
						m_IsName=!m_IsName;
						IsName(m_IsName);
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				case 11:  
					{
						m_IsIndex=!m_IsIndex;
						GetIndex( m_IsIndex ? 1 : -1);
						SaveConfig(m_IsAutoWidth,m_IsAutoHide,m_IsHide,m_IsContinue,m_IsName,m_IsIndex,hWnd);
					}
					break;
				}
			}
			break;

		default:
			return ::CallWindowProc(m_OldWndProc, hWnd, message, wParam, lParam); 
		}
		return 0;
	}
};





/****************************************************************************
CEasyTrace
****************************************************************************/
class CEasyTrace
{
private:

#ifdef	_UNICODE	
	std::wostringstream m_str;
#else	
	std::ostringstream m_str;
#endif	

public:
	void add(){}		//��trace���������ʱ����ô˺���
	void name(){}		//��trace���������ʱ����ô˺���

	//��ʼ��һ��ostringstream������������к�
	CEasyTrace() {	 m_str<<CTraceWnd::GetIndex()<<_T("  ");    }

	//����ʱ���ַ��������Edit����
	~CEasyTrace()
	{
		if(!IsReceive()) return;

		m_str<<_T("\r\n");   //��������С�
#ifdef	_UNICODE	
		CTraceWnd::PrintString(std::wstring(m_str.str()).c_str());
#else	
		CTraceWnd::PrintString(std::string(m_str.str()).c_str());
#endif	
	}

	//���������
	void name(char* pStr)
	{
		if(!CTraceWnd::IsName()) return;

		if (strchr(pStr,'\"')!=NULL) return;
#ifdef	_UNICODE	
		wchar_t* UnicodeStr=AnsiToUnicode(pStr);
		m_str<<UnicodeStr<<_T("=");
		delete[] UnicodeStr;
#else
		m_str<<pStr<<_T("=");
#endif
	}

	//���һ�����͵ı���
	template <class T> void add(T t)  {	m_str<<t<<_T("\t"); }

	//���point
	void add(POINT point) {	m_str<<(_T("("))<<point.x<<(_T(","))<<point.y<<(_T(")\t")); }

	//���rect
	void add(RECT rect)	{ m_str<<(_T("("))<<rect.left<<(_T(","))<<rect.top<<(_T(","))<<rect.right<<(_T(","))<<rect.bottom<<(_T(")-("))<<(rect.right-rect.left)<<(_T(","))<<(rect.bottom-rect.top)<<(_T(")\t"));	}

	//���������Ϣ
	void add(HWND hWnd) 
	{
		if (!::IsWindow(hWnd))	{ m_str<<_T("Invalid Window");	return;	}
		
		TCHAR WindowText[40];	::SendMessage(hWnd,WM_GETTEXT,(WPARAM)sizeof(WindowText)/sizeof(TCHAR),(LPARAM)WindowText);	
		TCHAR ClassName[40];	::GetClassName(hWnd,ClassName,sizeof(ClassName)/sizeof(TCHAR));
		RECT  rect;				::GetWindowRect(hWnd,&rect);

		m_str<<_T("��������������������������")
			 <<_T("\r\n\tWindow Text:  \"")	 <<WindowText
			 <<_T("\"\r\n\tClass Name:  \"") <<ClassName
			 <<_T("\"\r\n\tHandle:  0x")	 <<HWND(hWnd)
			 <<_T("\r\n\trect:\t");			 
		add(rect);
		m_str<<_T("\r\n\t��������������������������������");	
	}
	
	//���ϵͳ�������
	void TraceSysError()	{ m_str<<_T("System Error Codes:")<<GetLastError();	}

	//�������ִ�еĽ��
	void TraceResult(char* pStr,BOOL IsOk)  
	{
#ifdef	_UNICODE
		wchar_t* UnicodeStr=AnsiToUnicode(pStr);
		m_str<<UnicodeStr;
		delete[] UnicodeStr;
#else
		m_str<<pStr;
#endif
		m_str<<_T(" : ")<<( IsOk==TRUE ? _T(" Successful(true)") : _T("Failed(false)") ); 
	}

	//���MFC�� CPoint CRect CString
#ifdef __AFXWIN_H__
	void add(CPoint point)	{ add((POINT)point);}
	void add(CRect rect)	{ add((RECT)rect);}
	void add(CString str)	{ add((LPTSTR)(LPCTSTR)str); }
#endif 

	//ansi ת unicode ������Ҫɾ��ָ��
	wchar_t* AnsiToUnicode(char* AnsiStr)
	{
		UINT len=strlen(AnsiStr); 
		wchar_t* UnicodeStr= new wchar_t[len+1];
		setlocale(LC_ALL,".936");
		int num=mbstowcs(UnicodeStr,AnsiStr,len+1);
		setlocale(LC_ALL,"C");
		if(-1==num)	*UnicodeStr=NULL;
		return UnicodeStr;
	}

	//�����ʽ���ַ���������sprintf
	static void Tracef(LPCTSTR lpszFormat, ...)
	{
		va_list args; va_start(args, lpszFormat);	//begin
		TCHAR szBuffer[1024];
		int rel=_vsntprintf(szBuffer, sizeof(szBuffer)/sizeof(TCHAR)-1, lpszFormat, args);
		if (rel==-1) szBuffer[sizeof(szBuffer)/sizeof(TCHAR)-1]=NULL;
		va_end(args);		//end
		CEasyTrace().add(szBuffer); //����ַ���
	}

	/****************************************************************************
	���úͻ�ȡ����״̬ 
	-1��	��ȡ����״̬ 
	TRUE��	����Ϊ����״̬ 
	FALSE��	��ֹ���ա�
	****************************************************************************/
	static BOOL IsReceive(BOOL var=-1)
	{
		static BOOL state=true;
		if(var!=-1) state=var;
		return state;
	}

};


#endif // #define TRACE_WINDOW
#endif // #define _EASY_TRACE_H_
