#ifndef __HHOK_API_H__
#define __HOOK_API_H__

typedef void (WINAPI *APIFUNC)(void);
//API��Ϣ�ṹ�� ��һ���б�
typedef struct _APIINFO
{
	//dll name
	char module_name[100];
	//dll��Ҫ���ҹ��ĺ�������
	char api_name[100];
	//�Լ��Ĺҹ�����
	char my_api_name[100];
	//��Ա����
	char my_friend_api_name[100];
	//������������
	int param_count;
	//������ʼ��ַ
    int start_pos;
	//APIFUNC
	APIFUNC old_api, my_api;
	//�Ƿ����ڹҹ�
	int bHooking;
	//�����ԭʼ�����Ŀ�ͷ20���ֽ���
	BYTE save_bytes[20];
	//�������
	CRITICAL_SECTION cs;
    //ԭ�������ı�������
	DWORD old_protection_flags;
	_APIINFO* pNext;
} APIINFO, *PAPIINFO;

//���ڹҹ�����
class CApiInfo
{
public:
	CApiInfo();
	virtual ~CApiInfo();
    //��ǰ�б��йҹ������ĸ���
	int m_count;
	//�����ҹ��б�
	PAPIINFO m_pList;
    //���һ���ҹ�
	PAPIINFO AddHook(char* modulename, char* apiname, char* myapiname, int paramcount, APIFUNC oldapi = NULL,
		             APIFUNC myapi = NULL, char* myfriendapiname = NULL, int startpos = 0);
	//ɾ�����йҹ�����
	void DeleteAllHooks();
    PAPIINFO FindByMyApi(APIFUNC myapi);
	PAPIINFO FindByOldApi(APIFUNC oldapi);
	PAPIINFO FindByApiName(char* apiname);
	PAPIINFO FindByMyApiName(char* myapiname);
	void Lock(PAPIINFO pInfo);
	void Unlock(PAPIINFO pInfo);
};

bool WINAPI InjectLib(DWORD process_id, char *lib_name);
bool WINAPI InjectLib(HANDLE hProcess, char *lib_name);
bool WINAPI EjectLib(DWORD process_id, char *lib_name);
bool WINAPI EjectLib(HANDLE hProcess, char *lib_name);

#define HOOKAPI_DLL_NAME	"HookAPINT.dll"
#define MY_DLL_NAME			"mydll.dll"

class CHookApi  
{
public:
	CHookApi();
	virtual ~CHookApi();
protected:
	HMODULE m_hMyDll;
	CApiInfo m_apiinfo;
	CString m_strDllPath;
private:
	bool LoadMyDll();
};

#endif
