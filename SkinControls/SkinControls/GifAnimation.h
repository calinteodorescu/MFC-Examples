
#ifndef SKIN_GIFANIMATION_H_
#define SKIN_GIFANIMATION_H_
#pragma once

#include "SkinControlsDefine.h"
#include "GifImage.h"
#include "SkinImage.h"

// GIF������
/*
// �÷�
// CGifAnimation m_GifAnimation;
// m_GifAnimation.SetAnimationInfo(...);
// m_GifAnimation.LoadFromFile(...);
// m_GifAnimation.Play();
// ��OnPaint()�� m_GifAnimation.Draw(...);
*/

class CCompatibleDC;

class SKIN_CONTROLS_EXPORT CGifAnimation
{
public:
	// ���캯��
	CGifAnimation();
	// ��������
	~CGifAnimation();

	// ���ܺ���
public:
	// ���ó�ʼ��Ϣ
	void SetAnimationInfo(HWND hWnd, CPoint& ptDraw, COLORREF clrBackground);
	// ����
	void Play();
	// �滭����
	void Draw(CDC* pDC);
	// ֹͣ
	void Stop();
	// ���ض���
	bool LoadFromFile(LPCTSTR pszFileName);
	// ���ض���
	bool LoadFromMemroy(BYTE* pcbData, LONG lDataSize);
	// ���ض���
	bool LoadFromResource(LPCTSTR pszResourceName, LPCTSTR pszResourceType);
	// ֹͣ����ͬʱɾ����Դ
	void UnLoad();
	// ���ڲ���
	BOOL IsPlaying() const;

private:
	BOOL PrepareDC();
	void ThreadAnimation();
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);

private:
	CPoint m_ptDraw;
	BOOL m_bIsPlaying;
	volatile BOOL m_bExitThread;
	HWND m_hWnd;
	CGIFImage m_GifImage;
	
	HDC m_hMemDC;
	HDC m_hDispMemDC;
	HBITMAP m_hDispMemBM;
	HBITMAP m_hDispOldBM;
	BOOL m_bIsInitialized;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	COLORREF m_clrBackground;

	HANDLE m_hThread;
	HANDLE m_hExitEvent;
};

#endif // SKIN_GIFANIMATION_H_
