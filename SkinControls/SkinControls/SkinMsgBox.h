
#ifndef SKIN_MSG_BOX_H_
#define SKIN_MSG_BOX_H_
#pragma once

#include "SkinControlsDefine.h"
#include "SkinButton.h"
#include "SkinDialog.h"

// ��Ϣ����
class SKIN_CONTROLS_EXPORT CSkinMsgBox : public CSkinDialog
{
public:
	// ���캯��
	CSkinMsgBox(CWnd* pParentWnd = NULL);
	// ��������
	virtual ~CSkinMsgBox();

	// ���غ���
protected:
	// ��ʼ����Ϣ
	virtual BOOL OnInitDialog();
	// ������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// ���ܺ���
public:
	// ��ʾ��Ϣ
	int ShowMsgBox(LPCTSTR pszString, UINT uType = MB_OK, UINT nElapse = 0);
	// ��ʾ��Ϣ
	int ShowMsgBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType = MB_OK, UINT nElapse = 0);

	// �ڲ�����
private:
	// ������ť
	void CreateBoxButton();

	// ��Ϣӳ��
protected:
	// �滭��Ϣ
	VOID OnPaint();
	// ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

	// ���Ա���
private:
	UINT m_uType; // ��Ϣ����
	CString m_strString; // ��ʾ��Ϣ
	CString m_strCaption; // ��Ϣ����

	// ��������
private:
	UINT m_nElapse; // ʣ��ʱ��
	UINT m_nBtCount; // ��ť��Ŀ
	CRect m_rcString; // �ַ�λ��
	CString m_strDefBtn; // Ĭ�ϰ�ť
	UINT m_uDefBtnIndex; // Ĭ������

	// �ؼ�����
private:
	CSkinButton m_btnButton[3]; // ��ť�ؼ�
};

// ȫ�ֺ���
extern "C" SKIN_CONTROLS_EXPORT INT ShowMsgBox(LPCTSTR pszString, UINT uType = MB_OK, UINT nElapse = 0);
extern "C" SKIN_CONTROLS_EXPORT INT ShowMsgBoxEx(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType = MB_OK, UINT nElapse = 0);

#endif // SKIN_MSG_BOX_H_