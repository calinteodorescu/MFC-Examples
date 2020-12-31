
#include "StdAfx.h"
#include "Resource.h"
#include "SkinMsgBox.h"
#include "SkinHelper.h"

// ʱ���ʶ
#define IDI_MESSAGE					100									// ��Ϣ��ʶ

// ����λ��
#define SMB_WINDOW_WIDTH			230									// ��С���

// ͼ��ƫ��
#define SMB_ICON_LEFT				20									// ͼ��ƫ��
#define SMB_ICON_WIDTH				32									// ͼ����
#define SMB_ICON_HEIGHT				32									// ͼ��߶�

// �ַ�ƫ��
#define SMB_STRING_TOP				25									// �ַ�ƫ��
#define SMB_STRING_LEFT				20									// �ַ�ƫ��
#define SMB_STRING_RIGHT			40									// �ַ�ƫ��
#define SMB_STRING_BOTTOM			25									// �ַ�ƫ��
#define SMB_STRING_WIDTH			600									// �����
#define SMB_STRING_HEIGHT			600									// �����

// ��ťƫ��
#define SMB_BUTTON_LEFT				12									// ��ťƫ��
#define SMB_BUTTON_RIGHT			12									// ��ťƫ��
#define SMB_BUTTON_SPACE			20									// ��ť���
#define SMB_BUTTON_WIDTH			70									// ��ťƫ��
#define SMB_BUTTON_HEIGHT			23									// ��ťƫ��

// ��ť����
#define SMB_BUTTON_AREA_HEIGHT		30									// ��ť����

// ���ڷǿͻ���
#define WND_TOP_HEIGHT				27									// ���ڶ���
#define WND_BOTOOM_HEIGHT			3									// ���ڵײ�
#define WND_LEFT_WIDTH				3									// �������
#define WND_RIGHT_WIDTH				3									// �����ұ�

