// DlgSpatilQuery.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mapdemo.h"
#include "DlgSpatilQuery.h"
#include "mapdemoDlg.h"

// CDlgSpatilQuery �Ի���

IMPLEMENT_DYNAMIC(CDlgSpatilQuery, CDialog)

CDlgSpatilQuery::CDlgSpatilQuery( IMXObjectPtr & pMap, CWnd* pParent)
	: CDialog(CDlgSpatilQuery::IDD, pParent)
	, m_dis(0)
    , m_pMap(pMap)
{

}

CDlgSpatilQuery::~CDlgSpatilQuery()
{
    if (m_pMap)
    {
        m_pMap.Release();
    }
}

void CDlgSpatilQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SRCSET, m_cmbSrcSet);
	DDX_Control(pDX, IDC_COMBO_OPSET, m_cmbOpSet);
	DDX_Control(pDX, IDC_COMBO_OPTYPE, m_cmbOpType);
	DDX_Text(pDX, IDC_EDIT_DIS, m_dis);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_cmbUnit);
}


BEGIN_MESSAGE_MAP(CDlgSpatilQuery, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSpatilQuery::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_OPSET, &CDlgSpatilQuery::OnCbnSelchangeComboOpset)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTYPE, &CDlgSpatilQuery::OnCbnSelchangeComboOptype)
END_MESSAGE_MAP()


// CDlgSpatilQuery ��Ϣ�������

