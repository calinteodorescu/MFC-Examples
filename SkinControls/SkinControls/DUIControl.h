
#ifndef DUI_CONTROL_H_
#define DUI_CONTROL_H_
#pragma once

#include "SkinControlsDefine.h"

class CDUIManager;

// �ؼ�����
class SKIN_CONTROLS_EXPORT CDUIControl
{
public:
	CDUIControl();
	virtual ~CDUIControl();

public:
	// �ؼ�ID ����Ψһ
	virtual UINT GetID() const;
	virtual void SetID(UINT uID);
	virtual void SetName(LPCTSTR lpszName);
	virtual const CString& GetName() const;
	// ���� �������������ؼ� ����"Button" "CheckBox"��
	virtual LPCTSTR GetClass() const;

	virtual void SetManager(CDUIManager* pDUIManager);

	// �ı����
	virtual CString GetText() const;
	virtual void SetText(LPCTSTR pszText, bool bReDraw = false);
	virtual void SetFont(CFont* Font);
	// DrawText�����һ�������ĸ�ʽ
	virtual void SetTextFormat(UINT uFormat, bool bReDraw = false);
	virtual void SetTextColor(COLORREF crText, bool bReDraw = false);

	// λ�����
	virtual CRect GetPos() const;
	// ������ô��ҵ��� ��ԭ���Ǵ��ұ߿�ʼ(�����) ���µ���ͬ��
	virtual void SetPos(const CRect& rc, bool bReDraw = false);
	virtual void SetPos(int x, int y, int cx, int cy, bool bReDraw = false);
	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual int GetX() const;
	virtual int GetY() const;

	// DrawText�������� DT_LEFT DT_TOP
	virtual void SetLayoutAlign(UINT uLayoutFormat);

	// �����ʾ
	virtual CString GetToolTip() const;
	virtual void SetToolTip(LPCTSTR pszText);

	virtual COLORREF GetBkColor() const;
	virtual void SetBkColor(COLORREF crBk, bool bReDraw = false);

	// һЩ��Ҫ������
	virtual bool IsVisible() const;
	virtual void SetVisible(bool bVisible = true, bool bReDraw = false);
	virtual bool IsEnabled() const;
	virtual void SetEnabled(bool bEnable = true, bool bReDraw = false);

	virtual void SetAttribute(LPCTSTR lpszName, LPCTSTR lpszValue);

	// �滭
	virtual void DoPaint(CDC* pDC);
	virtual void PaintBkColor(CDC* pDC);
	virtual void PaintBkImage(CDC* pDC);
	virtual void PaintStatusImage(CDC* pDC);
	virtual void PaintText(CDC* pDC);

	// �������
	virtual bool OnMouseMove(const CPoint& point);
	virtual bool OnMouseLeave();	// �����Ҫ��Ŀ�ģ�buttonΪ���������ؼ��ڴ��ڱ�Ե��ʱ�� ����뿪���� ���ؼ�������ѡ�л��߰���״̬��ʱ����Ҫˢ��
	virtual bool OnLButtonDown(const CPoint& point);
	virtual bool OnLButtonUp(const CPoint& point);
	virtual bool OnRButtonDown(const CPoint& point);
	virtual bool OnRButtonUp(const CPoint& point);
	virtual bool OnMouseCursor(const CPoint& point);

	virtual void OnClick();
	virtual void UpdateControl();
	virtual void UpdateSkinInfo(LPCTSTR lpszName, LPCTSTR lpszValue);

	// �����ؼ�
	virtual bool CreateControl(UINT uID, LPCTSTR lpszName, CDUIManager* pDUIManager, const CRect& rc, LPCTSTR pszText = NULL, bool bVisble = true);

protected:
	UINT m_uID;
	CString m_strName;
	CRect m_rcItem;
	bool m_bVisible;
	bool m_bEnabled;
	CString m_strText;
	CString m_strToolTip;
	CFont* m_pFont;
	UINT m_uTextFormat;
	UINT m_uLayoutFormat;
	COLORREF m_clrBk;
	COLORREF m_clrTextColor;
	CDUIManager* m_pDUIManager;
};

#endif // DUI_CONTROL_H_