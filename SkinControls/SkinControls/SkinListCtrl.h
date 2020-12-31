
#ifndef SKIN_LISTCTRL_H_
#define SKIN_LISTCTRL_H_
#pragma once

#include "SkinScrollBar.h"
#include "SkinControlsDefine.h"
#include "SkinImage.h"

// ����λ��
#define SORT_AFTER 1 // ����λ��
#define SORT_FRONT -1 // ǰ��λ��

// ��������
enum enSortType
{
	SortType_NoSort,				//û������
	SortType_Ascend,				//��������
	SortType_Descale,				//��������
};

// ������
class CSkinListCtrl;
class CSkinHeaderCtrl;

//////////////////////////////////////////////////////////////////////////////////

// ��ͷ�ؼ�
class SKIN_CONTROLS_EXPORT CSkinHeaderCtrl : public CHeaderCtrl
{
public:
	// ���캯��
	CSkinHeaderCtrl();
	// ��������
	virtual ~CSkinHeaderCtrl();

	// ���غ���
public:
	// �ؼ���
	virtual void PreSubclassWindow();
	// �ؼ���Ϣ
	virtual BOOL OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);

	// ���ܺ���
public:
	// ��������
	void SetLockCount(UINT uLockCount);
	// �����и�
	void SetItemHeight(UINT uItemHeight);
	// ������Դ
	void SetHeaderImage(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ������Դ
	void SetHeaderImage(BYTE* pBuffer, DWORD dwBufferSize, BitmapFormat bf = BF_UNKNOWN);
	// ������Դ
	void SetHeaderImage(LPCTSTR lpszResource, HINSTANCE hInst, BitmapFormat bf = BF_UNKNOWN);

	// ��Ϣ����
protected:
	// �ػ���Ϣ
	void OnPaint();
	// �滭����
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()

	// �����б�
protected:
	UINT m_uLockCount; // ������Ŀ
	UINT m_uItemHeight; // ����߶�
	COLORREF m_crTitle; // ������ɫ
	CSkinImage m_ImageHead; // ͷ����Դ
};

//////////////////////////////////////////////////////////////////////////////////

// �б�ؼ�
class SKIN_CONTROLS_EXPORT CSkinListCtrl : public CListCtrl
{
public:
	// ���캯��
	CSkinListCtrl();
	// ��������
	virtual ~CSkinListCtrl();

	// ���غ���
public:
	// �ؼ���
	virtual VOID PreSubclassWindow();
	// �滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// ���Ҫ�ı��и� �����ڸ����ڵ�OnMeasureItem�е���
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	// ���뺯��
public:
	// ��������
	virtual void InsertDataItem(void* pItemData);
	// ��������
	virtual void UpdateDataItem(void* pItemData);
	// ɾ������
	virtual void DeleteDataItem(void* pItemData);

	// ���ݿ���
protected:
	// ��ȡλ��
	virtual int GetInsertIndex(void* pItemData);
	// ��������
	virtual int SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	// �����ַ�
	virtual void ConstructString(void* pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	// �滭����
protected:
	// ��ȡ��ɫ
	virtual void GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF& crColorText, COLORREF& crColorBack);
	// �滭����
	virtual void DrawCustomItem(CDC* pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect& rcSubItem, int nColumnIndex);

	// ��Ϣ����
protected:
	// �滭����
	BOOL OnEraseBkgnd(CDC* pDC);
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ����ƶ�
	void OnMouseMove(UINT nFlags, CPoint point);

	// ��̬����
protected:
	// ���к���
	static int CALLBACK SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList);

	// ��Ϣӳ��
protected:
	// ����б�
	void OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

	// ��������
public:
	CSkinHeaderCtrl m_SkinHeaderCtrl; // ��ͷ�ؼ�
	CSkinScrollBar m_SkinScrollBar; // ��������

	// ���ݱ���
protected:
	bool m_bAscendSort; // �����־
	COLORREF m_crTitle; // ������ɫ
	COLORREF m_crListTX; // �б�����
	COLORREF m_crListBK; // �б��ɫ
};

#endif // SKIN_LISTCTRL_H_