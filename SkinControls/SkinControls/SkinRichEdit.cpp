
#include "Stdafx.h"
#include "SkinMenu.h"
#include "SkinRichEdit.h"
#include "SkinImage.h"

// ���ֿؼ�
using namespace ImageOleLib;

//////////////////////////////////////////////////////////////////////////////////

// �˵�����
#define IDM_CLEAR_ALL				(WM_USER+201)						//ɾ����Ϣ
#define IDM_SELECT_ALL				(WM_USER+202)						//ȫ��ѡ��
#define IDM_COPY_STRING				(WM_USER+203)						//�����ַ�
#define IDM_SAVE_STRING				(WM_USER+205)						//������Ϣ
#define IDM_DELETE_STRING			(WM_USER+204)						//ɾ���ַ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinRichEdit, CRichEditCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	//�˵�����
	ON_COMMAND(IDM_CLEAR_ALL, OnClearAll)
	ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDM_COPY_STRING, OnCopyString)
	ON_COMMAND(IDM_SAVE_STRING, OnSaveString)
	ON_COMMAND(IDM_DELETE_STRING, OnDeleteString)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

// ���캯��
CImageDataObject::CImageDataObject()
{
	// �ں˱���
	m_ulRefCnt=0;
	ZeroMemory(&m_StgMedium,sizeof(m_StgMedium));
	ZeroMemory(&m_FormatEtc,sizeof(m_FormatEtc));

	// ���ýӿ�
	m_pIStorage=NULL;
	m_pIOleObject=NULL;

	return;
}

// ��������
CImageDataObject::~CImageDataObject()
{
	// �ͷŶ���
	::ReleaseStgMedium(&m_StgMedium);

	// �ͷŶ���
	if (m_pIOleObject!=NULL) m_pIOleObject->Release();
	if (m_pIStorage!=NULL) m_pIStorage->Release();

	return;
}

// ��������
ULONG STDMETHODCALLTYPE CImageDataObject::AddRef()
{
	m_ulRefCnt++;
	return m_ulRefCnt;
}

// �ͷŶ���
ULONG STDMETHODCALLTYPE CImageDataObject::Release()
{
	if (--m_ulRefCnt==0) delete this;
	return m_ulRefCnt;
}

