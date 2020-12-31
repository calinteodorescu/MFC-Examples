
#ifndef SKIN_SLIDERCTRL_H_
#define SKIN_SLIDERCTRL_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinImage.h"

#define WM_SKINSLIDER_MOVING (WM_USER + 9425)
#define WM_SKINSLIDER_MOVED  (WM_USER + 9426)

// ������
class SKIN_CONTROLS_EXPORT CSkinSliderCtrl : public CStatic
{
public:
	CSkinSliderCtrl();
	virtual ~CSkinSliderCtrl();

	// ���ܺ���
public:
	// ���ñ���ͼƬ
	void SetBackImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	void SetBackImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	void SetBackImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���ͼƬ2
	void SetBack2Image(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	void SetBack2Image(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	void SetBack2Image(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ���û�����ͼƬ
	void SetThumbImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	void SetThumbImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	void SetThumbImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���ɫ
	void SetBackColor(COLORREF crBackColor) { m_crBackColor = crBackColor; }

	// ��ѯ����
	void GetRange(int &nMin, int &nMax) { nMin = m_nMin; nMax = m_nMax; }
	int GetRangeMax() { return m_nMax; }
	int GetRangeMin() { return m_nMin; }

	int GetPos() { return m_nPos; }

	// ��������
	void SetRange(int nMin, int nMax, BOOL bRedraw=FALSE);
	void SetRangeMin(int nMin, BOOL bRedraw = FALSE);
	void SetRangeMax(int nMax, BOOL bRedraw = FALSE);

	void SetPos(int nPos);

	void SetVertical(BOOL bVertical = TRUE) { m_bVertical = bVertical; };

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();

protected:
	void SetBitmap(HBITMAP hBitmap) {};

	int Pixel2Pos(int nPixel);
	int Pos2Pixel(int nPos);

	// ��Ϣ����
protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDC* pDC);
	void OnPaint();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	UINT OnGetDlgCode();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnDestroy();
	void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinSliderCtrl)

protected:
	CSkinImage m_ImageBack; // ����1
	CSkinImage m_ImageBack2; // ����2
	CSkinImage m_ImageThumb; // ������
	COLORREF m_crBackColor;  // ����ɫ

	int m_nMax, m_nMin, m_nPos, m_nPage;
	int m_nMouseOffset;

	BOOL m_bVertical;
	BOOL m_bLButtonDown;

private:
	static CSkinImage m_ImageDefSlider; // ������
};

#endif // SKIN_SLIDERCTRL_H_