BEGIN_MESSAGE_MAP(CSkinMsgBox, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// ���캯��
CSkinMsgBox::CSkinMsgBox(CWnd* pParentWnd) : CSkinDialog(IDD_SKINMSGBOX, pParentWnd)
{
	// ���ñ���
	m_uType = MB_OK;

	// ��������
	m_nElapse = 0;
	m_nBtCount = 0;
	m_uDefBtnIndex = 0;
}

// ��������
CSkinMsgBox::~CSkinMsgBox()
{
}

// ��ʼ����Ϣ
BOOL CSkinMsgBox::OnInitDialog()
{
	__super::OnInitDialog();

	// ���ñ���
	SetWindowText(m_strCaption);

	// ������Դ
	AfxSetResourceHandle(GetModuleHandle(SKIN_CONTROLS_DLL_NAME));

	// ����ͼ��
	HICON hIcon = LoadIcon(GetModuleHandle(SKIN_CONTROLS_DLL_NAME), MAKEINTRESOURCE(IDI_MAINFRAME));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	// ��ť����
	CreateBoxButton();
	int nButtonAreaWidth = m_nBtCount * SMB_BUTTON_WIDTH + (m_nBtCount + 1) * SMB_BUTTON_SPACE + SMB_BUTTON_LEFT + SMB_BUTTON_RIGHT;

	// �ַ��ռ�
	CClientDC ClientDC(this);
	CRect rcString(0, 0, SMB_STRING_WIDTH, SMB_STRING_HEIGHT);
	ClientDC.SelectObject(&CSkinHelper::GetDefaultFont());
	ClientDC.DrawText(m_strString, rcString, DT_CALCRECT | DT_EXPANDTABS | DT_NOCLIP | DT_WORD_ELLIPSIS);

	// �ַ�����
	int nStringAreaHeight = rcString.Height() + SMB_STRING_TOP + SMB_STRING_BOTTOM;
	int nStringAreaWidth = rcString.Width() + SMB_ICON_LEFT + SMB_ICON_WIDTH + SMB_STRING_LEFT + SMB_STRING_RIGHT;

	// ��������
	int nWindowAreaHeight = nStringAreaHeight + SMB_BUTTON_AREA_HEIGHT;
	int nWindowAreaWidth = max(SMB_WINDOW_WIDTH, max(nStringAreaWidth, nButtonAreaWidth));

	// ���λ��
	if (nWindowAreaWidth > nStringAreaWidth)
	{
		m_rcString.top = SMB_STRING_TOP;
		m_rcString.left= SMB_ICON_LEFT + SMB_ICON_WIDTH + SMB_STRING_LEFT + (nWindowAreaWidth - nStringAreaWidth) / 2;
		m_rcString.right = m_rcString.left + rcString.Width();
		m_rcString.bottom = m_rcString.top + rcString.Height();
	}
	else
	{
		m_rcString.top = SMB_STRING_TOP;
		m_rcString.left = SMB_ICON_LEFT + SMB_ICON_WIDTH + SMB_STRING_LEFT;
		m_rcString.right = m_rcString.left + rcString.Width();
		m_rcString.bottom = m_rcString.top + rcString.Height();
	}

	// ��������
	int nWindowWidth = nWindowAreaWidth + WND_LEFT_WIDTH + WND_RIGHT_WIDTH;
	int nWindowHeight = nWindowAreaHeight + WND_TOP_HEIGHT + WND_BOTOOM_HEIGHT;
	SetWindowPos(NULL, 0, 0, nWindowWidth, nWindowHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);

	// ����λ��
	CRect rcClient;
	GetClientRect(rcClient);
	int nYButton = rcClient.bottom - SMB_BUTTON_AREA_HEIGHT + (SMB_BUTTON_AREA_HEIGHT - SMB_BUTTON_HEIGHT) / 2 + 2;
	int nXButton = (rcClient.Width() - m_nBtCount * SMB_BUTTON_WIDTH - (m_nBtCount + 1) * SMB_BUTTON_SPACE) / 2 + SMB_BUTTON_SPACE;

	// ������ť
	for (UINT i = 0; i < m_nBtCount; i++)
	{
		m_btnButton[i].SetWindowPos(NULL, nXButton, nYButton, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
		nXButton += SMB_BUTTON_SPACE + SMB_BUTTON_WIDTH;
	}

	// Ĭ�ϰ�ť
	m_uDefBtnIndex = (m_uType & MB_DEFMASK) >> 8;
	if (m_uDefBtnIndex >= m_nBtCount) m_uDefBtnIndex = 0;
	m_btnButton[m_uDefBtnIndex].SetFocus();
	m_btnButton[m_uDefBtnIndex].SetButtonStyle(m_btnButton[m_uDefBtnIndex].GetButtonStyle() | BS_DEFPUSHBUTTON);
	m_btnButton[m_uDefBtnIndex].GetWindowText(m_strDefBtn);
	m_strDefBtn = m_strDefBtn.Left(m_strDefBtn.Find(TEXT("(")));

	// ���д���
	SetActiveWindow();
	SetForegroundWindow();
	CenterWindow(GetParent());

	// ����ʱ��
	if (m_nElapse != 0) SetTimer(IDI_MESSAGE, 1000, NULL);

	return FALSE;
}

// ������Ϣ
BOOL CSkinMsgBox::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// ��ť��ʶ
	UINT uButtonID = LOWORD(wParam);

	// �����ж�
	for (UINT i = 0; i < m_nBtCount; i++)
	{
		// ��ȡ��ʶ
		UINT uExistID = GetWindowLong(m_btnButton[i], GWL_ID);

		// ��ʶ����
		if (uButtonID == uExistID)
		{
			EndDialog(uButtonID);
			return TRUE;
		}
	}

	// Ĭ�ϴ���
	if (uButtonID == IDOK)
	{
		EndDialog(GetWindowLong(m_btnButton[m_uDefBtnIndex], GWL_ID));
		return TRUE;
	}

	// ȡ������
	if (uButtonID == IDCANCEL)
	{
		EndDialog(GetWindowLong(m_btnButton[m_nBtCount - 1], GWL_ID));
		return TRUE;
	}

	// Ĭ�ϴ���
	EndDialog(uButtonID);

	return TRUE;
}

// ��ʾ��Ϣ
int CSkinMsgBox::ShowMsgBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	return ShowMsgBox(TEXT("ϵͳ��ʾ"), pszString, uType, nElapse);
}

// ��ʾ��Ϣ
int CSkinMsgBox::ShowMsgBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	// ���ñ���
	m_uType = uType;
	m_nElapse = nElapse;
	m_strString = pszString;
	m_strCaption = pszCaption;

	// ������Դ
	AfxSetResourceHandle(GetModuleHandle(SKIN_CONTROLS_DLL_NAME));

	// ��ʾ����
	int nResult = (int)DoModal();

	return nResult;
}

