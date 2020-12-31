#include "common.h"
#include "resource.h"

HHOOK g_hHook								= NULL;				// ���ڹ��Ӿ��
//HHOOK g_hGetMessageHook						= NULL;				// WH_GETMESSAGE HOOK ���

ULONG_PTR m_gdiplusToken					= NULL;				// GDI+ ȫ����Դ���

typedef struct _MetroUIData {
	WNDPROC oldWndProc;					// ԭ���Ĵ��ڹ���
	LONG lStyle;						// ԭ���Ĵ�����ʽ
	LONG lStyleEx;						// ԭ������չ������ʽ
	BOOL bTracking;						// �����������Ϣ�¼�
	int nMouseDownNCHTTEST;				// ��갴��ʱ���ڵ�����
	int nNcHitTestCode;					// ����ƶ�ʱ���ڵ�����
	HBRUSH hClientBrush;				// �ͻ���������ˢ
	map< HWND,HBRUSH > mapControlBrush;	// �ؼ��ı���HBRUSH
}MetroUIData,*PMetroUIData;

enum NCHITTEST { NCHTCLOSEBUTTON = 190,NCHTMAXBUTTON,NCHTMINBUTTON,NCHTMENUBUTTON };// ����ڱ�������λ��

Gdiplus::Image * m_pPngMenuNormal			= NULL;				// �˵���ť
Gdiplus::Image * m_pPngMenuOver				= NULL;
Gdiplus::Image * m_pPngMenuDown				= NULL;

Gdiplus::Image * m_pPngMinNormal			= NULL;				// ��С����ť
Gdiplus::Image * m_pPngMinOver				= NULL;
Gdiplus::Image * m_pPngMinDown				= NULL;

Gdiplus::Image * m_pPngMaxNormal			= NULL;				// ��󻯰�ť
Gdiplus::Image * m_pPngMaxOver				= NULL;
Gdiplus::Image * m_pPngMaxDown				= NULL;

Gdiplus::Image * m_pPngRestoreNormal		= NULL;				// �ָ���ť
Gdiplus::Image * m_pPngRestoreOver			= NULL;
Gdiplus::Image * m_pPngRestoreDown			= NULL;

Gdiplus::Image * m_pPngCloseNormal			= NULL;				// �رհ�ť
Gdiplus::Image * m_pPngCloseOver			= NULL;
Gdiplus::Image * m_pPngCloseDown			= NULL;

LRESULT CALLBACK WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam );				// �µĴ��ڹ���
LRESULT CALLBACK GetMsgProc( int code,WPARAM wParam,LPARAM lParam );

void SetWindowRegion( HWND hWnd );															// ���ô�������

void OnNcPaint( HWND hWnd,WPARAM wParam );													// ���������ƺ���
void OnPaint( HWND hWnd );																	// �ͻ������ƺ���

HBRUSH GetBrush( PMetroUIData pMetroUIData,HWND hWnd,WPARAM wParam,LPARAM lParam );								// ���ڳߴ�ı�ʱ���������ÿͻ����͸����ӿؼ��ı�����ˢ

void SetWindowRegion( HWND hWnd )
{
	SetWindowRgn( hWnd,NULL,FALSE );

	RECT rc;
	GetWindowRect( hWnd,&rc );
	HRGN hRgn = CreateRoundRectRgn( 0,0,rc.right - rc.left,rc.bottom - rc.top,4,4 );
	// ����Ҫ��SetWindowRgn���һ������һ��Ҫ����ΪTRUE����ͻ����������Ӱ�������Invalidateˢ�¿ͻ�������˸���ر�����
	SetWindowRgn( hWnd,hRgn,TRUE );
	// ʹ����HRGN��һ��Ҫ�ͷŸ�GDI����
	::DeleteObject( hRgn );
}

