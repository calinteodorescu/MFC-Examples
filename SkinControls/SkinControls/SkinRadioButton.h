
#ifndef SKIN_RADIOBUTTON_H_
#define SKIN_RADIOBUTTON_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinRadioBox.h"

// ��ѡ��ť ��������ģ��TabCtrl ListBox
class SKIN_CONTROLS_EXPORT CSkinRadioButton : public CSkinRadioBox
{
public:
	CSkinRadioButton();
	virtual ~CSkinRadioButton();

	// ���غ���
public:
	// �ؼ����໯
	virtual void PreSubclassWindow();
	// ����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// ���ú���
public:
	// ����λͼ
	bool SetButtonImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN, bool bAutoClipTrans = false);
	// ����λͼ
	bool SetButtonImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN, bool bAutoClipTrans = false);
	// ����λͼ
	bool SetButtonImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN, bool bAutoClipTrans = false);

	// ��Ϣ����
protected:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	// �ڲ�����
protected:
	void SizeToImage();
	bool CalcButtonRgn();

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinRadioButton)
};

#endif // SKIN_RADIOBUTTON_H_