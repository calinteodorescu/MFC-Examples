// Location.h : CLocation ������

#pragma once
#include "resource.h"       // ������


#include "SceneCOM_i.h"

#include <osgEarth/Viewpoint>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;
using namespace osgEarth;

// CLocation

class ATL_NO_VTABLE CLocation :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLocation, &CLSID_Location>,
	public IDispatchImpl<ILocation, &IID_ILocation, &LIBID_SceneCOMLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLocation()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LOCATION)


BEGIN_COM_MAP(CLocation)
	COM_INTERFACE_ENTRY(ILocation)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
    void Attach(Viewpoint& vp)
    {
        mLocation = vp;
    }
    Viewpoint& GetVP()
    {
        return mLocation;
    }
private:
    Viewpoint mLocation;
public:



};

OBJECT_ENTRY_AUTO(__uuidof(Location), CLocation)