LRESULT CALLBACK WindowProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
	LRESULT lResult = 0;
	PMetroUIData pMetroUIData = (PMetroUIData)GetProp( hWnd,_T("MetroUI_Prop") );
	switch( uMsg )
	{
	case WM_INITDIALOG:
		lResult = ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam );
		// ��Ϊ�����ڿͻ��������Ǳ仯�ģ����ǹ̶��ģ� ���Ե����ڳߴ�ı�ʱ�������ӿؼ��ı���ҲҪ��֮�ı䣬���Բ��ܼ��������仰��
		// ͨ���ȶ�QQ����QQ�ı������Ǳ仯�ģ�QQ�ĳߴ��ڸı�ʱֻ�б仯�����һ���ֳߴ�����������˱�����������ǰ����һ���ֳߴ����������ǰ�ı�����
		// ����������ⲻ�ý�����ͻ������������ųߴ�仯�Ĵ��ڣ���˸�Ƚ����������ý����
		//SetWindowLong( hWnd,GWL_STYLE,GetWindowLong( hWnd,GWL_STYLE ) | WS_CLIPCHILDREN );
		SetWindowRegion( hWnd );
		return lResult;
	case WM_NCDESTROY:
		{
			// ��������ʱ���ûؾɵĴ��ڹ���
			SetWindowLong( hWnd,GWL_WNDPROC,(LONG)(pMetroUIData->oldWndProc) );
			// ����һ�ξɵĴ��ڹ���
			lResult = ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam );
			// �ͷſͻ����ı���HDC
			if( pMetroUIData->hClientBrush != NULL )
			{
				DeleteObject( pMetroUIData->hClientBrush );
				pMetroUIData->hClientBrush = NULL;
			}
			// �ͷ�Ϊ���пؼ�������HBRUSH
			map< HWND,HBRUSH >::iterator p = pMetroUIData->mapControlBrush.begin();
			while( p != pMetroUIData->mapControlBrush.end() )
			{
				DeleteObject( (HBRUSH)(p->second) );
				p++;
			}
			pMetroUIData->mapControlBrush.clear();
			// �ͷ�new������pMetroUIData
			delete pMetroUIData;
			// �Ƴ����õĴ��ڱ��
			RemoveProp( hWnd,_T("MetroUI_Prop") );
			return lResult;
		}
		break;
	case WM_NCCALCSIZE:
		break;
		// ����Ҫ��
		// ��CDialog���н����û���Ĵ���CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);ȥ���������ı�lpncsp->rgrc[0]�����ֵ
		// Ч���ǶԻ���ı������ͱ߿�û���ˣ�
		// ������ı�lpncsp->rgrc[0]�����ֵ����lpncsp->rgrc[0].top���ϸ߶�36������ʵ�ֵ�Ч���Ǳ������ͱ߿��ֳ�����
		// ��������Ҳ���ǲ����û����CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);���ĺô��Ǳ������ھ���������¾������Լ����ƣ�
		// ����ϵͳ����С����󻯹رհ�ť�Ͳ�������� ��������һ�������ǣ����ļ�ѡ��Ի���ĸ߶ȱ�С�ˣ������ȡ����ť����ʾ��������
		// ��õ��������ǲ����������Ϣ�����������ػ���״̬�ı���ȥ�ػ档  
		//OutputDebugString( _T("WM_NCCALCSIZE\r\n") ); 
		if( wParam == TRUE )
		{
			LONG lStyle = GetWindowLong( hWnd,GWL_STYLE );
			bool bResize = ( ( lStyle & WS_THICKFRAME ) == WS_THICKFRAME );
			NCCALCSIZE_PARAMS * lpncsp = (NCCALCSIZE_PARAMS*)lParam;
			lpncsp->rgrc[0].left += bResize ? GetSystemMetrics( SM_CXSIZEFRAME ) : GetSystemMetrics( SM_CXFIXEDFRAME );
			lpncsp->rgrc[0].top += GetSystemMetrics( SM_CYCAPTION ) + ( bResize ? GetSystemMetrics( SM_CYSIZEFRAME ) : GetSystemMetrics( SM_CYFIXEDFRAME ) );
			lpncsp->rgrc[0].bottom -= bResize ? GetSystemMetrics( SM_CYSIZEFRAME ) : GetSystemMetrics( SM_CYFIXEDFRAME );
			lpncsp->rgrc[0].right -= bResize ? GetSystemMetrics( SM_CXSIZEFRAME ) : GetSystemMetrics( SM_CXFIXEDFRAME );
		}
		// ֱ�ӷ��أ����û����д�����ᡣ����Ϊ�˴���һ������HOOK���ڹ��̣������ε�����Ĵ���WM_NCCALCSIZE��Ϣ�Ĺ��̣����밲װWH_CBT����
		// �����װWH_CALLWNDPROC�ǲ��еģ�ϵͳ���ǻ���뵽CDialog���OnNcCalcSize��������У�����ԭ��û�������
		return 0L;
	case WM_NCACTIVATE:
		// ���� lResult = ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam ); ����lResultֵ����Ϊ1����ôֱ��return 1;��������ԭ���Ĵ��ڹ��̣��Ͳ�����ˢ��ʱ����ϵͳ������
		// ע�������ǲ��ܷ���0�� �������ֵ������ӶԻ�����MODAL״̬������
		//lResult = ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam );
		{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_NCACTIVATE %d %d\r\n"),lResult,i++);OutputDebugString( buf );}
		//OnNcPaint( hWnd,1 );
		return 1L;
	case WM_ACTIVATEAPP:
		break;
	case WM_EXITSIZEMOVE:
		// ���ش���ϢĿ����Ϊ�˽�� �� �����ڵ��� SetWindowRgn �����Ӵ��ڹ�����ʱ����û��Ӧ�����Ǹ�������¼������ָ��ʺܸߣ���
		::RedrawWindow( hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_FRAME|RDW_ALLCHILDREN);
		break;
	case WM_SIZING:
		break;
	case WM_SIZE:
		//{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_SIZE %d %d\r\n"),wParam,i++);OutputDebugString( buf );}
		lResult = ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam );
		if( wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED )
		{
			// �������ÿͻ���������ˢ�͸����ӿؼ��ı�����ˢ  
			RECT rc,rcControl;
			GetClientRect( hWnd,&rc );
			LONG lStyle = GetWindowLong( hWnd,GWL_STYLE );
			bool bResize = ( ( lStyle & WS_THICKFRAME ) == WS_THICKFRAME );
			// ��rc.top���ϱ��Ʊ������߶��ټ�2�����أ�Ŀ�����ڻ��ƿͻ�������ʱ���ֺͱ������ı�������һ�¡�
			rc.top -= GetSystemMetrics( SM_CYCAPTION ) + ( bResize ? GetSystemMetrics( SM_CYSIZEFRAME ) : GetSystemMetrics( SM_CYFIXEDFRAME ) ) - 2;
			// �������ô��ڿͻ�������HDC�����������ӿؼ��ı�����ˢ
			HDC hDC = ::GetDC( hWnd ); 
			if( hDC )
			{
				// ����ϴ��Ѿ�������HDC������������
				if( pMetroUIData->hClientBrush != NULL )
				{
					DeleteObject( pMetroUIData->hClientBrush );
					pMetroUIData->hClientBrush = NULL;
				}
				HDC hMemDC = ::CreateCompatibleDC( hDC );
				HBITMAP hBitmap = ::CreateCompatibleBitmap( hDC,rc.right - rc.left,rc.bottom - rc.top );
				HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC,hBitmap );
				GradientFillRect( hMemDC,rc,0xFDE7C5,0xffffff,true );
				pMetroUIData->hClientBrush = ::CreatePatternBrush( hBitmap );
				// �ͻ��������ı���HDC�����ɣ���ʼΪ�����ӿؼ����¹��챳����ˢ 
				map< HWND,HBRUSH >::iterator p = pMetroUIData->mapControlBrush.begin();
				while( p != pMetroUIData->mapControlBrush.end() )
				{
					// �����ӿؼ��ı�����ˢ֮ǰ�Ƚ��ÿؼ���ǰ�ı�����ˢ�ͷ� 
					DeleteObject( (HBRUSH)(p->second) );

					GetWindowRect( (HWND)(p->first),&rcControl );
					ScreenToClient( hWnd,&rcControl );

					HDC hMemDC2 = ::CreateCompatibleDC( hMemDC );
					HBITMAP hBitmap2 = ::CreateCompatibleBitmap( hMemDC,rcControl.right - rcControl.left,rcControl.bottom - rcControl.top );

					HBITMAP hOldBitmap2 = (HBITMAP)SelectObject( hMemDC2,hBitmap2 );
					BitBlt( hMemDC2,0,0,rcControl.right - rcControl.left,rcControl.bottom - rcControl.top,hMemDC, rcControl.left, rcControl.top, SRCCOPY);

					HBRUSH hBrush = ::CreatePatternBrush( hBitmap2 );
					pMetroUIData->mapControlBrush[(HWND)(p->first)] = hBrush;

					SelectObject( hMemDC2,hOldBitmap2 );
					DeleteObject( hBitmap2 );
					DeleteDC( hMemDC2 );

					p++;
				}
				//����ǰ���豸ѡ��ȥ��������ܻᵼ��GDI��Դ�ͷų������⡣
				SelectObject( hMemDC,hOldBitmap );
				DeleteObject( hBitmap );
				DeleteDC( hMemDC );
				ReleaseDC( hWnd,hDC );
			}
			// ���ڳߴ�ı��ˣ��������ô���REGION
			SetWindowRegion( hWnd );
			InvalidateRect( hWnd,NULL,TRUE );
		}
		return lResult;
	case WM_ERASEBKGND:
		// ��ʵ��ͨ�Ի����ﲻ��Ҫ������ı����ػ�������return TRUE;���ɡ������ļ�ѡ��Ի������������һ��ѡ��鿴�ļ����͵�ToolBarWindow32��������ϵͳ��ɫ�ġ�
		// ֻ�ý��ͻ����ı����ػ�һ�¾Ͳ�������Ǹ�ϵͳ��ɫ�ı����ˡ�
		if( pMetroUIData->hClientBrush )
		{
			// �����ȹ���õĿͻ�������HDC���ͻ����ı����ػ���Ϣ
			RECT rc;
			GetClientRect( hWnd,&rc );
			HDC hMemDC = ::CreateCompatibleDC( (HDC)wParam );
			HBITMAP hBitmap = ::CreateCompatibleBitmap( (HDC)wParam,rc.right - rc.left,rc.bottom - rc.top );
			HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC,hBitmap );
			::FillRect( hMemDC,&rc,pMetroUIData->hClientBrush );
			::BitBlt( (HDC)wParam,0,0,rc.right - rc.left,rc.bottom - rc.top,hMemDC,0,0,SRCCOPY );
			SelectObject( hMemDC,hOldBitmap );
			DeleteObject( hBitmap );
			DeleteDC( hMemDC );
			return TRUE;
		}
		break;
	case WM_NCPAINT:
		OnNcPaint( hWnd,wParam );
		return 0L;
	case WM_PAINT:
		OnPaint( hWnd );
		return 0L;
	// CTabCtrl �и����ĵĵط���������ˢ�޷��ı䣬����OnCtlColor���У�����OnEraseBkgnd���У�ֻ��һ���취������OnPaint ��ȫ�Լ������С�
	// ����һ���취������SetWindowRgn����TABCTRL����Ч���򣬿ɻ��⣬����������ػ�Ļ���ϵͳ��TABCTRL�ı߱����ߵȻ��Ǻ��ѿ�������Ͳ��ٿ����ˡ�
	//case WM_CTLCOLORDLG://����Ϣʼ���ղ�����������ԭ�� 
	//case WM_CTLCOLORMSGBOX:
	//case WM_CTLCOLOREDIT:
	//case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORBTN:
		switch( GetWindowLong( (HWND)lParam,GWL_STYLE ) & BS_TYPEMASK )
		{
		case BS_CHECKBOX:			// ��ѡ��
		case BS_AUTOCHECKBOX:		// �Զ���ѡ��
		case BS_3STATE:				// ��״̬��ѡ��
		case BS_AUTO3STATE:			// �Զ���״̬��ѡ��
		case BS_RADIOBUTTON:		// ��ѡ��
		case BS_AUTORADIOBUTTON:	// �Զ���ѡ��
		case BS_GROUPBOX:			// ��Ͽ�
			SetBkMode( (HDC)wParam,TRANSPARENT );
			return (LRESULT)GetBrush( pMetroUIData,hWnd,wParam,lParam );
		}
		break;
	//case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
		{
			// ���ý��û���ֻ��״̬�ı༭��Ҳ�ػ�����
			TCHAR tzClassName[8] = { 0 };
			GetClassName( (HWND)lParam,tzClassName,sizeof(tzClassName) );
			if( _tcsicmp( tzClassName,_T("Edit") ) == 0 )
				break;
		}
		SetBkMode( (HDC)wParam,TRANSPARENT );
		return (LRESULT)GetBrush( pMetroUIData,hWnd,wParam,lParam );
	case WM_NCHITTEST:
		lResult = ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam );
		{
			LONG lStyle = GetWindowLong( hWnd,GWL_STYLE );
			//{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_NCHITTEST %d %d\r\n"),lResult,i++);OutputDebugString( buf );}
			// ����С�� ��� �ر� ��ť
			if( lStyle & WS_SYSMENU )
			{
				POINT pt = { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };
				RECT rcWindow,rcButton;
				GetWindowRect( hWnd,&rcWindow );
				// ������������εĴ�С
				rcWindow.top += 2;
				rcWindow.bottom = rcWindow.top + GetSystemMetrics( SM_CYCAPTION ) + ( ( ( lStyle & WS_THICKFRAME ) == WS_THICKFRAME ) ? GetSystemMetrics( SM_CYSIZEFRAME ) : GetSystemMetrics( SM_CYFIXEDFRAME ) );
				// �رհ�ť�ľ�����
				rcButton = rcWindow;
				rcButton.left = rcButton.right - m_pPngCloseNormal->GetWidth();
				rcButton.right -= 2;
				if( PtInRect( &rcButton,pt ) )
					return NCHTCLOSEBUTTON;
				// ��󻯰�ť����
				rcButton.right = rcButton.left;
				if( lStyle & WS_MAXIMIZEBOX )
				{
					rcButton.left = rcButton.right - m_pPngMaxNormal->GetWidth();
					if( PtInRect( &rcButton,pt ) )
						return NCHTMAXBUTTON;
				}
				// ��С����ť����
				rcButton.right = rcButton.left;
				if( lStyle & WS_MINIMIZEBOX )
				{
					rcButton.left = rcButton.right - m_pPngMinNormal->GetWidth();
					if( PtInRect( &rcButton,pt ) )
						return NCHTMINBUTTON;
				}
				//// �˵���ť����
				//rcButton.right = rcButton.left;
				//rcButton.left = rcButton.right - m_pPngMenuNormal->GetWidth();
				//if( PtInRect( &rcButton,pt ) )
				//	return NCHTMENUBUTTON;
			}
			// ���������С�ر���Ϣ.�����������С����󻯹رհ�ť�ϰ���ʱ��Щ��ť������ʾ����
			if( lResult == HTZOOM || lResult == HTMINBUTTON || lResult == HTCLOSE || lResult == HTHELP )
				return HTNOWHERE;//��Ϊ������������
		}
		//{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_NCHITTEST %d %d\r\n"),lResult,i++);OutputDebugString( buf );}
		return lResult;
	case 0x00AE://:WM_NCUAHDRAWCAPTION
	case 0x00AF://:WM_NCUAHDRAWFRAME
		lResult = GetWindowLong( hWnd,GWL_STYLE );
		if( lResult & WS_VISIBLE )
			SetWindowLong( hWnd,GWL_STYLE,lResult & ( ~WS_VISIBLE ) );
		::DefWindowProc( hWnd,uMsg,wParam,lParam );
		if( lResult & WS_VISIBLE )
			SetWindowLong( hWnd,GWL_STYLE,lResult );
		return TRUE;
	case WM_NCMOUSEMOVE:
		// WM_NCMOUSEMOVE ��Ϣ���� WM_NCHITTEST ��Ϣ���������ģ�����WM_NCHITTEST��Ϣ��return HTERROR��������ֵʱ�����������WM_NCMOUSEMOVE��Ϣ
		if( !pMetroUIData->bTracking )
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = hWnd;
			tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
			pMetroUIData->bTracking = _TrackMouseEvent(&tme);
		}
		if( pMetroUIData->nNcHitTestCode != wParam )
		{
			// ���ԭ����ϵͳ��ť�ϵ����ڲ�����
			if( pMetroUIData->nNcHitTestCode == NCHTMAXBUTTON || pMetroUIData->nNcHitTestCode == NCHTMENUBUTTON || pMetroUIData->nNcHitTestCode == NCHTCLOSEBUTTON || pMetroUIData->nNcHitTestCode == NCHTMINBUTTON ||
				// ���ԭ������ϵͳ��ť�ϵ���������
				wParam == NCHTMAXBUTTON || wParam == NCHTMENUBUTTON || wParam == NCHTCLOSEBUTTON || wParam == NCHTMINBUTTON )
			{
				// �Ƚ�HTTESTֵ������������Ϊ���ػ溯���л��жϱ�����ֵ���ػ�ǰ������������Ӧֵ�Ļ����ػ�û��Ч����
				pMetroUIData->nNcHitTestCode = wParam;
				// ������û�а��²Ŵ����ػ溯�������������ڰ���״̬���򲻴����ػ溯��
				if( (GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0 )
				{
		{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_NCMOUSEMOVE %d %d %d\r\n"),wParam,pMetroUIData->nNcHitTestCode,i++);OutputDebugString( buf );}
					OnNcPaint( hWnd,1 );
				}
			}
			// ��ֹû�н�������������жϣ��ٸ�ֵһ�Ρ�
			pMetroUIData->nNcHitTestCode = wParam;
		}
		break;
	case WM_NCMOUSELEAVE:
		{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_NCMOUSELEAVE WM_NCMOUSELEAVE %d %d\r\n"),wParam,i++);OutputDebugString( buf );}
		pMetroUIData->bTracking = FALSE;
		// ���������ϵͳ��ť�ϰ�����
		if( pMetroUIData->nMouseDownNCHTTEST == NCHTMAXBUTTON || pMetroUIData->nMouseDownNCHTTEST == NCHTMENUBUTTON || pMetroUIData->nMouseDownNCHTTEST == NCHTCLOSEBUTTON || pMetroUIData->nMouseDownNCHTTEST == NCHTMINBUTTON )
		{
			// �����ǰ�����ɿ�״̬�����ݲ��Է���ʵ���ϲ�������������
			if( (GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0 )
			{
				pMetroUIData->nNcHitTestCode = HTERROR;
				pMetroUIData->nMouseDownNCHTTEST = HTERROR;
				OnNcPaint( hWnd,1 );
			}
		}
		// ���δ��ϵͳ��ť�ϰ��¹�
		else
		{
			// ����ϴ�����λ�øպ��������С���رհ�ť�ϣ���������뿪�ˣ�Ҫ�ػ���Щ��ť��
			if( pMetroUIData->nNcHitTestCode == NCHTCLOSEBUTTON || pMetroUIData->nNcHitTestCode == NCHTMAXBUTTON || pMetroUIData->nNcHitTestCode == NCHTMINBUTTON || pMetroUIData->nNcHitTestCode == NCHTMENUBUTTON )
			{
				pMetroUIData->nNcHitTestCode = HTERROR;
				OnNcPaint( hWnd,1 );
			}
			pMetroUIData->nNcHitTestCode = HTERROR;
		}
		break;
	case WM_NCLBUTTONDOWN:
		// ����������ɿ�����Ϣ�ںܶ���������ղ����ģ�����������������������������������ͷ������������WM_NCLBUTTONDOWN������ֱ��SetCaptureȻ����WM_NCLBUTTONUP��ReleaseCapture���������������ô���Ҳû�з�Ӧ�ˡ�
		// Ϊ�˽������������޷��յ�����������Ϣ�����������˺ܶ�취��û��һ����ȫ������ģ���������������BUG��
		// ���ս���취�������ϵͳ��ť�ϰ������Ժ���SetCapture�ٸ�һ��while(GetMessage())��ѭ����������������Ƶ��Ķ������Ϣ���ᱻ�������յ���
		// ���������ϢҲ���յ����յ����������Ϣ֮���ٷ���һ��WM_NCLBUTTONUP����Ϣ�����ɺܺõĴ���ϵͳ��ť��״̬���⡣
		// ���½��жϵ������ϵͳ��ť�ϰ���ʱ������nMouseDownNCHTTEST������ֵ��������while(GetMessage())��ѭ�����ڱ������������ط��������������Ĵ��롣
		if( wParam == NCHTCLOSEBUTTON || wParam == NCHTMAXBUTTON || wParam == NCHTMINBUTTON || wParam == NCHTMENUBUTTON )
		{
			pMetroUIData->nMouseDownNCHTTEST = wParam;
			// ��갴��֮�����whileѭ��֮ǰ���ػ�һ�±�����
			OnNcPaint( hWnd,1 );
			// ������꣬��������ƶ��κ�������ܽ��յ������Ϣ
			SetCapture( hWnd );
			// ��ʼ��Ϣѭ����ֱ���������ɿ����������¼�������
			MSG msg;
			BOOL bRet = FALSE;
			while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0 )
			{
				//{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("GetMessage  0x%04x %d\r\n"),msg.message,i++);OutputDebugString( buf );}
				if( bRet == -1 )
				{
					break;
				}
				else
				{
					// �����⵽�������ɿ�����Ϣ��������Ϣ��ֱ��������Ȼ����Ϣѭ������뵽WM_NCLBUTTONUP�ĺ�����������У����ػ洰��
					if( msg.message == WM_NCLBUTTONUP || msg.message == WM_LBUTTONUP )
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
						break;
					}
					// �û�����Ѱ��·�������û�ſ���ȥ�����̣�
					// �����⵽�������µ���Ϣ������ѭ���������ñ�־λ���ػ洰�ڣ�����ѭ������Ϊ��ʱ�Ѿ��޷�����WM_LBUTTONUP��Ϣ��
					if( msg.message == WM_KEYDOWN || msg.message == WM_KEYUP || msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP ||
						// ���û���յ��������Ϣ��������Ѿ��ɿ��ˣ�����ѭ����
						(GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0 )
					{
						POINT pt;
						GetCursorPos( &pt );
						pMetroUIData->nNcHitTestCode = SendMessage( hWnd,WM_NCHITTEST,0,MAKELPARAM(pt.x,pt.y) );
						pMetroUIData->nMouseDownNCHTTEST = HTERROR;
						OnNcPaint( hWnd,1 );
						TranslateMessage(&msg);
						DispatchMessage(&msg);
						break;
					}
					// ������������������������Ϣ���ַ���Ϣ��������Ϣ�Ĵ�����̡�
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			// ��Ϣѭ���������ͷ���겶��
			ReleaseCapture();
		}
		break;
	case WM_NCLBUTTONUP:
		// �������ϵ� WM_NCLBUTTONUP �� WM_NCMOUSELEAVE ��Ϣ�ǳ����ģ��ںܶ�����»��ղ��������԰ٶ�֮��Ū��һ��while(GetMessage))��ѭ����s
		{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_NCLBUTTONUP  %d %d\r\n"),wParam,i++);OutputDebugString( buf );}
		// �ɿ����ʱ����ڹرհ�ť��
		if( wParam == NCHTCLOSEBUTTON )
		{
			// ���������һ���ڹرհ�ť�ϰ����ˡ�
			if( pMetroUIData->nMouseDownNCHTTEST == NCHTCLOSEBUTTON )
			{
				// �����رմ��ڵ���Ϣ
				PostMessage( hWnd,WM_SYSCOMMAND, SC_CLOSE, 0 );
			}
		}
		else if( wParam == NCHTMAXBUTTON )
		{
			if( pMetroUIData->nMouseDownNCHTTEST == NCHTMAXBUTTON )
			{
				PostMessage( hWnd,WM_SYSCOMMAND, IsZoomed( hWnd ) ? SC_RESTORE : SC_MAXIMIZE, 0 );
			}
		}
		else if( wParam == NCHTMINBUTTON )
		{
			if( pMetroUIData->nMouseDownNCHTTEST == NCHTMINBUTTON )
			{
				PostMessage( hWnd,WM_SYSCOMMAND, SC_MINIMIZE, 0 );
			}
		}
		pMetroUIData->nNcHitTestCode = wParam;//SendMessage( hWnd,WM_NCHITTEST,0,lParam );
		// �����һ����ϵͳ��ť�ϰ����ˣ�
		if( ( pMetroUIData->nMouseDownNCHTTEST == NCHTMAXBUTTON || pMetroUIData->nMouseDownNCHTTEST == NCHTMENUBUTTON || pMetroUIData->nMouseDownNCHTTEST == NCHTCLOSEBUTTON || pMetroUIData->nMouseDownNCHTTEST == NCHTMINBUTTON )
			// �����ɿ����ʱ��겻��ϵͳ��ť���ˡ�
			&& ( wParam != NCHTMAXBUTTON && wParam != NCHTMENUBUTTON && wParam != NCHTCLOSEBUTTON && wParam != NCHTMINBUTTON ) )
		{
			pMetroUIData->nMouseDownNCHTTEST = HTERROR;
			OnNcPaint( hWnd,1 );
		}
		pMetroUIData->nMouseDownNCHTTEST = HTERROR;
		break;
	case WM_LBUTTONUP:
		{static int i = 0;TCHAR buf[64] = { 0 };_stprintf(buf,_T("WM_LBUTTONUP  %d %d\r\n"),wParam,i++);OutputDebugString( buf );}
		{
			POINT pt = {GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)};
			ClientToScreen( hWnd,&pt );
			SendMessage( hWnd,WM_NCLBUTTONUP,SendMessage( hWnd,WM_NCHITTEST,0,MAKELPARAM(pt.x,pt.y) ),MAKELPARAM(pt.x,pt.y) );
		}
		break;
	default:break;
	}
	return ::CallWindowProc( pMetroUIData->oldWndProc,hWnd,uMsg,wParam,lParam );
}

void OnNcPaint( HWND hWnd,WPARAM wParam )
{
	PMetroUIData pMetroUIData = (PMetroUIData)GetProp( hWnd,_T("MetroUI_Prop") );
	HDC hDC = NULL;
	//if( wParam == 1 )
		hDC = GetWindowDC( hWnd);
	//else
	//{
	//	hDC = ::GetDCEx(m_hWnd,(HRGN)wParam,DCX_WINDOW | DCX_INTERSECTRGN | 0x10000);
	//}
	if( hDC == NULL ) return;

	RECT rcWindow,rcClient;
	GetWindowRect( hWnd,&rcWindow );
	GetClientRect( hWnd,&rcClient );
	ClientToScreen( hWnd,&rcClient );
	int nOffsetX = rcClient.left - rcWindow.left;
	int nOffsetY = rcClient.top - rcWindow.top;
	ScreenToClient( hWnd,&rcClient );
	int nWidth = rcWindow.right - rcWindow.left;
	int nHeight = rcWindow.bottom - rcWindow.top;
	rcWindow.left = 0;rcWindow.right = nWidth;rcWindow.top = 0;rcWindow.bottom = nHeight;
	rcClient.left += nOffsetX;rcClient.right += nOffsetX;rcClient.top += nOffsetY;rcClient.bottom += nOffsetY;

	// һ��Ҫ��˫����DC��������˸��ǳ�����
	HDC hMemDC = CreateCompatibleDC( hDC );

	HBITMAP hMemBitmap = CreateCompatibleBitmap( hDC,nWidth,nHeight );
	HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC,hMemBitmap );

	HRGN rgnWindow = CreateRectRgn( rcWindow.left,rcWindow.top,rcWindow.right,rcWindow.bottom );
	HRGN rgnClient = CreateRectRgn( rcClient.left,rcClient.top,rcClient.right,rcClient.bottom );
	CombineRgn( rgnWindow,rgnWindow,rgnClient,RGN_XOR );
	DeleteObject( rgnClient );

	HRGN rgnWindowOld = CreateRectRgn( 0,0,0,0 );
	int nRet = GetWindowRgn( hWnd,rgnWindowOld );
	
	HBRUSH hBrush;
	HRGN hRgn;
	// ���Ʊ�����ʱһ��Ҫ���ü������򣬷�����Ƶ����ݻḲ�ǵ��ͻ�������
	// �Ȼ�����߿�
	CombineRgn( rgnWindow,rgnWindow,rgnWindowOld,RGN_AND );
	SelectClipRgn( hDC,rgnWindow );
	SelectClipRgn( hMemDC,rgnWindow );
	hBrush = CreateSolidBrush( 0x877A6F );
	FillRect( hMemDC,&rcWindow,hBrush );
	DeleteObject( hBrush );
	
	// �ٻ�����߿���������һ�����ص��ڰױ�
	hRgn = CreateStretchRgn( rgnWindowOld,((float)nWidth-(float)2)/(float)nWidth,((float)nHeight-(float)2)/(float)nHeight,1,1 );
	CombineRgn( rgnWindow,rgnWindow,hRgn,RGN_AND );
	SelectClipRgn( hMemDC,rgnWindow );
	hBrush = CreateSolidBrush( 0xFBF6ED );
	FillRect( hMemDC,&rcWindow,hBrush );
	DeleteObject( hBrush );
	DeleteObject( hRgn );

	// �ٻ����ڰױ����ڵķǿͻ�������
	hRgn = CreateStretchRgn( rgnWindowOld,((float)nWidth-(float)4)/(float)nWidth,((float)nHeight-(float)4)/(float)nHeight,2,2 );
	CombineRgn( rgnWindow,rgnWindow,hRgn,RGN_AND );
	SelectClipRgn( hMemDC,rgnWindow );
	hBrush = CreateSolidBrush( 0xFDE7C5 );
	//FillRect( hMemDC,&rcWindow,hBrush );
	GradientFillRect( hMemDC,rcWindow,0xFDE7C5,0xffffff,true );
	DeleteObject( hBrush );
	DeleteObject( hRgn );

	//// �ٻ��ƿͻ�����������һ�����صı߿�
	//RECT rcTmp = rcClient;
	//rcTmp.left -= 1;rcTmp.top -= 1;rcTmp.right += 1;rcTmp.bottom += 1;
	//hBrush = CreateSolidBrush( 0xC1B592 );
	//FrameRect( hMemDC,&rcTmp,hBrush );
	//DeleteObject( hBrush );

	//rcTmp.top -= 1;rcTmp.bottom = rcTmp.top + 1;
	//hBrush = CreateSolidBrush( 0xF5EEE0 );
	//FillRect( hMemDC,&rcTmp,hBrush );
	//DeleteObject( hBrush );

	RECT rcTitle = rcWindow;
	rcTitle.left += 8;rcTitle.bottom = 30;
	// ��������ͼ��
	HICON hIcon = (HICON)::SendMessage( hWnd, WM_GETICON, ICON_SMALL, 0 );
	if( hIcon )
	{
		DrawIconEx( hMemDC,rcTitle.left,( rcTitle.bottom - rcTitle.top - GetSystemMetrics( SM_CYSMICON ) ) / 2,hIcon,GetSystemMetrics( SM_CXSMICON ),GetSystemMetrics( SM_CYSMICON ),NULL,NULL,DI_NORMAL );
		rcTitle.left += GetSystemMetrics( SM_CXSMICON ) + 5;
	}

	// ����С�� ��� �ر� ��ť ͬʱ������������ֵ��ҽ��ߣ���ֹ���ֻ����ر���С����󻯰�ť����
	LONG lStyle = GetWindowLong( hWnd,GWL_STYLE );
	if( lStyle & WS_SYSMENU )
	{
		RECT rcButton = rcWindow;
		Gdiplus::Image * m_pImage = NULL;
		// �Ȼ��رհ�ť
		rcButton.left = rcButton.right - m_pPngCloseNormal->GetWidth();
		Gdiplus::Graphics graphics( hMemDC );
		m_pImage = m_pPngCloseNormal;
		rcTitle.right -= m_pImage->GetWidth();
		if( pMetroUIData->nNcHitTestCode == NCHTCLOSEBUTTON )
		{
			// ��һ���жϣ�����ֱ����if( pMetroUIData->nMouseDownNCHTTEST == NCHTCLOSEBUTTON )�жϣ��������������������󻯰�ť�ϰ����ˣ����󽡻�û���ͷ�
			// ��ʱ���رհ�ť��״̬��������ΪOver״̬���ˡ�
			if( pMetroUIData->nMouseDownNCHTTEST != HTERROR )
			{
				if( pMetroUIData->nMouseDownNCHTTEST == NCHTCLOSEBUTTON )
					m_pImage = m_pPngCloseDown;
			}
			else
				m_pImage = m_pPngCloseOver;
		}
		graphics.DrawImage( m_pImage,rcButton.left,0,m_pImage->GetWidth(),m_pImage->GetHeight() );
		// ����󻯰�ť
		rcButton.right = rcButton.left;
		if( lStyle & WS_MAXIMIZEBOX )
		{
			if( lStyle & WS_MAXIMIZE )// �Ѿ������״̬�����ָ���ť
			{
				m_pImage = m_pPngRestoreNormal;
				rcTitle.right -= m_pImage->GetWidth();
				if( pMetroUIData->nNcHitTestCode == NCHTMAXBUTTON )
				{
					if( pMetroUIData->nMouseDownNCHTTEST != HTERROR )
					{
						if( pMetroUIData->nMouseDownNCHTTEST == NCHTMAXBUTTON )
							m_pImage = m_pPngRestoreDown;
					}
					else
						m_pImage = m_pPngRestoreOver;
				}
				rcButton.left = rcButton.right - m_pImage->GetWidth();
				graphics.DrawImage( m_pImage,rcButton.left,0,m_pImage->GetWidth(),m_pImage->GetHeight() );
			}
			else// �������״̬������󻯰�ť
			{
				m_pImage = m_pPngMaxNormal;
				rcTitle.right -= m_pImage->GetWidth();
				if( pMetroUIData->nNcHitTestCode == NCHTMAXBUTTON )
				{
					if( pMetroUIData->nMouseDownNCHTTEST != HTERROR )
					{
						if( pMetroUIData->nMouseDownNCHTTEST == NCHTMAXBUTTON )
							m_pImage = m_pPngMaxDown;
					}
					else
						m_pImage = m_pPngMaxOver;
				}
				rcButton.left = rcButton.right - m_pImage->GetWidth();
				graphics.DrawImage( m_pImage,rcButton.left,0,m_pImage->GetWidth(),m_pImage->GetHeight() );
			}
		}
		// ����С����ť
		m_pImage = m_pPngMinNormal;
		rcTitle.right -= m_pImage->GetWidth();
		if( pMetroUIData->nNcHitTestCode == NCHTMINBUTTON )
		{
			if( pMetroUIData->nMouseDownNCHTTEST != HTERROR )
			{
				if( pMetroUIData->nMouseDownNCHTTEST == NCHTMINBUTTON )
					m_pImage = m_pPngMinDown;
			}
			else
				m_pImage = m_pPngMinOver;
		}
		rcButton.right = rcButton.left;
		if( lStyle & WS_MINIMIZEBOX )
		{
			rcButton.left = rcButton.right - m_pImage->GetWidth();
			graphics.DrawImage( m_pImage,rcButton.left,0,m_pImage->GetWidth(),m_pImage->GetHeight() );
		}
		//// ���˵���ť
		//m_pImage = m_pPngMenuNormal;
		//rcTitle.right -= m_pImage->GetWidth();
		//if( pMetroUIData->nNcHitTestCode == NCHTMENUBUTTON )
		//{
		//	if( pMetroUIData->nMouseDownNCHTTEST != HTERROR )
		//	{
		//		if( pMetroUIData->nMouseDownNCHTTEST == NCHTMENUBUTTON )
		//			m_pImage = m_pPngMenuDown;
		//	}
		//	else
		//		m_pImage = m_pPngMenuOver;
		//}
		//rcButton.right = rcButton.left;
		//rcButton.left = rcButton.right - m_pImage->GetWidth();
		//graphics.DrawImage( m_pImage,rcButton.left,0,m_pImage->GetWidth(),m_pImage->GetHeight() );
	}

	// д����������
	rcTitle.right -= 8;
	TCHAR tzTitle[64] = { 0 };//���ֻ��ʾǰ64���ַ�
	GetWindowText( hWnd,tzTitle,sizeof(tzTitle) );
	int nOldBkMode = SetBkMode( hMemDC,TRANSPARENT );
	HFONT hFont = (HFONT)::SendMessage( hWnd, WM_GETFONT, 0, 0 );
	HFONT hFontTitle;
	LOGFONT lf;
	if( hFont != NULL )
	{
		::GetObject( hFont, sizeof(LOGFONT),&lf );
		//lf.lfWeight = FW_BOLD;
	}
	else
	{
		SystemParametersInfo( SPI_GETICONTITLELOGFONT,sizeof(LOGFONT),&lf,0 );
		//lf.lfWeight = FW_BOLD;
	}
	hFontTitle = CreateFontIndirect( &lf );
	HFONT hOldFont = (HFONT)SelectObject( hMemDC,hFontTitle );
	DrawShadowText( hMemDC,tzTitle,_tcsclen( tzTitle ),&rcTitle,DT_LEFT | DT_SINGLELINE | DT_VCENTER,0x000001,0xffffff,1,2 );
	DrawShadowText( hMemDC,tzTitle,_tcsclen( tzTitle ),&rcTitle,DT_LEFT | DT_SINGLELINE | DT_VCENTER,0x000001,0xffffff,-1,0 );
	SelectObject( hMemDC,hOldFont );
	DeleteObject( hFontTitle );
	SetBkMode( hMemDC,nOldBkMode );

	BitBlt( hDC,0,0,nWidth,nHeight,hMemDC,0,0,SRCCOPY );

	SelectObject( hMemDC,hOldBitmap );

	// ��ʼ�ͷ�ǰ�潨������Դ����
	DeleteObject( rgnWindowOld );
	DeleteObject( rgnWindow );
	DeleteObject( hMemBitmap );
	DeleteDC( hMemDC );

	ReleaseDC( hWnd,hDC );
}

HBRUSH GetBrush( PMetroUIData pMetroUIData,HWND hWnd,WPARAM wParam,LPARAM lParam )
{
	map< HWND,HBRUSH >::iterator p = pMetroUIData->mapControlBrush.find( (HWND)lParam );
	if( p == pMetroUIData->mapControlBrush.end() )
	{
		TCHAR buf[64] = { 0 };
		GetWindowText( (HWND)lParam,buf,64 );
		HDC hDC = ::GetDC( hWnd );
		if( hDC )
		{
			RECT rc,rcControl;
			GetClientRect( hWnd,&rc );
			LONG lStyle = GetWindowLong( hWnd,GWL_STYLE );
			bool bResize = ( ( lStyle & WS_THICKFRAME ) == WS_THICKFRAME );
			// ��rc.top���ϱ��Ʊ������߶��ټ�2�����أ�Ŀ�����ڻ��ƿͻ�������ʱ���ֺͱ������ı�������һ�¡�
			rc.top -= GetSystemMetrics( SM_CYCAPTION ) + ( bResize ? GetSystemMetrics( SM_CYSIZEFRAME ) : GetSystemMetrics( SM_CYFIXEDFRAME ) ) - 2;
			GetWindowRect( (HWND)lParam,&rcControl );
			ScreenToClient( hWnd,&rcControl );

			HDC hMemDC = ::CreateCompatibleDC( hDC );
			HBITMAP hBitmap = ::CreateCompatibleBitmap( hDC,rc.right - rc.left,rc.bottom - rc.top );
			HBITMAP hOldBitmap = (HBITMAP)::SelectObject( hMemDC,hBitmap );

			GradientFillRect( hMemDC,rc,0xFDE7C5,0xffffff,true );

			HDC hMemDC2 = ::CreateCompatibleDC( hMemDC );
			HBITMAP hBitmap2 = ::CreateCompatibleBitmap( hMemDC,rcControl.right - rcControl.left,rcControl.bottom - rcControl.top );
			HBITMAP hOldBitmap2 = (HBITMAP)SelectObject( hMemDC2,hBitmap2 );
			
			BitBlt( hMemDC2,0,0,rcControl.right - rcControl.left,rcControl.bottom - rcControl.top,hMemDC, rcControl.left, rcControl.top, SRCCOPY);

			HBRUSH hBrush = ::CreatePatternBrush( hBitmap2 );
			pMetroUIData->mapControlBrush.insert( map<HWND,HBRUSH>::value_type( (HWND)lParam,hBrush ) );

			SelectObject( hMemDC2,hOldBitmap2 );
			DeleteObject( hBitmap2 );
			DeleteDC( hMemDC2 );

			SelectObject( hMemDC,hOldBitmap );
			DeleteObject( hBitmap );
			DeleteDC( hMemDC );

			ReleaseDC( hWnd,hDC );

			return hBrush;
		}
	}
	else
	{
		return (HBRUSH)(p->second);
	}
	return NULL;
}

void OnPaint( HWND hWnd )
{
	/*********************************************************************************************************************************************\
	IsIconic()�������жϴ����Ƿ�����С��״̬���������С����ť֮�󣩡�  ������ͨ�ĶԻ�����˵���������if (IsIconic()) �������AfxMessageBox("haha") ��
	��ᷢ����Ϣ�򲢲��ᵯ����  ԭ���ǣ�if (IsIconic()) ��δ�������OnPaint()�����ڣ�������С���˶Ի���֮����ȻIsIconic()��ֵ��TRUE������OnPaint()����
	���������С���ΪOnPaint()��Ӧ����WM_PAINT��Ϣ����WM_PAINT��Ϣ����Կͻ����ġ�һ����С���˵Ĵ��ڲ���Ҫ�ػ�ͻ�����  Ϊ����֤��һ�㣬��������һ��ʱ����
	��OnTimer()������д�� if(IsIconic()) MessageBeep(MB_OK); ��������С����ť���������������  ��ô��δ��뾿����ʲô�ã����ǲ�����Զ���ᱻִ���أ�
	��Ȼ���ǡ����������ӡ� ��һ�������ǿ�з���WM_PAINT��Ϣ������ִ�С� �ڶ�������ĶԻ��򡣱���һ��ToolBox���ĶԻ�������Ի�����ʾ��������������С
	��֮��������һ����С��һ����ʾ�������ϡ���ʱ��������ڵ����ͻ����WM_PAINT��Ϣ���Ӷ�ִ���Ƕδ��롣    ��֮��һ������¿��Բ�Ҫ��δ��룬����������
	;��Ҳ���Ǻ��˽⣬�����������ٿ���֪��������ô�������ġ�
	\********************************************************************************************************************************************/
	if( ::IsIconic( hWnd ) )
	{
	}
	else
	{
		PAINTSTRUCT ps;
		RECT rc;
		GetClientRect( hWnd,&rc );
		LONG lStyle = GetWindowLong( hWnd,GWL_STYLE );
		bool bResize = ( ( lStyle & WS_THICKFRAME ) == WS_THICKFRAME );
		rc.top -= GetSystemMetrics( SM_CYCAPTION ) + ( bResize ? GetSystemMetrics( SM_CYSIZEFRAME ) : GetSystemMetrics( SM_CYFIXEDFRAME ) ) - 2;
		HDC hDC = ::BeginPaint( hWnd,&ps );
		HDC hMemDC = ::CreateCompatibleDC( hDC );
		HBITMAP hBitmap = ::CreateCompatibleBitmap( hDC,rc.right - rc.left,rc.bottom - rc.top );
		HBITMAP hOldBitmap = (HBITMAP)SelectObject( hMemDC,hBitmap );
		GradientFillRect( hMemDC,rc,0xFDE7C5,0xffffff,true );
		::BitBlt( hDC,0,0,rc.right - rc.left,rc.bottom - rc.top,hMemDC,0,0,SRCCOPY );
		SelectObject( hMemDC,hOldBitmap );
		DeleteObject( hBitmap );
		DeleteDC( hMemDC );
		::EndPaint( hWnd,&ps );
	}
}

LRESULT CALLBACK CallWndProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if( ((CWPSTRUCT*)lParam)->message==WM_NCCREATE)
	{
		TCHAR tzClassName[64] = { 0 };
		GetClassName( ((CWPSTRUCT*)lParam)->hwnd,tzClassName,sizeof(tzClassName)-1 );
		if( _tcsicmp( tzClassName,_T("#32770") ) == 0 )
		{
			PMetroUIData pMetroUIData = new MetroUIData;
			pMetroUIData->oldWndProc = (WNDPROC)SetWindowLong( ((CWPSTRUCT*)lParam)->hwnd,GWL_WNDPROC,(LONG)WindowProc );
			pMetroUIData->lStyle = GetWindowLong( ((CWPSTRUCT*)lParam)->hwnd,GWL_STYLE );
			pMetroUIData->lStyleEx = GetWindowLong( ((CWPSTRUCT*)lParam)->hwnd,GWL_EXSTYLE );
			pMetroUIData->bTracking = FALSE;
			pMetroUIData->nMouseDownNCHTTEST = HTERROR;
			pMetroUIData->nNcHitTestCode = HTERROR;
			pMetroUIData->hClientBrush = NULL;
			SetProp( (HWND)wParam,_T("MetroUI_Prop"),(HANDLE)pMetroUIData );
		}
	}

	return CallNextHookEx(g_hHook,nCode,wParam,lParam);
}

LRESULT CALLBACK CBTProc( int nCode,WPARAM wParam,LPARAM lParam )
{
	if( HCBT_CREATEWND == nCode )
	{
		TCHAR tzClassName[64] = { 0 };
		GetClassName( (HWND)wParam,tzClassName,sizeof(tzClassName)-1 );
		if( _tcsicmp( tzClassName,_T("#32770") ) == 0 )
		{
			PMetroUIData pMetroUIData = new MetroUIData;
			pMetroUIData->oldWndProc = (WNDPROC)SetWindowLong( (HWND)wParam,GWL_WNDPROC,(LONG)WindowProc );
			pMetroUIData->lStyle = GetWindowLong( (HWND)wParam,GWL_STYLE );
			pMetroUIData->lStyleEx = GetWindowLong( (HWND)wParam,GWL_EXSTYLE );
			pMetroUIData->bTracking = FALSE;
			pMetroUIData->nMouseDownNCHTTEST = HTERROR;
			pMetroUIData->nNcHitTestCode = HTERROR;
			pMetroUIData->hClientBrush = NULL;
			SetProp( (HWND)wParam,_T("MetroUI_Prop"),(HANDLE)pMetroUIData );
		}
	}
	return CallNextHookEx(g_hHook,nCode,wParam,lParam);
}

LRESULT CALLBACK GetMsgProc( int code,WPARAM wParam,LPARAM lParam )
{
	if( ((MSG*)lParam)->message == WM_LBUTTONUP )
	{
		HWND hWnd = GetForegroundWindow();
		if( hWnd )
		{
			PMetroUIData pMetroUIData = (PMetroUIData)GetProp( hWnd,_T("MetroUI_Prop") );
			if( pMetroUIData )
			{
				if( pMetroUIData->nMouseDownNCHTTEST != HTERROR )
				{
					SendMessage( hWnd,WM_NCMOUSEMOVE,HTCLIENT,((MSG*)lParam)->lParam );
				}
			}
		}
	}
	return CallNextHookEx(g_hHook,code,wParam,lParam);
}

BOOL UseMetroUI( BOOL bUse )
{
	if( bUse )
	{
		// �Ѿ���װ�����ӣ��򷵻�FALSE
		if( g_hHook != NULL ) return FALSE;
		// ��ʼ��GDI+֧��
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL);
		// ����Դ�����PNGͼƬ
		// �˵���ť
		ImageFromIDResource( g_ucSysButtonMenuNormal,sizeof(g_ucSysButtonMenuNormal),m_pPngMenuNormal );
		ImageFromIDResource( g_ucSysButtonMenuOver,sizeof(g_ucSysButtonMenuOver),m_pPngMenuOver );
		ImageFromIDResource( g_ucSysButtonMenuDown,sizeof(g_ucSysButtonMenuDown),m_pPngMenuDown );
		// ��С����ť
		ImageFromIDResource( g_ucSysButtonMinNormal,sizeof(g_ucSysButtonMinNormal),m_pPngMinNormal );
		ImageFromIDResource( g_ucSysButtonMinOver,sizeof(g_ucSysButtonMinOver),m_pPngMinOver );
		ImageFromIDResource( g_ucSysButtonMinDown,sizeof(g_ucSysButtonMinDown),m_pPngMinDown );
		// ��󻯰�ť
		ImageFromIDResource( g_ucSysButtonMaxNormal,sizeof(g_ucSysButtonMaxNormal),m_pPngMaxNormal );
		ImageFromIDResource( g_ucSysButtonMaxOver,sizeof(g_ucSysButtonMaxOver),m_pPngMaxOver );
		ImageFromIDResource( g_ucSysButtonMaxDown,sizeof(g_ucSysButtonMaxDown),m_pPngMaxDown );
		// �ָ���ť
		ImageFromIDResource( g_ucSysButtonRestoreNormal,sizeof(g_ucSysButtonRestoreNormal),m_pPngRestoreNormal );
		ImageFromIDResource( g_ucSysButtonRestoreOver,sizeof(g_ucSysButtonRestoreOver),m_pPngRestoreOver );
		ImageFromIDResource( g_ucSysButtonRestoreDown,sizeof(g_ucSysButtonRestoreDown),m_pPngRestoreDown );
		// �رհ�ť
		ImageFromIDResource( g_ucSysButtonCloseNormal,sizeof(g_ucSysButtonCloseNormal),m_pPngCloseNormal );
		ImageFromIDResource( g_ucSysButtonCloseOver,sizeof(g_ucSysButtonCloseOver),m_pPngCloseOver );
		ImageFromIDResource( g_ucSysButtonCloseDown,sizeof(g_ucSysButtonCloseDown),m_pPngCloseDown );

		g_hHook = SetWindowsHookEx(WH_CBT,CBTProc,0,GetCurrentThreadId());
		return ( g_hHook != NULL );
	}
	else
	{
		// �ͷ�GDI+֧��
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
		if( g_hHook )
		{
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}
	return TRUE;
}

/*********************************************************************************************************************************************************\
���ڷǿͻ���������������ͷ�ʱ��ϵͳ�ᷢ�ʹ��ڵ�WM_LBUTTONUP(ע�ⲻ��WM_NCLBUTTONUP�����ǿͻ���������ͷ���Ϣ���������������ڷǿͻ����ͷ���꣡
��֮�κ�)��Ϣ���̵߳���Ϣ���У����̵߳���Ϣ����ѹ���Ͳ��������Ϣ���͸�������ڵĴ��ڹ���!

����취���£�

1.       ��Ӧ�ǿͻ�����갴�µ���ϢWM_NCLBUTTONDOWN�� ����һ��flag, m_bNCLButtonDowned = true; ��ʾ��ǰ�ڷǿͻ���������������

2.       ʹ��WH_GETMESSAGE��HOOK, hook�����̵߳���Ϣ����,�Ӷ��õ��ڷǿͻ����ͷ����ʱ��WM_LBUTTONUP��Ϣ(��Ϊspy++�������ַ�ʽ���������Ҳ���Բ������ַ�ʽ����Ȼgoogle����Ǹ���ʹ�õ���mouse hook,߯������Щ)

3.       ��hook�ص��������WM_LBUTTONUP��Ϣ�жϵ�ǰm_bNCLButtonDowned == true?�ǿͻ������Ƿ�����������������ǣ���PostMessageһ��WM_NCLBUTTONUP���������ķǿͻ�����

����ͷ���Ϣ���͸����ڵĴ��ڹ���

4.       �ڴ��ڹ������WM_NCLBUTTONUP�Ĵ��������ж�m_bNCLButtonDowned == true?����ǣ���ִ�д��룬ͬʱ��λm_bNCLButtonDowned,��ʾ��ǰ�Ѿ��ͷ��������������������û�а���

��ʵ�и��õİ취��������ô˵�ģ�����֤��һ�£��Ƚϳ��������ǽ����õ�һ�ַ�����������hook��


��OnSysCommand�����ﴦ��һ�£��ֶ���һ��WM_NCLBUTTONUP��Ϣ������������£�
void CXX::OnSysCommand(UINT nID, LPARAM lParam)

{
   CDialog::OnSysCommand(nID, lParam);
   if( (nID & 0xFFF0) == SC_MOVE )
   {
    PostMessage(WM_NCLBUTTONUP, HTCAPTION, lParam);   // ����WM_NCLBUTTONUP��Ϣ
   }

}

����zhllxt��ʵ�ʲ��Է��֣���ϵͳ��ť�ϰ����������������ƶ���������ߣ��ͷ������WH_GETMESSAGE��HOOK��Ȼ���ղ���WM_LBUTTONUP��Ϣ�����ǻᴥ��WM_NCMOUSELEAVE��Ϣ
���������ϵͳ��ť�ϰ��£�Ȼ������ƶ����ͻ����Ŀؼ��ϣ����ڿؼ����ͷ����ʱ���������WM_NCMOUSELEAVE��Ϣ��ȴ�����WH_GETMESSAGE HOOK������
\*********************************************************************************************************************************************************/
