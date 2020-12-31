
#ifndef DUI_MANAGER_H_
#define DUI_MANAGER_H_
#pragma once

#include "SkinControlsDefine.h"

#include <afxtempl.h>

#define WM_DUI_MESSAGE				(WM_USER + 9430)			// �ؼ���Ϣ

class CDUIControl;

class SKIN_CONTROLS_EXPORT CDUIManager
{
public:
	CDUIManager();
	CDUIManager(CWnd* pOwnerWnd);
	~CDUIManager();

	// ���ܺ���
public:
	// ���ô���
	void SetOwnerWnd(CWnd* pOwnerWnd);
	// ������ڴ�С�ı� ���ҿؼ������˴��ҵ������µ��� ������OnSize������������
	void SetWndSize(int nWidth, int nHeight);
	const CSize& GetWndSize() const;
	CWnd* GetOwnerWnd() const;
	void SetFont(CFont* pFont);
	// ����Ƥ��
	bool UpdateSkinInfo(LPCTSTR lpszXML, LPCTSTR lpszWindow);
	// ��ӿؼ�
	void AddControl(CDUIControl* pControl);
	// �õ��ؼ�
	CDUIControl* GetControlFromID(const UINT uID);
	// �õ��ؼ�
	CDUIControl* GetControlFromName(LPCTSTR lpszName);
	bool ModifyNameHash(CDUIControl* pControl, LPCTSTR lpszOld, LPCTSTR lpszNew);
	// �Ƴ��ؼ�
	void RemoveControl(const UINT uID);
	void RemoveControl(LPCTSTR lpszName);
	void RemoveAllControls();
	// ������
	bool AddGroup(WORD wGroupID, CDUIControl* pControl);
	CPtrArray* GetGroup(WORD wGroupID);
	void RemoveGroup(WORD wGroupID, CDUIControl* pControl);
	void RemoveAllGroups();
	// �滭�ؼ�
	void PaintControls(CDC* pDC);
	// �������
	void OnMouseMove(const CPoint& point);
	void OnMouseLeave();
	bool OnLButtonDown(const CPoint& point);
	void OnLButtonUp(const CPoint& point);
	void OnRButtonDown(const CPoint& point);
	void OnRButtonUp(const CPoint& point);
	bool OnMouseCursor(const CPoint& point);

private:
	CArray<CDUIControl*> m_arrControls;
	CMap<CString, LPCTSTR, CDUIControl*, CDUIControl*> m_ControlsNameMap; // �ؼ���
	CMap<WORD, WORD, CPtrArray*, CPtrArray*> m_GroupsMap;
	CWnd* m_pOwnerWnd;
	CDUIControl* m_pHoverControl;
	CDUIControl* m_pClickControl;
	CSize m_szWindow;
};

#endif // DUI_MANAGER_H_


