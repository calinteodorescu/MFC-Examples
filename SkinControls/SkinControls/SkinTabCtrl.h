
#ifndef SKIN_TABCTRL_H_
#define SKIN_TABCTRL_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinImage.h"

// ѡ��ؼ�
class SKIN_CONTROLS_EXPORT CSkinTabCtrl : public CTabCtrl
{
public:
	// ���캯��
	CSkinTabCtrl();
	// ��������
	virtual ~CSkinTabCtrl();

	// ���ܺ���
public:
	// ���ð�ť���
	void SetItemPadding(int nPadding = 1);
	// ������ɫ
	void SetTabCtrlColor(COLORREF crBackGround, COLORREF crBorder);
	// ����TABͼ
	bool SetTabImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ����TABͼ
	bool SetTabImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ����TABͼ
	bool SetTabImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);
	// ����TAB����ͼ
	bool SetTabBgImage(LPCTSTR lpszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ����TAB����ͼ
	bool SetTabBgImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ����TAB����ͼ
	bool SetTabBgImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);

	// ���غ���
protected:
	// �ؼ����໯
	virtual void PreSubclassWindow();

	// ��Ϣ����
protected:
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �ػ�����	
	void OnPaint();
	// �滭����
	BOOL OnEraseBkgnd(CDC* pDC);
	// ��С��Ϣ
	void OnSize(UINT nType, int cx, int cy);
	// �������
	void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

	// ��������
protected:
	COLORREF m_crBackGround; // ������ɫ
	COLORREF m_crBorder; // �߿���ɫ
	CSkinImage m_ImageTab; // TABͼƬ
	CSkinImage m_ImageTabBg; // TAB ����
	int m_nItempadding; // ��ť�м�ľ���

private:
	static CSkinImage m_ImageDefTab; // Ĭ��ͼƬ
};

#endif // SKIN_TABCTRL_H_