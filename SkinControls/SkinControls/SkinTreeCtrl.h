
#ifndef SKIN_TREECTRL_H_
#define SKIN_TREECTRL_H_
#pragma once

#include "SkinScrollBar.h"
#include "SkinControlsDefine.h"
#include "SkinImage.h"

// �����б�
class SKIN_CONTROLS_EXPORT CSkinTreeCtrl : public CTreeCtrl
{
public:
	CSkinTreeCtrl();
	virtual ~CSkinTreeCtrl();

	// ������Դ
public:
	// ���ü�ͷ
	// ����Դ����ͼƬ���ü�ͷ
	void SetArrowImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ���ļ�����ͼƬ���ü�ͷ
	void SetArrowImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ���ڴ����ͼƬ���ü�ͷ
	void SetArrowImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���ɫ
	void SetBackColor(COLORREF crBack) { m_crBack = crBack; }
	// ���ú�����ɫ
	void SetLineColor(COLORREF crLine) { m_crLine = crLine; }
	// ����ѡ����ɫ
	void SetSelectedColor(COLORREF crSelected) { m_crSelected = crSelected; }
	// ����������ɫ
	void SetHoverColor(COLORREF crHover) { m_crHover = crHover; }

	// ���ܺ���
public:
	// ��ȡѡ��
	HTREEITEM GetCurrentSelectItem(bool bOnlyText);

	// չ������
public:
	// չ���ж�
	bool ExpandVerdict(HTREEITEM hTreeItem);
	// չ���б�
	bool ExpandListItem(HTREEITEM hTreeItem);

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();
	//���ں���
	virtual LRESULT DefWindowProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

public:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �ػ���Ϣ
	void OnPaint();
	// �滭����
	BOOL OnEraseBkgnd(CDC* pDC);
	// λ����Ϣ
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	// ����ƶ�
	void OnMouseMove(UINT nFlags, CPoint point);
	// �����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	// �Ҽ��б�
	void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	// ����б�
	void OnNMLClick(NMHDR* pNMHDR, LRESULT* pResult);
	// �б�ı�
	void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	// �б�չ��
	void OnTvnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinTreeCtrl)

	// �滭����
private:
	// �滭����
	void DrawTreeItem(CDC* pDC, CRect& rcClient, CRect& rcClipBox);
	// �滭����
	void DrawTreeBack(CDC* pDC, CRect& rcClient, CRect& rcClipBox);

	// �滭����
private:
	// ����ͼ��
	void DrawListImage(CDC* pDC, CRect rcRect, HTREEITEM hTreeItem);
	// �����ı�
	void DrawItemString(CDC* pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	// λ�ñ���
protected:
	int m_nXScroll; // ����ƫ��
	int m_nYScroll; // ����ƫ��

	// ��Դ����
protected:
	CSkinImage m_ImageArrow; // ��ͷͼ��
	COLORREF m_crBack; // ����ɫ
	COLORREF m_crLine; // ������ɫ
	COLORREF m_crSelected; // ѡ����ɫ
	COLORREF m_crHover; // ������ɫ

	// ��������
protected:
	HTREEITEM m_hItemMouseHover; // ��������
	HTREEITEM m_hTreeClickExpand; // ��������

protected:
	CSkinScrollBar m_SkinScrollBar; // ��������

};

#endif // SKIN_TREECTRL_H_