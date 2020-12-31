
#ifndef DUI_BUTTON_H_
#define DUI_BUTTON_H_
#pragma once

#include "SkinControlsDefine.h"
#include "DUIControl.h"
#include "SkinImage.h"

// ��ť 
class SKIN_CONTROLS_EXPORT CDUIButton : public CDUIControl
{
public:
	CDUIButton();
	virtual ~CDUIButton();

public:
	virtual LPCTSTR GetClass() const;
	virtual void PaintStatusImage(CDC* pDC);
	virtual void PaintText(CDC* pDC);

	// �������
	virtual bool OnMouseMove(const CPoint& point);
	virtual bool OnMouseLeave();
	virtual bool OnLButtonDown(const CPoint& point);
	virtual bool OnLButtonUp(const CPoint& point);

	virtual void SetEnabled(bool bEnable  = true, bool bReDraw = false);

	virtual void SetImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN, COLORREF clrTrans = CLR_NONE);

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);
	virtual void UpdateSkinInfo(LPCTSTR lpszName, LPCTSTR lpszValue);

	virtual void OnClick();

protected:
	CSkinImage m_ImageStatus; // ��ť��ͼ���������ĸ�״̬ͼ�� ���ε�״̬ ����m_byStatus
	BYTE m_byStatus; // 0 normal 1 hover 2 down 3 disable
	bool m_bMouseHover;	// ������ƶ���ʱ�� ������ڵ�ǰ�ؼ����ƶ� ֻˢ��һ��
	COLORREF m_clrTrans; // ͸��ɫ
};

#endif // DUI_BUTTON_H_
