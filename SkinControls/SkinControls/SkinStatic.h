
#ifndef SKIN_STATIC_H_
#define SKIN_STATIC_H_
#pragma once

#include "SkinControlsDefine.h"

// ���ֻ�ָ���
class SKIN_CONTROLS_EXPORT CSkinStatic : public CStatic
{
public:
	// ���캯��
	CSkinStatic();
	// ��������
	virtual ~CSkinStatic();

	// ���ú���
public:
	// �����»���
    void SetUnderline(bool bUnderline);
	// ���÷ָ���
	void SetSeperator(bool bSeperator);
	// ���ñ���ɫ
	void SetBackColor(COLORREF cr);
	// ������ɫ
	void SetStaticTextColor(COLORREF crHoverText, COLORREF crNormalText);
	// �ָ�����ɫ
	void SetSeperatorColor(COLORREF crFirst, COLORREF crSecond);

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();

	// ��Ϣ����
protected:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �滭��Ϣ
	void OnPaint();
	// ����ƶ�
	void OnMouseMove(UINT nFlags, CPoint point);
	// �����Ϣ
    BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	// ����뿪
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// ������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);
	// �ؼ���ɫ
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinStatic)

	// ��������
protected:
	bool m_bHovering; // ������־
	bool m_bUnderline; // �Ƿ���
	bool m_bSeperator; // �ָ���
	COLORREF m_crBack; // ����
	COLORREF m_crHoverText; // ������ɫ
	COLORREF m_crNormalText; // ������ɫ
	COLORREF m_crSepFirst; // �ָ�����ɫ
	COLORREF m_crSepSecond; // �ָ�����ɫ
};

#endif // SKIN_STATIC_H_