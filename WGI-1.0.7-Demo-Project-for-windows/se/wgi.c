/**
 *
 * �� �� ����wgi.c
 *
 * ��    ����
 *
 * �� �� �ߣ�
 *
 * �������ڣ�
 *
 * ��    ����1.0.8
 *
 * ��    ע��
 *
 *
 * * ά����ʷ *
 *
 *   <����>                <�޸���>
 *   <�޸�����...>
 *
 **/

#include <olectl.h>
#include "se.h"


/**
*
* �����ţ�	
* �����ܣ�	����ͼƬ�ļ���
* ��    ����	HBITMAP  LoadPictureFile(const TCHAR* adrame)
* ��    ����	����ͼƬ�ļ���SE������
* ��    ����	adrame - [IN] address and name��ͼƬ��ַ�����ơ�
* ��    �أ�	��NULL - ���سɹ���NULL - ����ʧ�ܡ�
* �� �� �ߣ�	��ƽ��
* �� �� �ߣ�	��ƽ��
* ʵ �� �ߣ�	��ƽ��
* ʹ �� �ߣ�	
* ��    ����	V1.0
* �������ڣ�	20110713
* ��    ע��	
* �޸ļ�¼��	
* ����Ҫ�㣺		
*/
HBITMAP  LoadPictureFile(const TCHAR* adrame)
{ 
	HANDLE hFile; 
	HBITMAP hBmp; 
	DWORD dwSize; 
	DWORD dwRead; 
	HGLOBAL hMemJpeg; 
	LPSTREAM lpStream; 
	OLE_HANDLE hJpegBmp; 
	HRESULT hr; 
	LPPICTURE lpPicture = NULL; 
	void *pMemJpeg; 
	
	/* Open the file and get the size. */ 
	if((hFile = CreateFile(adrame, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) 
		return NULL; 
	if((dwSize = GetFileSize(hFile, NULL)) == 0xFFFFFFFF) 
	{ 
		CloseHandle(hFile); 
		return NULL; 
	} 
	
	/* Allocate space for file, read it in, and then close the file again. */ 
	if((hMemJpeg = GlobalAlloc(GMEM_MOVEABLE, dwSize)) == NULL) 
	{ 
		CloseHandle(hFile); 
		return NULL; 
	} 
	if((pMemJpeg = GlobalLock(hMemJpeg)) == NULL) 
	{ 
		CloseHandle(hFile); 
		GlobalFree(hMemJpeg); 
		return NULL; 
	} 
	if(!ReadFile(hFile, pMemJpeg, dwSize, &dwRead, NULL)) 
	{ 
		CloseHandle(hFile); 
		GlobalFree(hMemJpeg); 
		return NULL; 
	} 
	CloseHandle(hFile); 
	GlobalUnlock(hMemJpeg); 
	
	/* Create the stream and load the picture. */ 
	if((hr = CreateStreamOnHGlobal(hMemJpeg, TRUE, &lpStream)) != S_OK) 
	{ 
		GlobalFree(hMemJpeg); 
		return NULL; 
	} 
	if(OleLoadPicture(lpStream, dwSize, FALSE, &IID_IPicture, &lpPicture) != S_OK) 
	{ 
		GlobalFree(hMemJpeg); 
		lpStream->lpVtbl->Release(lpStream); 
		return NULL; 
	} 
	
	/* Get the handle to the image, and then copy it. */ 
	if((lpPicture->lpVtbl->get_Handle(lpPicture, &hJpegBmp)) != S_OK) 
	{ 
		GlobalFree(hMemJpeg); 
		lpStream->lpVtbl->Release(lpStream); 
		lpPicture->lpVtbl->Release(lpPicture); 
		return NULL; 
	} 
	if((hBmp = CopyImage((HANDLE *) hJpegBmp, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG)) == NULL) 
	{ 
		GlobalFree(hMemJpeg); 
		lpStream->lpVtbl->Release(lpStream); 
		lpPicture->lpVtbl->Release(lpPicture); 
		return NULL; 
	} 
	
	/* Free the original image and memory. */ 
	GlobalFree(hMemJpeg); 
	lpStream->lpVtbl->Release(lpStream); 
	lpPicture->lpVtbl->Release(lpPicture); 

	return hBmp; 

} /* End of LoadPictureFile(). */  


/**
*
* �����ţ�	
* �����ܣ�	�ж�һ���Ƿ���һ��������ڡ�
* ��    ����	BOOL  IsPointInPolygon(int x, int y, const POLYGON*  pplg)
* ��    ����	�ж�һ���Ƿ���һ��������ڣ�SE������
* ��    ����	x - [IN] ���X�����ꣻ 
*               y - [IN] ���Y�����ꣻ
*               pplg - [IN] ָ�����Ρ�
* ��    �أ�	TRUE - ���ڶ�����ڣ�FALSE - �㲻�ڶ�����ڡ�
* �� �� �ߣ�	��ƽ��
* �� �� �ߣ�	��ƽ��
* ʵ �� �ߣ�	��ƽ��
* ʹ �� �ߣ�	
* ��    ����	V1.0.8
* �������ڣ�	20110713
* ��    ע��	
* �޸ļ�¼��	
* ����Ҫ�㣺	
*/
BOOL  IsPointInPolygon(int x, int y, const POLYGON*  pplg)
{
	return TRUE;
}

/**
*
* �����ţ�	
* �����ܣ�	�ж�������ͬһƽ��ľ����Ƿ��ཻ��
* ��    ����	BOOL  IsInter2Rects(const RECT* pSrcRect1, const RECT* pSrcRect2, RECT*  pDstRect)
* ��    ����	�ж�������ͬһƽ��ľ����Ƿ��ཻ�������ں�����SE������
* ��    ����	pSrcRect1 - [IN] ��һ��ƽ�����Ρ�
*               pSrcRect2 - [IN] �ڶ���ƽ�����Ρ�
*               pDstRect - [OUT] �����������ΪNULL�������ΪNULL,������������ཻ�Ĳ��֣�Ҳ�Ǿ��Σ�,����������β��ཻ,�Ͱ�����ṹ���ÿա�
* ��    �أ�	TRUE - ��ƽ������ཻ��FALSE - ��ƽ����β��ཻ��
* �� �� �ߣ�	��ƽ��
* �� �� �ߣ�	��ƽ��
* ʵ �� �ߣ�	��ƽ��
* ʹ �� �ߣ�	
* ��    ����	V1.0.8
* �������ڣ�	20110713
* ��    ע��	
* �޸ļ�¼��	
* ����Ҫ�㣺	
*/
BOOL  IsInter2Rects(const RECT* pSrcRect1, const RECT* pSrcRect2, RECT*  pDstRect)
{
	if(NULL == pDstRect)
	{
		RECT  rc;
		return IntersectRect(&rc, pSrcRect1, pSrcRect2);
	}else
	{
		return IntersectRect(pDstRect, pSrcRect1, pSrcRect2);
	}
}
