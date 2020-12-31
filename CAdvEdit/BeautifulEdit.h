/***************************************************************************/
/*                        ������CBeautifulEdit                             */
/*				 	   (C)CopyRight www.cguage.com		     			   */
/***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
//���ࣺCEdit
//���ܣ�ʹEdit�ؼ��б���ͼ�񣬺�����Ϊ��ʱ��ʾָ���Ļ�ɫ��Ϣ���֣������ö�Ӧ
//		����ɫ�Ĺ���
//���ߣ�C�ϸ�(admin@cguage.com)
//�汾��V1.0(2010��4��14��2:31:20)
//		V1.1(    �� ��  ��        )
//��Ҫ������
//	void AddTipString(CString strTip);	/*���á���������*/
//	void AddBitmap(UINT nIDBitmap);		/*���ñ���ͼ��*/
//										/*�������ֺ���ʾɫ:*/
//	void SetColors(COLORREF clrText = RGB(0, 0, 0),		
//						COLORREF clrTip = RGB(180, 180, 180));
//			
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEAUTIFULEDIT_H__CA359AF1_C2BC_4DA0_BE11_11802673AE54__INCLUDED_)
#define AFX_BEAUTIFULEDIT_H__CA359AF1_C2BC_4DA0_BE11_11802673AE54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BeautifulEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBeautifulEdit window

class CBeautifulEdit : public CEdit
{
// Construction
public:
	CBeautifulEdit(CString strTip = _T(""), UINT nIDBitmap = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeautifulEdit)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddTipString(CString strTip);	/*���á���������*/
	void AddBitmap(UINT nIDBitmap);		/*���ñ���ͼ��*/
										/*�������ֺ���ʾɫ:*/
	void SetColors(COLORREF clrText = RGB(0, 0, 0),		
					COLORREF clrTip = RGB(180, 180, 180));
	virtual ~CBeautifulEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBeautifulEdit)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bIsTxtChangeByUsr;
	COLORREF m_clrTip;		//Tip���ֵ�RGB��ɫ
	COLORREF m_clrText;		//Text(�ı�)��RGB��ɫ
	CBrush m_brhWhite;		//��ɫ��ˢ
	CBrush m_brhNull;		//�ջ�ˢ
	BOOL m_bIsEmpty;		//(�û��༭��)Text�Ƿ�Ϊ��
	int m_bBkImg;			//�Ƿ��б���ͼ��
	BOOL m_bIsFocus;		//�Ƿ��ý���
	CString m_strTip;		//Tip��������(���磺�����Ա༭�ؼ�)
	CBitmap m_bitmap;		//����ͼ��
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEAUTIFULEDIT_H__CA359AF1_C2BC_4DA0_BE11_11802673AE54__INCLUDED_)
