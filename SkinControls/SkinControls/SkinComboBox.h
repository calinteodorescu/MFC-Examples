
#ifndef SKIN_COMBOBOX_H_
#define SKIN_COMBOBOX_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinEdit.h"
#include "SkinImage.h"

// �б�ؼ�
class SKIN_CONTROLS_EXPORT CSkinComboBoxList : public CListBox
{
public:
	// ���캯��
	CSkinComboBoxList();
	// ��������
	virtual ~CSkinComboBoxList();

	// ���غ���
protected:
	// �滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ��Ϣӳ��
protected:
	// ����滭
	void OnNcPaint();

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

// �༭�ؼ�
class SKIN_CONTROLS_EXPORT CSkinComboBoxEdit : public CSkinEdit
{
protected:
	// ���ý���
	afx_msg VOID OnSetFocus(CWnd* pOldWnd);
	// ʧȥ����
	afx_msg VOID OnKillFocus(CWnd* pNewWnd);
	// ����뿪
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// ����ƶ�
	afx_msg VOID OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

// ѡ��ؼ�
class SKIN_CONTROLS_EXPORT CSkinComboBox : public CComboBox
{
	friend class CSkinComboBoxEdit;

public:
	// ���캯��
	CSkinComboBox();
	// ��������
	virtual ~CSkinComboBox();

	// ���غ���
protected:
	// �ؼ���
    virtual VOID PreSubclassWindow();
	// ��������
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	// ���ܺ���
public:
	// �߿�����
	void SetDrawBorder(bool bDrawBorad);
	// ������Դ
	bool SetButtonImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ������Դ
	bool SetButtonImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ������Դ
	bool SetButtonImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);

	// �ڲ�����
protected:
	// ���ÿؼ�
	void Initialization();
	// ���¿ؼ�
	void RedrawFocusedFrame();
	// �����ؼ�
	void RectifyControl(int nWidth, int nHeight);
	// ���ø���
	void SetHighlighted(BOOL bHot);

	// ��Ϣӳ��
protected:
	// �ػ���Ϣ
    void OnPaint();
	// �б�ر�
	void OnCbnCloseup();
	// �滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ����ƶ�
	void OnMouseMove(UINT nFlags, CPoint Point);
	// ����뿪
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// ���ý���
	void OnSetFocus(CWnd* pOldWnd);
	// ʧȥ����
	void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP()

	// ���Ա���
protected:
	enum
	{
		_nImageNormal = 0,
		_nImageFocusOrHover,
		_nImagePushed,
		_nImageDisabled,

		_nImageCount = 4,
	};
	UINT m_uButtonWidth; // ��ť���
	COLORREF m_crEnableBack; // ������ɫ
	COLORREF m_crDisableBack; // ������ɫ
	COLORREF m_crEnableBorder; // �߿���ɫ
	COLORREF m_crDisableBorder; // �߿���ɫ
	CSkinImage m_ImageButton;

	// ״̬����
protected:
	BOOL m_bHighlighted; // ����
	BOOL m_bFocused; // ����
	bool m_bDrawBorder; // �滭�߿�

	// �ؼ�����
protected:
	CSkinComboBoxEdit m_SkinComboBoxEdit; // �༭�ؼ�
	CSkinComboBoxList m_SkinComboBoxList; // �б�ؼ�

private:
	static CSkinImage m_ImageDefButton;
};

#endif // SKIN_COMBOBOX_H_