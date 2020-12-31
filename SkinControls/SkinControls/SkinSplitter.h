
#ifndef SKIN_SPLITTER_H_
#define SKIN_SPLITTER_H_
#pragma once

#include "SkinControlsDefine.h"

// �������
enum SplitterType
{
	ST_HOR, // ������
	ST_VOR, // ������
};

// ����¼�
interface ISplitterEvent
{
	// ����¼�
	virtual void OnSplitterEvent(UINT uControlID, int nXPos, int nYPos) = 0;
};

// �������
class SKIN_CONTROLS_EXPORT CSkinSplitter : public CWnd
{
public:
	// ���캯��
	CSkinSplitter();
	// ��������
	virtual ~CSkinSplitter();

	// ���غ���
protected:
	// �󶨺���
	virtual void PreSubclassWindow();
	// ������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// ���ú���
public:
	// ��������
	void SetSplitterType(SplitterType st);
	// ���÷�Χ
	void SetSplitterRange(int nMinDragPos, int nMaxDragPos);
	// ���ýӿ�
	void SetSplitterEvent(ISplitterEvent* pISplitterEvent);

	// ������ɫ
public:
	// ������ɫ
	void SetSplitterColor(COLORREF crBackColor);

	// ��ѯ����
public:
	// ��ȡ����
	SplitterType GetSplitterType() { return m_SplitterType; }
	// ��ȡ�ӿ�
	ISplitterEvent* GetSplitterEvent() { return m_pISplitterEvent; };
	// ���÷�Χ
	void GetSplitterRange(int& nMinDragPos, int& nMaxDragPos) { nMinDragPos = m_nMinDragPos; nMaxDragPos = m_nMaxDragPos; }

	// ��Ϣ����
protected:
	// �ػ���Ϣ
	void OnPaint();
	// �滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	// ʧȥ����
	void OnKillFocus(CWnd * pNewWnd);
	// �����Ϣ
	void OnMouseMove(UINT nFlags, CPoint point);
	// �����Ϣ
	void OnLButtonUp(UINT nFlags, CPoint point);
	// �����Ϣ
	void OnLButtonDown(UINT nFlags, CPoint point);
	// �����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMmessage);

	DECLARE_MESSAGE_MAP()

	// ״̬����
protected:
	bool m_bMouseDown; // �Ƿ���
	CRect m_rcDrawLine; // ����λ��

	// ���Ա���
protected:
	int m_nMaxDragPos; // ���λ��
	int m_nMinDragPos; // ��Сλ��
	SplitterType m_SplitterType; // �������

	// ��ɫ����
protected:
	COLORREF m_crBackColor; // ������ɫ

	// �������
protected:
	ISplitterEvent* m_pISplitterEvent; // ����¼�
};

#endif // SKIN_SPLITTER_H_