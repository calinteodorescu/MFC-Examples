
#ifndef SKIN_EDIT_H_
#define SKIN_EDIT_H_
#pragma once

#include "SkinControlsDefine.h"

// �༭��
class SKIN_CONTROLS_EXPORT CSkinEdit : public CEdit
{
	// ��������
public:
	// ���캯��
	CSkinEdit();
	// ��������
	virtual ~CSkinEdit();

	// ���ܺ���
public:
	// ������ɫ
	void SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorder);
	// ������ɫ
	void SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorder);

	// ��Ϣ���� 
protected:
	// �ػ���Ϣ
    void OnNcPaint();
	// �滭�ؼ�
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

	// ��ˢ����
public:
	CBrush m_brEnable; // ��ˢ��Դ
	CBrush m_brDisable; // ��ˢ��Դ

	// ������ɫ
public:
	COLORREF m_crEnableBK; // ������ɫ
	COLORREF m_crDisableBK; // ������ɫ

	// ������ɫ
public:
	COLORREF m_crEnableText; // ������ɫ
	COLORREF m_crDisableText; // ������ɫ

	// �߿���ɫ
public:
	COLORREF m_crEnableBorder; // �߿���ɫ
	COLORREF m_crDisableBorder; // �߿���ɫ
};

#endif // SKIN_EDIT_H_