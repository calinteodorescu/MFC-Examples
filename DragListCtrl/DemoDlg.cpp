#include "stdafx.h"
#include "Resource.h"
#include "DemoDlg.h"



//�������Ϊͨ�ð汾
#ifdef	_UNICODE
#define titoa _itow	
#else	
#define titoa	_itoa
#endif	



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDemoDlg::CDemoDlg(CWnd* pParent)
	: CDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl2);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl3);
	DDX_Control(pDX, IDC_LIST4, m_ListBox1);
	DDX_Control(pDX, IDC_LIST5, m_ListBox2);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);			// Set big icon
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE); //���ô����
 	static CLoadMyLibrary inst(m_hWnd);
	//-----------------------------------------------------------------------------

/****************************************************************************
��ʼ��list1 ������
****************************************************************************/

	//������չ��ʽ
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();		//��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT;				//ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES;					//�����ߣ�report���ʱ��
//	dwStyle |= LVS_EX_CHECKBOXES;					//itemǰ����checkbox�ؼ�
//	dwStyle |= LVS_SHOWSELALWAYS;			//��Ҫ���������ã��ڿؼ�����������,������ʾ��������
//	dwStyle |= LVS_SHAREIMAGELISTS			//���һ��������������Listctrlͬ��һ��HIMAGELIST��������һ���ݻٲ���Ӱ������һ����Specifies that the control does not take ownership of the image lists assigned to it (that is, it does not destroy the image lists when it is destroyed). This style enables the same image lists to be used with multiple list view controls.
	m_ListCtrl.SetExtendedStyle(dwStyle);				//������չ���

	SHFILEINFO shFinfo;
	//��ȡϵͳСͼ��
	HIMAGELIST hImageListSmall=(HIMAGELIST)::SHGetFileInfo(_T("c:\\"),0,&shFinfo,sizeof(SHFILEINFO),
						SHGFI_SYSICONINDEX|	SHGFI_SMALLICON);

	//��ȡϵͳ��ͼ��
	 HIMAGELIST hImageListLarge=(HIMAGELIST)::SHGetFileInfo(_T(""),0,&shFinfo,sizeof(SHFILEINFO),
						SHGFI_SYSICONINDEX|	SHGFI_ICON);
	
	m_ListCtrl.SetImageList(CImageList::FromHandle(hImageListSmall),LVSIL_SMALL);
	m_ListCtrl.SetImageList(CImageList::FromHandle(hImageListLarge),LVSIL_NORMAL);
	
	//������
	m_ListCtrl.InsertColumn(0,   _T("��һ��"),   LVCFMT_LEFT,   60);   
	m_ListCtrl.InsertColumn(1,   _T("�ڶ���"),   LVCFMT_LEFT,   60);   
	m_ListCtrl.InsertColumn(2,   _T("������"),   LVCFMT_LEFT,   100);   

	TCHAR buf[20];
	for(int j=0;j<30;j++)
	{
		titoa(j,buf,10);
		
		//�������
		int nRow = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), buf,0);//����1��"11"�����0�е����� ����(����,����,ͼ������)
		titoa(j+100,buf,10);
		m_ListCtrl.SetItemText(nRow, 1, buf);	
		titoa(j+1000,buf,10);
		m_ListCtrl.SetItemText(nRow, 2, _T("CtrlList1"));		
	}
	


