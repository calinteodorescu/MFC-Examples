
#ifndef SKIN_WINDOW_H_
#define SKIN_WINDOW_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinImage.h"

class SKIN_CONTROLS_EXPORT CSkinWindow  
{
public:
	CSkinWindow();
	~CSkinWindow();

public:
	// ����SKINͼƬ
	bool SetSkinImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	bool SetSkinImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	bool SetSkinImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���������ɫ crInactiveText: δ�������ֵ���ɫ
	void SetCaptionTextColor(COLORREF crText, COLORREF crInactiveText);

public:
	// ��ʼ����
	void InitWindow(CWnd* pWnd);
	// �ǿͻ�����С
	void OnNcCalcSize(CWnd* pWnd, BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	// �ǿͻ�����������
	LRESULT OnNcHitTest(CWnd* pWnd, CPoint point, BOOL bResizable = FALSE);
	// �ǿͻ�������
	void OnNcPaint(CWnd* pWnd);
	// ������Ϣ
	void OnSetText(CWnd* pWnd);
	// λ����Ϣ
	void OnSize(CWnd* pWnd);
	// �ǿͻ�������ƶ�
	void OnNcMouseMove(CWnd* pWnd, UINT nHitTest, CPoint point);
	// �ǿͻ�������뿪
	void OnNcMouseLeave(CWnd* pWnd);
	// �������
	BOOL OnNcLButtonDown(CWnd* pWnd, UINT nFlags, CPoint point);
	// ����ɿ�
	BOOL OnNcLButtonUp(CWnd* pWnd, UINT nFlags, CPoint point);
	// �Ҽ��ɿ�
	BOOL OnNcRButtonUp(CWnd* pWnd, UINT nFlags, CPoint point);
	// ������Ϣ
	BOOL OnActivateApp(CWnd* pWnd, BOOL bActive, DWORD dwThreadID);
	// ����
	BOOL OnEraseBkgnd(CWnd* pWnd, CDC * pDC);

	// �ڲ�����
private:
	void ResolveAnchor(const CRect& rcClient, CRect& rcAnchor, int nAnchor);
	void Paint(CWnd* pWnd, CDC* pDC);

private:
	enum
	{
		SKINPART_TOP_LEFT, SKINPART_TOP, SKINPART_TOP_RIGHT,
		SKINPART_IA_TOP_LEFT, SKINPART_IA_TOP, SKINPART_IA_TOP_RIGHT,
		SKINPART_LEFT_TOP, SKINPART_LEFT, SKINPART_LEFT_BOTTOM,
		SKINPART_IA_LEFT_TOP, SKINPART_IA_LEFT, SKINPART_IA_LEFT_BOTTOM,
		SKINPART_RIGHT_TOP, SKINPART_RIGHT, SKINPART_RIGHT_BOTTOM,
		SKINPART_IA_RIGHT_TOP, SKINPART_IA_RIGHT, SKINPART_IA_RIGHT_BOTTOM,
		SKINPART_BOTTOM_LEFT, SKINPART_BOTTOM, SKINPART_BOTTOM_RIGHT,
		SKINPART_IA_BOTTOM_LEFT, SKINPART_IA_BOTTOM, SKINPART_IA_BOTTOM_RIGHT,

		SKINPART_MINIMIZE, SKINPART_MINIMIZE_HOT, SKINPART_MINIMIZE_DOWN,
		SKINPART_IA_MINIMIZE,
		SKINPART_MAXIMIZE, SKINPART_MAXIMIZE_HOT, SKINPART_MAXIMIZE_DOWN,
		SKINPART_IA_MAXIMIZE,
		SKINPART_CLOSE, SKINPART_CLOSE_HOT, SKINPART_CLOSE_DOWN,
		SKINPART_IA_CLOSE,
		SKINPART_RESTORE, SKINPART_RESTORE_HOT, SKINPART_RESTORE_DOWN,
		SKINPART_IA_RESTORE,

		SKINPART__COUNT
	};

	enum
	{
		SKINANCHOR_SYSICON,
		SKINANCHOR_MINIMIZE,
		SKINANCHOR_MAXIMIZE,
		SKINANCHOR_CLOSE,

		SKINANCHOR__COUNT
	};

	enum
	{
		SKINPARTMODE_TILE,
		SKINPARTMODE_STRETCH
	};

private:
	static CSkinImage m_ImageDefSkin;
	CSkinImage m_ImageSkin; // ͼƬ

private:
	CRect m_rcPart[SKINPART__COUNT];
	CRect m_rcAnchor[SKINANCHOR__COUNT];

private:
	CRect m_rcResize;
	CRect m_rcCaption;
	CFont m_fnCaption;
	COLORREF m_crCaptionText;
	COLORREF m_crCaptionInactive;

private:
	int m_nHoverAnchor;
	int m_nDownAnchor;

private:
	BOOL m_bTracking;
	BOOL m_bIsMinimzseEnable;
	BOOL m_bIsMaximizeEnable;
};

#endif // SKIN_WINDOW_H_
