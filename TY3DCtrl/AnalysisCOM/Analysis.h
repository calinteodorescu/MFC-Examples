// Analysis.h : CAnalysis ������

#pragma once
#include "resource.h"       // ������



#include "AnalysisCOM_i.h"
#include "_IAnalysisEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CAnalysis

class ATL_NO_VTABLE CAnalysis :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAnalysis, &CLSID_Analysis>,
	public IConnectionPointContainerImpl<CAnalysis>,
	public CProxy_IAnalysisEvents<CAnalysis>,
	public IDispatchImpl<IAnalysis, &IID_IAnalysis, &LIBID_AnalysisCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CAnalysis()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ANALYSIS)


BEGIN_COM_MAP(CAnalysis)
	COM_INTERFACE_ENTRY(IAnalysis)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CAnalysis)
	CONNECTION_POINT_ENTRY(__uuidof(_IAnalysisEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



};

OBJECT_ENTRY_AUTO(__uuidof(Analysis), CAnalysis)
