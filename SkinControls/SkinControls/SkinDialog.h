
#ifndef SKIN_DIALOG_H_
#define SKIN_DIALOG_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinWindow.h"

// �Ի�����
class SKIN_CONTROLS_EXPORT CSkinDialog : public CDialog
{
public:
	// ���캯��
	CSkinDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	// ��������
	virtual ~CSkinDialog();

	// ���ܺ���
public:
	// ����SKINͼƬ
	bool SetSkinImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	bool SetSkinImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	bool SetSkinImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���������ɫ crInactiveText: δ�������ֵ���ɫ
	void SetCaptionTextColor(COLORREF crText, COLORREF crInactiveText);

	// ���غ���
protected:
	// ��ʼ����Ϣ
	virtual BOOL OnInitDialog();

	// ��Ϣӳ��
protected:
	// �ǿͻ�����С
	void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	// �ǿͻ�����������
	LRESULT OnNcHitTest(CPoint point);
	// �ǿͻ�������
	void OnNcPaint();
	// ������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);
	// λ����Ϣ
	void OnSize(UINT nType, int cx, int cy);
	// �ǿͻ�������ƶ�
	void OnNcMouseMove(UINT nHitTest, CPoint point);
	// �ǿͻ�������뿪
	void OnNcMouseLeave();
	// �������
	void OnNcLButtonDown(UINT nFlags, CPoint point);
	// ����ɿ�
	void OnNcLButtonUp(UINT nFlags, CPoint point);
	// �Ҽ��ɿ�
	void OnNcRButtonUp(UINT nFlags, CPoint point);
	// ������Ϣ
	void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	// ����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	// �ؼ���ɫ
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinDialog)

protected:
	CSkinWindow* m_pSkin;

protected:
	COLORREF m_crBackground;
	COLORREF m_crBottomBar;
	COLORREF m_crBottomLine;
	int m_nBottomBarHeight;
	CBrush m_BkBrush;
};

#endif // SKIN_DIALOG_H_