void CDlgSpatilQuery::OnBnClickedOk()
{
 
	if(NULL == m_pMap)
    {
        return;
    }
    IMXLayersPtr playrs = m_pMap->Getlayers();//��ȡͼ���б�
    if (playrs == NULL)
    {
        return;
    }

	UpdateData();
	int n = 0;
	if((n = m_cmbSrcSet.GetCurSel()) < 0)
	{
		return;
	}
    //�������ݼ�
	int m = m_cmbOpSet.GetCurSel();
	IMXDatasetPtr ds = NULL;
	if(m == 0)
	{//ѡ��
        IMXVectLayerPtr pSelLayer = NULL;
        for(int i = 0,n = playrs->GetLayerCount();i < n;i++)
        {
            IMXLayerPtr pLayer = playrs->GetLayer(i);//����������ȡͼ��

            if(pLayer->GetLayerType() != mxLayerVector)//�Ƿ�Ϊʸ��ͼ��
                continue;
            IMXVectLayerPtr pVectLayer = pLayer;
            if (pVectLayer->GetSelectFeatureCount() > 0)
            {
                pSelLayer = pVectLayer;//��ѡͼ��
                break;
            }
            pLayer.Release();
            pVectLayer.Release();
        }
        if (pSelLayer)
        {	
            ds = pSelLayer->GetSelectFeatures();//��ȡѡ��
            pSelLayer.Release();
        }

	}
	else
	{//ͼ����
		CString strOpLayer;
		m_cmbOpSet.GetLBText(m, strOpLayer);
		IMXVectLayerPtr pOpLayer = playrs->GetLayerByName(strOpLayer.GetString());//�������ƻ�ȡͼ��
		if(NULL == pOpLayer)
        { 
            return;
        }
		ds = pOpLayer->SearchAllFeature();//��ѯ����ͼԪ
        pOpLayer.Release();
	}
	//��ѯѡ��
	CString strSrcLayer;
	m_cmbSrcSet.GetLBText(n,strSrcLayer);
	IMXVectLayerPtr pSrcLayer = playrs->GetLayerByName(strSrcLayer.GetString());//�������ƻ�ȡͼ��
	if(NULL == pSrcLayer)
    {
        return;
    }
    //�������ѡ��
    for(int i = 0,n = playrs->GetLayerCount();i < n;i++)
    {
        IMXLayerPtr pLayer = playrs->GetLayer(i);//����������ȡͼ��

        if(pLayer->GetLayerType() != mxLayerVector)//�Ƿ�Ϊʸ��ͼ��
            continue;
        IMXVectLayerPtr pVectLayer = pLayer;
        pVectLayer->RemoveAllSelect();//���ѡ��
        pLayer.Release();
        pVectLayer.Release();
    }
    //��ѯ�������ѡ����
	int t = m_cmbOpType.GetCurSel();
	MXSearchTypeConstants sc = (MXSearchTypeConstants)m_cmbOpType.GetItemData(t);
	if(sc == mxSearchTypeDistanceWithIn)
	{
		MXMapUnit u = (MXMapUnit)m_cmbUnit.GetCurSel();
		IMXFeaturePtr ft = NULL;
		while(ft = ds->GetNext())
		{//������ʼѡ�񼯵�ÿ��ͼԪ
			IMXDatasetPtr ds2 = pSrcLayer->SearchByDistance(ft, m_dis, u);//�������ѯ
			IMXFeaturePtr ft2 = NULL;
			while(ft2 = ds2->GetNext())
			{//�����������ÿ��ͼԪ
				pSrcLayer->SelectFeature(ft2->GetGeoId());//ѡ��ͼԪ
			    ft2.Release();
			}
            ds2.Release();
			ft.Release();
		}
	}
	else
	{
		IMXFeaturePtr ft = NULL;
 
		while(ft = ds->GetNext())
		{//������ʼѡ�񼯵�ÿ��ͼԪ
			IMXDatasetPtr ds2 = pSrcLayer->SearchByFeature(ft,_T(""),sc);//����ͼԪ���пռ��ѯ
			IMXFeaturePtr ft2 = NULL;
			while(ft2 = ds2->GetNext())
			{//�����������ÿ��ͼԪ
				pSrcLayer->SelectFeature(ft2->GetGeoId());//ѡ��ͼԪ
				ft2.Release();
			}
            ds2.Release();
			ft.Release();
		}
 
	}
    ds.Release();
    pSrcLayer.Release();
	m_pMap->FreshExtern();//ˢ��ѡ��ͼԪ
	OnOK();
}
BOOL CDlgSpatilQuery::OnInitDialog()
{
	CDialog::OnInitDialog();
  
	int n = m_cmbOpType.AddString(L"Intersects(�ཻ)");
	m_cmbOpType.SetItemData(n,mxSearchTypeIntersects);
 
	n = m_cmbOpType.AddString(L"Contains(����)");
	m_cmbOpType.SetItemData(n,mxSearchTypeContains);
 
	n = m_cmbOpType.AddString(L"Within(�Ȳ�)");
	m_cmbOpType.SetItemData(n,mxSearchTypeWithin);
    
 	n = m_cmbOpType.AddString(L"Touches(����)");
	m_cmbOpType.SetItemData(n,mxSearchTypeTouches);
 
	n = m_cmbOpType.AddString(L"Disjoint(����)");
	m_cmbOpType.SetItemData(n,mxSearchTypeDisjoint);
     
	n = m_cmbOpType.AddString(L"DisanceWithIn(���뷶Χ��)");
	m_cmbOpType.SetItemData(n,mxSearchTypeDistanceWithIn);
	m_cmbOpType.SetCurSel(0);
 
    //MXMapUnit
 	m_cmbUnit.AddString(L"Ӣ��");// mxUnitMile = 0,
 	m_cmbUnit.AddString(L"����");//mxUnitKilometer = 1,
 	m_cmbUnit.AddString(L"Ӣ��");//mxUnitInch = 2,
 	m_cmbUnit.AddString(L"����");//mxUnitMillimeter = 3,
 	m_cmbUnit.AddString(L"����");//mxUnitCentimeter = 4,
 	m_cmbUnit.AddString(L"��");  //mxUnitMeter = 5, 
	m_cmbUnit.AddString(L"��");  //mxUnitDegree = 6
	m_cmbUnit.AddString(L"����");//mxUnitDegree = 6

	m_cmbUnit.SetCurSel(1);

	CheckDlgButton(IDC_CHECK_SELECT,TRUE);
 
	m_cmbOpSet.AddString(L"ѡ��");
    
    if (m_pMap)
    {
        IMXLayersPtr playrs = m_pMap->Getlayers();//��ȡͼ���б�
        if (playrs != NULL)
        {
            for(int i = 0,n = playrs->GetLayerCount();i < n;i++)
            {
                IMXLayerPtr pLayer = playrs->GetLayer(i);//����������ȡͼ��

                if(pLayer->GetLayerType() != mxLayerVector)//�Ƿ�Ϊʸ��ͼ��
                    continue;
                m_cmbOpSet.AddString(pLayer->GetLayerName());

                m_cmbSrcSet.AddString(pLayer->GetLayerName());
                pLayer.Release();
            }
        }
        playrs.Release();
    }
	m_cmbOpSet.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgSpatilQuery::OnCbnSelchangeComboOpset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgSpatilQuery::OnCbnSelchangeComboOptype()
{
	int nSel = m_cmbOpType.GetCurSel();
	if(nSel < 0)return;
	MXSearchTypeConstants sc = (MXSearchTypeConstants)m_cmbOpType.GetItemData(nSel);
	if(sc == mxSearchTypeDistanceWithIn)
	{
		GetDlgItem(IDC_EDIT_DIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_UNIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_UNIT)->EnableWindow(FALSE);
	}
}
