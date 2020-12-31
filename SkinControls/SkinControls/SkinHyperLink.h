
#ifndef SKIN_HYPERLINK_H_
#define SKIN_HYPERLINK_H_
#pragma once

#include "SkinControlsDefine.h"

// ��������
class SKIN_CONTROLS_EXPORT CSkinHyperLink : public CStatic
{
public:
	// ���캯��
	CSkinHyperLink();
	// ��������
	virtual ~CSkinHyperLink();

	// ��ѯ����
public:
	// ��ȡ����
	bool GetAutoAdjust() { return m_bAutoAdjust; }
	// ��ȡ����
	bool GetVisited() { return m_bVisited; }
	// ��ȡ�»���
	bool GetUnderline() { return m_bUnderline; }
	// ��ȡ URL
	LPCTSTR GetHyperLinkUrl() { return m_strURL; }

	// ���ú���
public:
	// ���÷���
    void SetVisited(bool bVisited);
	// �����»���
    void SetUnderline(bool bUnderline);
	// ���õ���
    void SetAutoAdjust(bool bAutoAdjust);
	// ���� URL
	void SetHyperLinkUrl(LPCTSTR pszUrl);
	// ���ñ���ɫ
	void SetBackColor(COLORREF cr);
	// ������ɫ
	void SetHyperLinkTextColor(COLORREF crHoverText, COLORREF crNormalText, COLORREF crVisitedText);

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();

	// �ڲ�����
protected:
	// ��ʼ��
	void InitHyperLink();
	// ����λ��
	void AdjustHyperLink();

	// ��Ϣ����
protected:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �滭��Ϣ
	void OnPaint();
	// �����ؼ�
	void OnStnClicked();
	// ����ƶ�
	void OnMouseMove(UINT nFlags, CPoint point);
	// �����Ϣ
    BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	// ����뿪
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// ������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);
	// �ؼ���ɫ
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinHyperLink)

	// ��������
private:
	bool m_bHovering; // ������־
	bool m_bVisited; // �Ƿ����
	bool m_bUnderline; // �Ƿ���
	bool m_bAutoAdjust; // �Զ�����
	CString	 m_strURL; // ���ӵ�ַ
	COLORREF m_crBack; // ����
	COLORREF m_crHoverText; // ������ɫ
	COLORREF m_crNormalText; // ������ɫ
	COLORREF m_crVisitedText; // ������ɫ
};

#endif // SKIN_HYPERLINK_H_