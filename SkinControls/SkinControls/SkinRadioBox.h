
#ifndef SKIN_RADIOBOX_H_
#define SKIN_RADIOBOX_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinButton.h"

// ��ѡ��
class SKIN_CONTROLS_EXPORT CSkinRadioBox : public CSkinButton
{
public:
	CSkinRadioBox();
	virtual ~CSkinRadioBox();

	// ���غ���
public:
	// �ؼ����໯
	virtual void PreSubclassWindow();
	// ����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ���ú���
public:
	// ����λͼ
	bool SetButtonImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ����λͼ
	bool SetButtonImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ����λͼ
	bool SetButtonImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ����������ɫ
	void SetRadioBoxTextColor(COLORREF clrTextColor);
	// ���ñ���ɫ
	void SetBackColor(COLORREF clrBack) { m_clrBack = clrBack; }

public:
	// ������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ��С��Ϣ
	void OnSize(UINT nType, int cx, int cy);
	// �������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ����ɿ�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// ��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// ʧȥ����
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnGetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnSetCheck(WPARAM, LPARAM);
	// �ؼ���ɫ
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinRadioBox)

	// �ڲ�����
protected:
	void UncheckRadioButtonsInGroup();

protected:
	BOOL m_bChecked;
	COLORREF m_clrBack;
	COLORREF m_clrTextColor;

private:
	static CSkinImage m_ImageDefButton; // Ĭ�ϰ�ť
};

#endif // SKIN_RADIOBOX_H_