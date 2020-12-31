
#ifndef SKIN_CHECKBOX_H_
#define SKIN_CHECKBOX_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinButton.h"

// ��ѡ��
class SKIN_CONTROLS_EXPORT CSkinCheckBox : public CSkinButton
{
public:
	CSkinCheckBox();
	virtual ~CSkinCheckBox();

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
	void SetCheckBoxTextColor(COLORREF clrTextColor);
	// ���ñ���ɫ
	void SetBackColor(COLORREF clrBack) { m_clrBack = clrBack; }
	// ����Դ����ͼƬ���ñ���ͼƬ
	void SetBackImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ���ļ�����ͼƬ���ñ���ͼƬ
	void SetBackImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ���ڴ����ͼƬ���ñ���ͼƬ
	void SetBackImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);

public:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ��С��Ϣ
	void OnSize(UINT nType, int cx, int cy);
	// �������
	void OnLButtonDown(UINT nFlags, CPoint point);
	// ����ɿ�
	void OnLButtonUp(UINT nFlags, CPoint point);
	// ʧȥ����
	void OnKillFocus(CWnd* pNewWnd);
	LRESULT OnGetCheck(WPARAM, LPARAM);
	LRESULT OnSetCheck(WPARAM, LPARAM);
	// �ؼ���ɫ
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinCheckBox)

protected:
	BOOL m_bChecked;
	CSkinImage m_ImageBackground;
	COLORREF m_clrBack;
	COLORREF m_clrTextColor;

private:
	static CSkinImage m_ImageDefButton; // Ĭ�ϰ�ť
};

#endif // SKIN_CHECKBOX_H_