// ��ѯ�ӿ�
HRESULT STDMETHODCALLTYPE CImageDataObject::QueryInterface(REFIID iid, void** ppvObject)
{
	//��ѯ�ӿ�
	if (iid==IID_IUnknown||iid==IID_IDataObject)
	{
		*ppvObject=this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

// ��ȡ����
HRESULT STDMETHODCALLTYPE CImageDataObject::GetData(FORMATETC* pFormatEtcIn, STGMEDIUM* pStgMedium) 
{
	// ״̬�ж�
	ASSERT(m_StgMedium.hBitmap!=NULL);
	if (m_StgMedium.hBitmap==NULL) return E_HANDLE;

	// ��ȡ����
	HANDLE hHandle=OleDuplicateData(m_StgMedium.hBitmap,CF_BITMAP,0);
	if (hHandle==NULL) return E_HANDLE;

	// ���ñ���
	pStgMedium->tymed=TYMED_GDI;
	pStgMedium->pUnkForRelease=NULL;
	pStgMedium->hBitmap=(HBITMAP)hHandle;

	return S_OK;
}

// ��������
HRESULT STDMETHODCALLTYPE CImageDataObject::SetData(FORMATETC* pFormatEtc, STGMEDIUM* pStgMedium, BOOL fRelease) 
{
	//���ñ���
	m_FormatEtc=*pFormatEtc;
	m_StgMedium=*pStgMedium;

	return S_OK;
}

// ����ͼƬ
bool CImageDataObject::SetImage(HBITMAP hBitmap)
{
	// Ч��״̬
	ASSERT((m_pIOleObject==NULL)&&(m_pIStorage==NULL));
	if ((m_pIOleObject!=NULL)||(m_pIStorage!=NULL)) return false;

	// ������Ϣ
	m_StgMedium.tymed=TYMED_GDI;
	m_StgMedium.hBitmap=hBitmap;
	m_StgMedium.pUnkForRelease=NULL;

	// ������Ϣ
	m_FormatEtc.ptd=NULL;
	m_FormatEtc.lindex=-1;
	m_FormatEtc.tymed=TYMED_GDI;
	m_FormatEtc.cfFormat=CF_BITMAP;
	m_FormatEtc.dwAspect=DVASPECT_CONTENT;

	// ��������
	try
	{
		// ��������
		LPLOCKBYTES lpLockBytes=NULL;

		// �����洢
		if (FAILED(CreateILockBytesOnHGlobal(NULL,TRUE,&lpLockBytes)))
		{
			ASSERT(FALSE);
			throw 0;
		}

		// ��������
		if (FAILED(StgCreateDocfileOnILockBytes(lpLockBytes,STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,0,&m_pIStorage)))
		{
			ASSERT(FALSE);
			throw 0;
		}

		// д������
		if (FAILED(OleCreateStaticFromData(this,IID_IOleObject,OLERENDER_FORMAT,&m_FormatEtc,NULL,m_pIStorage,(void * *)&m_pIOleObject)))
		{
			ASSERT(FALSE);
			throw 0;
		}

		// ���ö���
		if (FAILED(OleSetContainedObject(m_pIOleObject,TRUE)))
		{
			ASSERT(FALSE);
			throw 0;
		}

		return true;
	}
	catch (...)
	{
		// �ͷŶ���
		if (m_pIStorage != NULL)
		{
			m_pIStorage->Release();
			m_pIStorage = NULL;
		}
		if (m_pIOleObject != NULL)
		{
			m_pIOleObject->Release();
			m_pIOleObject = NULL;
		}
	}

	return false;
}

// ����ͼƬ
bool CImageDataObject::LoadImage(LPCTSTR pszFileName)
{
	// ����ͼƬ
	HBITMAP hbmp = CSkinImage::LoadBitmapFromFile(pszFileName, NULL);

	// ����ж�
	if (hbmp == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	// ����ͼƬ
	SetImage(hbmp);

	return true;
}

// ����ͼƬ
bool CImageDataObject::LoadImage(HINSTANCE hInstance, LPCTSTR lpBitmapName)
{
	HBITMAP hbmp = CSkinImage::LoadBitmapFromResource(hInstance, lpBitmapName, NULL);

	// ����ж�
	if (hbmp == NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	// ����ͼƬ
	SetImage(hbmp);

	return true;
}

// ��ȡ��Ϣ
bool CImageDataObject::IntercalateReObject(REOBJECT& ReObject, IOleClientSite* pIOleClientSite, WORD wIndex)
{
	// ״̬�ж�
	ASSERT(m_StgMedium.hBitmap!=NULL);
	if (m_StgMedium.hBitmap==NULL) return false;

	// Ч��״̬
	ASSERT((m_pIOleObject!=NULL)&&(m_pIStorage!=NULL));
	if ((m_pIOleObject==NULL)||(m_pIStorage==NULL)) return false;

	// ���ñ���
	ZeroMemory(&ReObject,sizeof(ReObject));

	// �ӿ���Ϣ
	ReObject.pstg=m_pIStorage;
	ReObject.poleobj=m_pIOleObject;
	ReObject.polesite=pIOleClientSite;

	// ������Ϣ
	ReObject.cbStruct=sizeof(ReObject);
	ReObject.cp=REO_CP_SELECTION;
	ReObject.dvaspect=DVASPECT_CONTENT;
	ReObject.dwUser = wIndex;

	// ��ȡ��ʶ
	m_pIOleObject->GetUserClassID(&ReObject.clsid);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

// ���캯��
CSkinRichEdit::CSkinRichEdit()
{
	// ���ñ���
	m_pIRichEditOLE=NULL;
	m_bEnableMenu = true;
}

// ��������
CSkinRichEdit::~CSkinRichEdit()
{
	// �ͷŶ���
	if (m_pIRichEditOLE != NULL) 
	{
		m_pIRichEditOLE->Release();
		m_pIRichEditOLE=NULL;
	}
}

// �ؼ���
void CSkinRichEdit::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	// ���ù���
	m_SkinScrollBar.InitScroolBar(this);

	// ��ʼ���ؼ�
	SetEventMask(ENM_LINK);
	SetOptions(ECOOP_OR,ECO_NOHIDESEL);

	// ��ȡ�ӿ�
	m_pIRichEditOLE=GetIRichEditOle();

	return;
}

// ������Ϣ
int CSkinRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	// ���ù���
	m_SkinScrollBar.InitScroolBar(this);

	// ��ʼ���ؼ�
	SetEventMask(ENM_LINK);
	SetOptions(ECOOP_OR,ECO_NOHIDESEL);

	// ��ȡ�ӿ�
	m_pIRichEditOLE=GetIRichEditOle();

	return 0;
}

// �����Ļ
bool CSkinRichEdit::CleanScreen()
{
	// �����Ļ
	SetSel(0,GetWindowTextLength());
	ReplaceSel(TEXT(""));

	return true;
}

// ������Ϣ
bool CSkinRichEdit::LoadMessage(LPCTSTR pszFileName)
{
	// Ч�����
	ASSERT(pszFileName!=NULL);
	if (pszFileName==NULL) return false;

	// ���ļ�
	CFile File;
	if (File.Open(pszFileName,CFile::modeRead,NULL)==FALSE) return false;

	// ��������
	EDITSTREAM EditStream;
	EditStream.pfnCallback=LoadCallBack;
	EditStream.dwCookie=(DWORD)(LONGLONG)(&File);

	// ��ȡ�ļ�
	StreamIn(SF_RTF,EditStream);

	return true;
}

// ������Ϣ
bool CSkinRichEdit::SaveMessage(LPCTSTR pszFileName)
{
	// ��������
	CString strFileName=pszFileName;

	// ���ļ�
	if (strFileName.IsEmpty()==true)
	{
		CFileDialog FileDlg(FALSE,TEXT("RTF"),0,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,TEXT("��Ϣ�ļ�(*.RTF)|*.RTF||"),this);
		if (FileDlg.DoModal()==IDOK) strFileName=FileDlg.GetPathName();
		else return true;
	}

	// ���ļ�
	CFile File;
	if (File.Open(strFileName,CFile::modeWrite|CFile::modeCreate,NULL)==FALSE) return false;

	// ��������
	EDITSTREAM EditStream;
	EditStream.pfnCallback=SaveCallBack;
	EditStream.dwCookie=(DWORD)(LONGLONG)(&File);

	// д���ļ�
	StreamOut(SF_RTF,EditStream);

	return true;
}

//����ͼƬ
bool CSkinRichEdit::InsertImage(LPCTSTR pszImage, WORD wIndex)
{
	// ��������
	LPOLEOBJECT	lpOleObject=NULL;
	LPOLECLIENTSITE pIOleClientSite=NULL;

	// ����״̬
	CHARRANGE CharRange;
	GetSel(CharRange.cpMin,CharRange.cpMax);

	// ��������
	bool bResumeSelect=(CharRange.cpMax!=CharRange.cpMin);

	try
	{
		// ��������
		IGifAnimatorPtr lpIGifAnimator;

		// ��������
		if (FAILED(lpIGifAnimator.CreateInstance(CLSID_GifAnimator)))
		{
			ASSERT(FALSE);
			throw 0L;
		}

		// ����ͼ��
		if (FAILED(lpIGifAnimator->LoadFromFile((LPCWSTR)(CT2CW(pszImage)))))
		{
			ASSERT(FALSE);
			throw 0L;
		}

		// ��ȡ����
		if (FAILED(lpIGifAnimator->QueryInterface(IID_IOleObject,(void**)&lpOleObject)))
		{
			ASSERT(FALSE);
			throw 0L;
		}

		// ��ѯ�ӿ�
		ASSERT(m_pIRichEditOLE!=NULL);
		m_pIRichEditOLE->GetClientSite(&pIOleClientSite);

		// ��������
		REOBJECT ReObject;
		ZeroMemory(&ReObject,sizeof(REOBJECT));

		// ���ñ���
		ReObject.cbStruct=sizeof(ReObject);	
		ReObject.dwUser=wIndex;
		ReObject.cp=REO_CP_SELECTION;
		ReObject.dwFlags=REO_BELOWBASELINE;
		ReObject.dvaspect=DVASPECT_CONTENT;
		ReObject.pstg=NULL;
		ReObject.poleobj=lpOleObject;
		ReObject.polesite=pIOleClientSite;

		// ����ѡ��
		SetSel(-1,-1);

		// �������
		m_pIRichEditOLE->InsertObject(&ReObject);

		// �ͷŶ���
		if (pIOleClientSite != NULL)
		{
			pIOleClientSite->Release();
			pIOleClientSite = NULL;
		}

		// ״̬����
		if (bResumeSelect==true)
		{
			SetSel(CharRange);
		}
		else
		{
			SendMessage(WM_VSCROLL,SB_BOTTOM,0);
		}

		// ���´���
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_UPDATENOW|RDW_INVALIDATE);

		return true;
	}
	catch (...)
	{
		// �ͷŶ���
		if (lpOleObject != NULL)
		{
			lpOleObject->Release();
			lpOleObject = NULL;
		}
		if (pIOleClientSite != NULL)
		{
			pIOleClientSite->Release();
			pIOleClientSite = NULL;
		}
	}

	return false;
}

// ����ͼƬ
bool CSkinRichEdit::InsertImage(CBitmap* pBitmap, WORD wIndex)
{
	// �������
	CImageDataObject ImageDataObject;
	ImageDataObject.SetImage((HBITMAP)pBitmap->GetSafeHandle());

	// ����ͼƬ
	InsertDataObject(&ImageDataObject, wIndex);

	return false;
}

// �������
bool CSkinRichEdit::InsertDataObject(CImageDataObject* pDataObject, WORD wIndex)
{
	// Ч�����
	ASSERT((m_pIRichEditOLE!=NULL)&&(pDataObject!=NULL));
	if ((m_pIRichEditOLE==NULL)||(pDataObject==NULL)) return false;

	// ����״̬
	CHARRANGE CharRange;
	GetSel(CharRange.cpMin,CharRange.cpMax);

	// ��������
	IOleClientSite * pIOleClientSite=NULL;
	bool bResumeSelect=(CharRange.cpMax!=CharRange.cpMin);

	// �����Ϣ
	REOBJECT ReObject;
	ZeroMemory(&ReObject,sizeof(ReObject));

	// ��ȡ����
	ASSERT(m_pIRichEditOLE!=NULL);
	m_pIRichEditOLE->GetClientSite(&pIOleClientSite);

	// ����ͼƬ
	if (pDataObject->IntercalateReObject(ReObject,pIOleClientSite, wIndex)==true)
	{
		// ����ͼƬ
		SetSel(-1L,-1L);
		m_pIRichEditOLE->InsertObject(&ReObject);

		// �ָ���Ϣ
		if (bResumeSelect==true)
		{
			SetSel(CharRange);
		}
		else
		{
			SendMessage(WM_VSCROLL,SB_BOTTOM,0);
		}
	}

	// �ͷŶ���
	if (pIOleClientSite != NULL)
	{
		pIOleClientSite->Release();
		pIOleClientSite = NULL;
	}

	return true;
}

// �����ִ�
bool CSkinRichEdit::InsertString(LPCTSTR pszString, COLORREF crTextColor)
{
	// ��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	// ��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=crTextColor;

	// �����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

// �����ִ�
bool CSkinRichEdit::InsertString(LPCTSTR pszString, COLORREF crTextColor, COLORREF crBackColor)
{
	// ��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	// ��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_BACKCOLOR|CFM_OFFSET;
	CharFormat.crTextColor=crTextColor;
	CharFormat.crBackColor=crBackColor;

	// �����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

// �����ִ�
bool CSkinRichEdit::InsertHyperLink(LPCTSTR pszString)
{
	// ��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	// ��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_LINK|CFM_OFFSET;
	CharFormat.dwEffects=CFE_LINK;

	// �����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

// �����ִ�
bool CSkinRichEdit::InsertHyperLink(LPCTSTR pszString, COLORREF crBackColor)
{
	// ��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	// ��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_BACKCOLOR|CFM_LINK;
	CharFormat.dwEffects=CFE_LINK;
	CharFormat.crBackColor=crBackColor;

	//�����ִ�
	InsertString(pszString,CharFormat);

	return true;
}

// ���ù��
BOOL CSkinRichEdit::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	// ���ù��
	if (nHitTest==HTCAPTION)
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�����ִ�
bool CSkinRichEdit::InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat)
{
	// ��������
	bool bResumeSelect;
	CHARRANGE CharRange;

	// ����״̬
	GetSel(CharRange.cpMin,CharRange.cpMax);
	bResumeSelect=(CharRange.cpMax!=CharRange.cpMin);

	// ��������
	LPCTSTR pszHttp=TEXT("http://");
	const int nHttpLength=lstrlen(pszHttp);
	const int nStringLength=lstrlen(pszString);

	// ��������
	int nStringStart=0;
	int nStringPause=0;

	// �ַ�����
	for (int i=0;i<nStringLength;i++)
	{
		// ��������
		int nUrlPause=i;

		// ��ַ�ж�
		if (((i+nHttpLength)<nStringLength)&&(memcmp(&pszString[i],pszHttp,nHttpLength*sizeof(TCHAR))==0))
		{
			// ��������
			nUrlPause=i+nHttpLength;

			// ��ַ����
			while (nUrlPause<nStringLength)
			{
				// �ַ��ж�
				if (EfficacyUrlChar(pszString[nUrlPause])==true)
				{
					nUrlPause++;
					continue;
				}

				break;
			}
		}

		// ��ֹ�ַ�
		if (nUrlPause<=(i+nHttpLength)) nStringPause=(i+1);

		// �����ַ�
		if ((i==(nStringLength-1))||(nUrlPause>(i+nHttpLength)))
		{
			// ��ͨ�ַ�
			if (nStringPause>nStringStart)
			{
				// ��ȡ����
				CString strNormalString;
				LPTSTR pszNormalString=strNormalString.GetBuffer(nStringPause-nStringStart+1);

				// �����ַ�
				pszNormalString[nStringPause-nStringStart]=0;
				CopyMemory(pszNormalString,&pszString[nStringStart],(nStringPause-nStringStart)*sizeof(TCHAR));

				// �ͷŻ���
				strNormalString.ReleaseBuffer();

				// ������Ϣ
				SetSel(-1L,-1L);
				SetWordCharFormat(CharFormat);
				ReplaceSel((LPCTSTR)strNormalString);
			}

			// ���ӵ�ַ
			if (nUrlPause>(i+nHttpLength))
			{
				// ��ȡ����
				CString strUrlString;
				LPTSTR pszUrlString=strUrlString.GetBuffer((nUrlPause-i)+1);

				// �����ַ�
				pszUrlString[nUrlPause-i]=0;
				CopyMemory(pszUrlString,&pszString[i],(nUrlPause-i)*sizeof(TCHAR));

				// �ͷŻ���
				strUrlString.ReleaseBuffer();

				// �����ʽ
				CHARFORMAT2 CharFormatHyper;
				CharFormatHyper=CharFormat;

				// Ч���ַ
				if (EfficacyUrlString(strUrlString)==true)
				{
					CharFormatHyper.dwMask|=CFM_LINK;
					CharFormatHyper.dwEffects|=CFE_LINK;
				}

				// ������Ϣ
				SetSel(-1L,-1L);
				SetWordCharFormat(CharFormatHyper);
				ReplaceSel((LPCTSTR)strUrlString);
			}

			//��������
			nStringStart=__max(i,nUrlPause);
		}

		// ��������
		i+=(nUrlPause-i);
	}

	// ״̬����
	if (bResumeSelect==true) SetSel(CharRange);
	else PostMessage(WM_VSCROLL,SB_BOTTOM,0);

	return true;
}

// ���ػص�
DWORD CALLBACK CSkinRichEdit::LoadCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	// ��ȡ�ļ�
	*pcb=((CFile *)(LONGLONG)(dwCookie))->Read(pbBuff,cb);

	return 0;
}

// ����ص�
DWORD CALLBACK CSkinRichEdit::SaveCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	// д���ļ�
	((CFile *)(LONGLONG)(dwCookie))->Write(pbBuff,cb);

	return 0;
}

// �ַ��ж�
bool CSkinRichEdit::EfficacyUrlChar(TCHAR chChar)
{
	// �����ַ�
	if (chChar==TEXT('.')) return true;
	if (chChar==TEXT('=')) return true;
	if (chChar==TEXT('+')) return true;
	if (chChar==TEXT('?')) return true;
	if (chChar==TEXT('#')) return true;
	if (chChar==TEXT('%')) return true;
	if (chChar==TEXT('/')) return true;
	if (chChar==TEXT(':')) return true;
	if (chChar==TEXT('&')) return true;

	// �ַ���Χ
	if ((chChar>=TEXT('a'))&&(chChar<=TEXT('z'))) return true;
	if ((chChar>=TEXT('A'))&&(chChar<=TEXT('Z'))) return true;
	if ((chChar>=TEXT('0'))&&(chChar<=TEXT('9'))) return true;

	return false;
}

// ��ַ�ж�
bool CSkinRichEdit::EfficacyUrlString(LPCTSTR pszUrl)
{
	return true;
}

// ������Ϣ
void CSkinRichEdit::OnDestroy()
{
	__super::OnDestroy();

	// �ͷŽӿ�
	if (m_pIRichEditOLE != NULL)
	{
		m_pIRichEditOLE->Release();
		m_pIRichEditOLE = NULL;
	}

	return;
}

// �Ҽ���Ϣ
void CSkinRichEdit::OnRButtonDown(UINT nFlags, CPoint Point)
{
	// ���ý���
	SetFocus();

	// ����˵�
	CSkinMenu FunctionMenu;
	FunctionMenu.CreatePopupMenu();

	// �����˵�
	FunctionMenu.AppendMenu(0, IDM_COPY_STRING, TEXT("����"));
	FunctionMenu.AppendMenu(0, IDM_SELECT_ALL, TEXT("ȫѡ"));
	FunctionMenu.AppendMenu(0, IDM_DELETE_STRING, TEXT("ɾ��"));
	FunctionMenu.AppendMenu(0, IDM_CLEAR_ALL, TEXT("�����Ļ"));
	//FunctionMenu.AppendSeparator();
	//FunctionMenu.AppendMenu(0, IDM_SAVE_STRING, TEXT("������Ϣ..."));

	// ��ֹ�˵�
	UINT nEnableMask=((GetSelectionType()!=SEL_EMPTY)?0:MF_GRAYED);
	FunctionMenu.EnableMenuItem(IDM_COPY_STRING,MF_BYCOMMAND|nEnableMask);
	FunctionMenu.EnableMenuItem(IDM_DELETE_STRING,MF_BYCOMMAND|nEnableMask);

	// ��ʾ�˵�
	ClientToScreen(&Point);
	FunctionMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, Point.x,Point.y,this);

	return;
}

// ɾ����Ϣ
void CSkinRichEdit::OnClearAll()
{
	// ɾ����Ϣ
	SetSel(0L,-1L);
	ReplaceSel(NULL);

	return;
}

// ȫ��ѡ��
void CSkinRichEdit::OnSelectAll()
{
	// ȫ��ѡ��
	SetSel(0L,-1L);

	return;
}

// �����ַ�
void CSkinRichEdit::OnCopyString()
{
	// �����ַ�
	Copy();

	return;
}

// ������Ϣ
void CSkinRichEdit::OnSaveString()
{
	// ������Ϣ
	SaveMessage(NULL);

	return;
}

// ɾ���ַ�
void CSkinRichEdit::OnDeleteString()
{
	// ɾ���ַ�
	ReplaceSel(TEXT(""));

	return;
}

