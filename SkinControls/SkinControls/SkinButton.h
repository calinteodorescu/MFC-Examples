
#ifndef SKIN_BUTTON_H_
#define SKIN_BUTTON_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinImage.h"

// ��ť��
// ���ͼƬ��PNG �� ��alpha�Ļ� ���Լ̳�CSkinButtonȻ�����´���OnEraseBkgnd���䷵��Ĭ�ϴ���ʽ(CButton::OnEraseBkgnd),
// ��������Ͳ��ᵼ��4��״̬ͼ���غϵ�����,����һ��ϵͳĬ�ϱ���,��������һ��.
class SKIN_CONTROLS_EXPORT CSkinButton : public CButton
{
public:
	CSkinButton();
	virtual ~CSkinButton();

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();
	// ����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ���ú���
public:
	// ����λͼ
	bool SetButtonImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN, bool bAutoClipTrans = false);
	// ����λͼ
	bool SetButtonImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN, bool bAutoClipTrans = false);
	// ����λͼ
	bool SetButtonImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN, bool bAutoClipTrans = false);
	// ����͸��ɫ
	void SetMaskColor(COLORREF clrMask) { m_clrMask = clrMask; }

	// ��Ϣ����
protected:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ����ƶ�
	void OnMouseMove(UINT nFlags, CPoint point);
	// ����뿪
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// ������Ϣ
	BOOL OnEraseBkgnd(CDC * pDC);
	// ��С��Ϣ
	void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinButton)

	// �ڲ�����
protected:
	void SizeToImage();
	bool CalcButtonRgn();

protected:
	CSkinImage m_ImageButton; // ��ťͼƬ
	bool m_bAutoClipTrans; // �Ƿ��Զ��ü�͸������
	COLORREF m_clrMask; // MASK��ɫ
	bool m_bMouseOver; // ������־

private:
	static CSkinImage m_ImageDefButton; // Ĭ�ϰ�ť
};

#endif // SKIN_BUTTON_H_