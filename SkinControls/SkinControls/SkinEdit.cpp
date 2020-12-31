
#include "Stdafx.h"
#include "SkinEdit.h"

BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
	ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// ���캯��
CSkinEdit::CSkinEdit()
{
	// ������ɫ
	m_crEnableBK = RGB(255,255,255);
	m_crDisableBK = RGB(125,125,125);

	// ������ɫ
	m_crEnableText = RGB(10,10,10);
	m_crDisableText = RGB(50,50,50);

	// �߿���ɫ
	m_crEnableBorder = RGB(76,162,225);
	m_crDisableBorder = GetSysColor(COLOR_WINDOW);

	// ������ˢ
	m_brEnable.DeleteObject();
	m_brDisable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);
	m_brDisable.CreateSolidBrush(m_crDisableBK);
}

// ��������
CSkinEdit::~CSkinEdit()
{
}

// ������ɫ
void CSkinEdit::SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorder)
{
	// ������ɫ
	m_crEnableBK = crEnableBK;
	m_crEnableText = crEnableText;
	m_crEnableBorder = crEnableBorder;

	// ������ˢ
	m_brEnable.DeleteObject();
	m_brEnable.CreateSolidBrush(m_crEnableBK);

	// ���½���
	if (m_hWnd != NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return;
}

// ������ɫ
void CSkinEdit::SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorder)
{
	// ������ɫ
	m_crDisableBK = crDisableBK;
	m_crDisableText = crDisableText;
	m_crDisableBorder = crDisableBorder;

	// ������ˢ
	m_brDisable.DeleteObject();
	m_brDisable.CreateSolidBrush(m_crDisableBK);

	// ���½���
	if (m_hWnd != NULL)
	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ERASENOW);
	}

	return;
}

// �ػ���Ϣ
void CSkinEdit::OnNcPaint()
{
	// ״̬����
	bool bDisable = (IsWindowEnabled() == FALSE);
	COLORREF crColorBK = (bDisable == false) ? m_crEnableBK : m_crDisableBK;
	COLORREF crColorBorder = (bDisable == false) ? m_crEnableBorder : m_crDisableBorder;

	// �滭�߿�
	CClientDC ClientDC(this);

	// ��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);

	// �滭�߿�
	if (GetExStyle()&WS_EX_CLIENTEDGE)
	{
		ClientDC.Draw3dRect(&rcWindow, crColorBorder, crColorBorder);
		ClientDC.Draw3dRect(rcWindow.left+1, rcWindow.top+1, rcWindow.Width()-2, rcWindow.Height()-2, crColorBK, crColorBK);
	}
	else
	{
		ClientDC.Draw3dRect(&rcWindow, crColorBK, crColorBK);
		ClientDC.Draw3dRect(rcWindow.left+1, rcWindow.top+1, rcWindow.Width()-2, rcWindow.Height()-2, crColorBK, crColorBK);
	}

	return;
}

// �滭�ؼ�
HBRUSH CSkinEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// ���û���
	if (IsWindowEnabled() == FALSE)
	{
		// ����״̬
		pDC->SetBkColor(m_crDisableBK);
		pDC->SetTextColor(m_crDisableText);
		return m_brDisable;
	}
	else
	{
		// ����״̬
		pDC->SetBkColor(m_crEnableBK);
		pDC->SetTextColor(m_crEnableText);
		return m_brEnable;
	}

	return NULL;
}
