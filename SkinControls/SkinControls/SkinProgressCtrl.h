
#ifndef SKIN_PROGRESSCTRL_H_
#define SKIN_PROGRESSCTRL_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinImage.h"

// ������
class SKIN_CONTROLS_EXPORT CSkinProgressCtrl : public CProgressCtrl
{
public:
	CSkinProgressCtrl();
	virtual ~CSkinProgressCtrl();

	// ���ܺ���
public:
	// ���ñ���ͼ
	void SetProgressBackImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���ͼ
	void SetProgressBackImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ���ñ���ͼ
	void SetProgressBackImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ���ý���ͼ
	void SetProgressImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ���ý���ͼ
	void SetProgressImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ���ý���ͼ
	void SetProgressImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();

protected:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �滭
	void OnPaint();
	// ����
	BOOL OnEraseBkgnd(CDC* pDC);
	void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinProgressCtrl)

protected:
	CSkinImage m_ImageProgress; // ������
	CSkinImage m_ImageBack; //����ͼ
};

#endif // SKIN_PROGRESSCTRL_H_