// DataCache.h : CDataCache ������

#pragma once
#include "resource.h"       // ������



#include "DataSourceCOM_i.h"
#include "_IDataCacheEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CDataCache

class ATL_NO_VTABLE CDataCache :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataCache, &CLSID_DataCache>,
	public IConnectionPointContainerImpl<CDataCache>,
	public CProxy_IDataCacheEvents<CDataCache>,
	public IDispatchImpl<IDataCache, &IID_IDataCache, &LIBID_DataSourceCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDataCache()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DATACACHE)


BEGIN_COM_MAP(CDataCache)
	COM_INTERFACE_ENTRY(IDataCache)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDataCache)
	CONNECTION_POINT_ENTRY(__uuidof(_IDataCacheEvents))
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
    STDMETHOD(CheckNew)(LONG id, LONG version, VARIANT_BOOL* b);
    STDMETHOD(BeginCache)(LONG id, BSTR url);


};

OBJECT_ENTRY_AUTO(__uuidof(DataCache), CDataCache)
