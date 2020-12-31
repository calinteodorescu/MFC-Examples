
#ifndef SKIN_RICHEDIT_H_
#define SKIN_RICHEDIT_H_
#pragma once

#include "SkinControlsDefine.h"
#include <RichOle.h>
#include "SkinScrollBar.h"

// ͼ�����
class SKIN_CONTROLS_EXPORT CImageDataObject : public IDataObject
{
public:
	// ���캯��
	CImageDataObject();
	// ��������
	virtual ~CImageDataObject();

	// �����ӿ�
public:
	// ��������
	virtual ULONG STDMETHODCALLTYPE AddRef();
	// �ͷŶ���
	virtual ULONG STDMETHODCALLTYPE Release();
	// ��ѯ�ӿ�
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);

	// ����ӿ�
public:
	// ��ȡ����
	virtual HRESULT STDMETHODCALLTYPE GetData(FORMATETC* pformatetcIn, STGMEDIUM* pStgMedium);
	// ��������
	virtual HRESULT STDMETHODCALLTYPE SetData(FORMATETC* pFormatEtc, STGMEDIUM* pStgMedium, BOOL fRelease);

	// �����ӿ�
public:
	virtual HRESULT STDMETHODCALLTYPE DUnadvise(DWORD dwConnection) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* pFormatEtc) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA** ppenumAdvise) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC * pFormatEtc, STGMEDIUM*  pStgMedium ) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc ) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC* pFormatEtcIn, FORMATETC * pFormatEtcOut) { return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC* pFormatEtc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection) { return E_NOTIMPL; }

	// ���ú���
public:
	// ����ͼƬ
	bool SetImage(HBITMAP hBitmap);
	// ����ͼƬ
	bool LoadImage(LPCTSTR pszFileName);
	// ����ͼƬ
	bool LoadImage(HINSTANCE hInstance, LPCTSTR lpBitmapName);

	// ͼƬ����
public:
	// ��ȡͼƬ
	HBITMAP GetBitmap() { return m_StgMedium.hBitmap; }
	// ��ȡ��Ϣ
	bool IntercalateReObject(REOBJECT& ReObject, IOleClientSite* pIOleClientSite, WORD wIndex);

	// �ں˱���
private:
	ULONG m_ulRefCnt; // ���ü���
	STGMEDIUM m_StgMedium; // ������Ϣ
	FORMATETC m_FormatEtc; // ������Ϣ

	// �ӿڱ���
private:
	IStorage* m_pIStorage; // ����ӿ�
	IOleObject* m_pIOleObject; // ����ӿ�
};

//////////////////////////////////////////////////////////////////////////////////

// ��Ϣ����
class SKIN_CONTROLS_EXPORT CSkinRichEdit : public CRichEditCtrl
{
public:
	// ���캯��
	CSkinRichEdit();
	// ��������
	virtual ~CSkinRichEdit();

	// ���غ���
protected:
	// �ؼ���
	virtual void PreSubclassWindow();

	// ���ܺ���
public:
	// �����Ļ
	bool CleanScreen();
	// ������Ϣ
	bool LoadMessage(LPCTSTR pszFileName);
	// ������Ϣ
	bool SaveMessage(LPCTSTR pszFileName);
	// �����Ҽ��˵�
	void EnableRightMenu(bool bEnable) { m_bEnableMenu = bEnable; }

	// ͼƬ����
public:
	// ����ͼƬ
	bool InsertImage(LPCTSTR pszImage, WORD wIndex);
	// ����ͼƬ
	bool InsertImage(CBitmap* pBitmap, WORD wIndex);
	// �������
	bool InsertDataObject(CImageDataObject* pDataObject, WORD wIndex);

	// �ִ�����
public:
	// �����ִ�
	bool InsertString(LPCTSTR pszString, COLORREF crTextColor);
	// �����ִ�
	bool InsertString(LPCTSTR pszString, COLORREF crTextColor, COLORREF crBackColor);

	// ��������
public:
	// �����ִ�
	bool InsertHyperLink(LPCTSTR pszString);
	// �����ִ�
	bool InsertHyperLink(LPCTSTR pszString, COLORREF crBackColor);
	// �滻OLE
	void ReplaceREOBJ();

	// ��������
protected:
	// �����ִ�
	bool InsertString(LPCTSTR pszString, CHARFORMAT2& CharFormat);
	// ���ػص�
	static DWORD CALLBACK LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
	// ����ص�
	static DWORD CALLBACK SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

	// �ڲ�����
protected:
	// �ַ��ж�
	bool EfficacyUrlChar(TCHAR chChar);
	// ��ַ�ж�
	bool EfficacyUrlString(LPCTSTR pszUrl);

	// �˵�����
protected:
	// ɾ����Ϣ
	void OnClearAll();
	// ȫ��ѡ��
	void OnSelectAll();
	// �����ַ�
	void OnCopyString();
	// ������Ϣ
	void OnSaveString();
	// ɾ���ַ�
	void OnDeleteString();

	// ��Ϣӳ��
protected:
	// ������Ϣ
	void OnDestroy();
	// ������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// �Ҽ���Ϣ
	void OnRButtonDown(UINT nFlags, CPoint point);
	// ���ù��
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

	// �ӿڱ���
protected:
	IRichEditOle* m_pIRichEditOLE; // �ӿ�ָ��
	bool m_bEnableMenu; // ����˵�
	CSkinScrollBar m_SkinScrollBar; // ��������
};

#endif // SKIN_RICHEDIT_H_