/****************************************************************************
��ʼ��list2 ������
****************************************************************************/

	{
		//������չ��ʽ
		DWORD dwStyle = m_ListCtrl2.GetExtendedStyle();		//��ȡ��ǰ��չ��ʽ
		dwStyle |= LVS_EX_FULLROWSELECT;				//ѡ��ĳ��ʹ���и�����report���ʱ��
		dwStyle |= LVS_EX_GRIDLINES;					//�����ߣ�report���ʱ��
		//	dwStyle |= LVS_EX_CHECKBOXES;					//itemǰ����checkbox�ؼ�
		//	dwStyle |= LVS_SHOWSELALWAYS;			//��Ҫ���������ã��ڿؼ�����������,������ʾ��������
		//	dwStyle |= LVS_SHAREIMAGELISTS			//���һ��������������Listctrlͬ��һ��HIMAGELIST��������һ���ݻٲ���Ӱ������һ����Specifies that the control does not take ownership of the image lists assigned to it (that is, it does not destroy the image lists when it is destroyed). This style enables the same image lists to be used with multiple list view controls.
		m_ListCtrl2.SetExtendedStyle(dwStyle);				//������չ���

		SHFILEINFO shFinfo;
		//��ȡϵͳСͼ��
		HIMAGELIST hImageListSmall=(HIMAGELIST)::SHGetFileInfo(_T("c:\\"),0,&shFinfo,sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX|	SHGFI_SMALLICON);

		//��ȡϵͳ��ͼ��
		HIMAGELIST hImageListLarge=(HIMAGELIST)::SHGetFileInfo(_T(""),0,&shFinfo,sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX|	SHGFI_ICON);

		m_ListCtrl2.SetImageList(CImageList::FromHandle(hImageListSmall),LVSIL_SMALL);
		m_ListCtrl2.SetImageList(CImageList::FromHandle(hImageListLarge),LVSIL_NORMAL);

		//������
		m_ListCtrl2.InsertColumn(0,   _T("��һ��"),   LVCFMT_LEFT,   60);   
		m_ListCtrl2.InsertColumn(1,   _T("�ڶ���"),   LVCFMT_LEFT,   60);   
		m_ListCtrl2.InsertColumn(2,   _T("������"),   LVCFMT_LEFT,   100);   

		TCHAR buf[20];
		for(int j=0;j<3;j++)
		{
			titoa(j,buf,10);

			//�������
			int nRow = m_ListCtrl2.InsertItem(m_ListCtrl2.GetItemCount(), buf,0);//����1��"11"�����0�е����� ����(����,����,ͼ������)
			titoa(j+200,buf,10);
			m_ListCtrl2.SetItemText(nRow, 1, buf);	
			titoa(j+1000,buf,10);
			m_ListCtrl2.SetItemText(nRow, 2, _T("CtrlList2"));		
		}

	}


/****************************************************************************
��ʼ��list3 ������
****************************************************************************/

	{
		//������չ��ʽ
		DWORD dwStyle = m_ListCtrl3.GetExtendedStyle();		//��ȡ��ǰ��չ��ʽ
		dwStyle |= LVS_EX_FULLROWSELECT;				//ѡ��ĳ��ʹ���и�����report���ʱ��
		dwStyle |= LVS_EX_GRIDLINES;					//�����ߣ�report���ʱ��
		//	dwStyle |= LVS_EX_CHECKBOXES;					//itemǰ����checkbox�ؼ�
		//	dwStyle |= LVS_SHOWSELALWAYS;			//��Ҫ���������ã��ڿؼ�����������,������ʾ��������
		//	dwStyle |= LVS_SHAREIMAGELISTS			//���һ��������������Listctrlͬ��һ��HIMAGELIST��������һ���ݻٲ���Ӱ������һ����Specifies that the control does not take ownership of the image lists assigned to it (that is, it does not destroy the image lists when it is destroyed). This style enables the same image lists to be used with multiple list view controls.
		m_ListCtrl3.SetExtendedStyle(dwStyle);				//������չ���

		SHFILEINFO shFinfo;
		//��ȡϵͳСͼ��
		HIMAGELIST hImageListSmall=(HIMAGELIST)::SHGetFileInfo(_T("c:\\"),0,&shFinfo,sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX|	SHGFI_SMALLICON);

		//��ȡϵͳ��ͼ��
		HIMAGELIST hImageListLarge=(HIMAGELIST)::SHGetFileInfo(_T(""),0,&shFinfo,sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX|	SHGFI_ICON);

		m_ListCtrl3.SetImageList(CImageList::FromHandle(hImageListSmall),LVSIL_SMALL);
		m_ListCtrl3.SetImageList(CImageList::FromHandle(hImageListLarge),LVSIL_NORMAL);

		//������
		m_ListCtrl3.InsertColumn(0,   _T("��һ��"),   LVCFMT_LEFT,   60);   
		m_ListCtrl3.InsertColumn(1,   _T("�ڶ���"),   LVCFMT_LEFT,   60);   
		m_ListCtrl3.InsertColumn(2,   _T("������"),   LVCFMT_LEFT,   100);   

		TCHAR buf[20];
		for(int j=0;j<30;j++)
		{
			titoa(j,buf,10);

			//�������
			int nRow = m_ListCtrl3.InsertItem(m_ListCtrl3.GetItemCount(), buf,0);//����1��"11"�����0�е����� ����(����,����,ͼ������)
			titoa(j+300,buf,10);
			m_ListCtrl3.SetItemText(nRow, 1, buf);	
			titoa(j+1000,buf,10);
			m_ListCtrl3.SetItemText(nRow, 2, _T("CtrlList3"));		
		}

	}

/****************************************************************************

****************************************************************************/
	{
		TCHAR buf[20];
		for(int j=0;j<30;j++)
		{
			titoa(j,buf,10);
			m_ListBox1.AddString(buf);

			_tcscat(buf,_T("  List2"));
			m_ListBox2.AddString(buf);
		}
	}

	return TRUE;  
}






