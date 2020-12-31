
#ifndef SKIN_CHECKBUTTON_H_
#define SKIN_CHECKBUTTON_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinCheckBox.h"

// ��ѡ��ť ��������ģ�ⰴť��һ��״̬ �������¼���λͼ
class SKIN_CONTROLS_EXPORT CSkinCheckButton : public CSkinCheckBox
{
public:
	CSkinCheckButton();
	virtual ~CSkinCheckButton();

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

public:
	// ������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinCheckButton)

	// �ڲ�����
protected:
	void SizeToImage();
	bool CalcButtonRgn();
};

#endif // SKIN_CHECKBUTTON_H_