// ���ť
void CSkinMsgBox::CreateBoxButton()
{
	// ��������
	UINT uButtonID[3] = {0,0,0};
	LPCTSTR pszString[3] = {NULL, NULL, NULL};

	// ���ð�ť
	switch (m_uType & MB_TYPEMASK)
	{
	case MB_OKCANCEL:
		{
			m_nBtCount = 2;
			uButtonID[0] = IDOK;
			uButtonID[1] = IDCANCEL;
			pszString[0] = TEXT("ȷ��(&O)");
			pszString[1] = TEXT("ȡ��(&C)");
			break;
		}
	case MB_YESNO:
		{
			m_nBtCount = 2;
			uButtonID[0] = IDYES;
			uButtonID[1] = IDNO;
			pszString[0] = TEXT("��(&Y)");
			pszString[1] = TEXT("��(&N)");
			break;
		}
	case MB_YESNOCANCEL:
		{
			m_nBtCount = 3;
			uButtonID[0] = IDYES;
			uButtonID[1] = IDNO;
			uButtonID[2] = IDCANCEL;
			pszString[0] = TEXT("��(&Y)");
			pszString[1] = TEXT("��(&N)");
			pszString[2] = TEXT("ȡ��(&C)");
			break;
		}
	case MB_RETRYCANCEL:
		{
			m_nBtCount = 2;
			uButtonID[0] = IDRETRY;
			uButtonID[1] = IDCANCEL;
			pszString[0] = TEXT("����(&R)");
			pszString[1] = TEXT("ȡ��(&C)");
			break;
		}
	case MB_ABORTRETRYIGNORE:
		{
			m_nBtCount = 3;
			uButtonID[0] = IDABORT;
			uButtonID[1] = IDRETRY;
			uButtonID[2] = IDIGNORE;
			pszString[0] = TEXT("��ֹ(&A)");
			pszString[1] = TEXT("����(&R)");
			pszString[2] = TEXT("����(&I)");
			break;
		}
	default:
		{
			m_nBtCount=1;
			uButtonID[0]=IDOK;
			pszString[0]=TEXT("ȷ��(&O)");
			break;
		}
	}

	// ��ʾ��ť
	CRect rcButton(0, 0, SMB_BUTTON_WIDTH, SMB_BUTTON_HEIGHT);
	for (UINT i = 0; i< m_nBtCount; i++) m_btnButton[i].Create(pszString[i], WS_CHILD, rcButton, this, uButtonID[i]);

	return;
}

// �滭��Ϣ
void CSkinMsgBox::OnPaint()
{
	CPaintDC dc(this);

	// ��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	// ����ͼ��
	HICON hIcon = NULL;
	UINT uIconType = (m_uType & MB_ICONMASK);
	if (uIconType == MB_ICONHAND) hIcon = LoadIcon(NULL,IDI_HAND);
	else if (uIconType == MB_ICONQUESTION) hIcon = LoadIcon(NULL,IDI_QUESTION);
	else if (uIconType == MB_ICONEXCLAMATION) hIcon = LoadIcon(NULL,IDI_EXCLAMATION);
	else hIcon = LoadIcon(NULL, IDI_ASTERISK);

	// �滭ͼ��
	int nYPos=(m_rcString.bottom + m_rcString.top) / 2 - SMB_ICON_HEIGHT / 2;
	DrawIconEx(dc, SMB_ICON_LEFT, nYPos, hIcon, SMB_ICON_WIDTH, SMB_ICON_HEIGHT, 0, NULL, DI_NORMAL);

	// �滭����
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(10, 10, 10));
	dc.SelectObject(&CSkinHelper::GetDefaultFont());
	dc.DrawText(m_strString, &m_rcString, DT_EXPANDTABS | DT_NOCLIP | DT_WORD_ELLIPSIS);

	return;
}

// ʱ����Ϣ
void CSkinMsgBox::OnTimer(UINT_PTR nIDEvent)
{
	// ����ʱ��
	if (nIDEvent == IDI_MESSAGE)
	{
		// ɾ���ж�
		if (m_nElapse == 0)
		{
			PostMessage(WM_CLOSE, 0, 0);
			return;
		}

		// ���ý���
		CString strBtn;
		strBtn.Format(TEXT("%s(%ld)"), m_strDefBtn, m_nElapse--);
		m_btnButton[m_uDefBtnIndex].SetWindowText(strBtn);

		return;
	}

	__super::OnTimer(nIDEvent);
}

// ��ʾ��Ϣ
extern "C" SKIN_CONTROLS_EXPORT int ShowMsgBox(LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CSkinMsgBox SkinMsgBox;
	return SkinMsgBox.ShowMsgBox(pszString, uType, nElapse);
}

// ��ʾ��Ϣ
extern "C" SKIN_CONTROLS_EXPORT int ShowMsgBoxEx(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType, UINT nElapse)
{
	CSkinMsgBox SkinMsgBox;
	return SkinMsgBox.ShowMsgBox(pszCaption, pszString, uType, nElapse);
}
