
#ifndef SKIN_IMAGE_H_
#define SKIN_IMAGE_H_
#pragma once

#include "SkinControlsDefine.h"

// λͼ��ʽ
enum BitmapFormat
{
	BF_UNKNOWN = 0, // δ֪��ʽ
	BF_BMP,
	BF_PNG,
	BF_OTHER // ���� ����JPG
};

// ͼ������
class SKIN_CONTROLS_EXPORT CSkinImage : public CCmdTarget
{
public:
	CSkinImage();
	virtual ~CSkinImage();

public:
	// ����λͼ
	BOOL Create(int nWidth, int nHeight, HDC hDc = NULL);
	// ���ļ�����λͼ
	BOOL LoadFromFile(LPCTSTR pszFileName, BitmapFormat bf = BF_UNKNOWN);
	// ����Դ����λͼ
	BOOL LoadFromResource(HMODULE hModule, LPCTSTR lpszResource, BitmapFormat bf = BF_UNKNOWN);
	// ���ڴ����λͼ
	BOOL LoadFromMemory(BYTE* pBuffer, DWORD dwSize, BitmapFormat bf = BF_UNKNOWN);
	// ����λͼ
	void DestroyImage();

	// clrTransparent ���λͼ��ALPHAͨ������Ч
	// �滭λͼ
	void DrawImage(CDC* pDC, int nXPos, int nYPos, COLORREF clrTransparent);
	// �滭λͼ
	void DrawImage(CDC* pDC, int nXDest, int nYDest, int nDestWidth, int nDestHeight, COLORREF clrTransparent);
	// �滭λͼ
	void DrawImage(CDC* pDC, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXScr, int nYSrc, int nSrcWidth, int nSrcHeight, COLORREF clrTransparent);
	// �滭λͼ 
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, COLORREF clrTransparent);

	// �滭λͼ
	void DrawImage(CDC* pDC, int nXPos, int nYPos);
	// �滭λͼ
	void DrawImage(CDC* pDC, int nXDest, int nYDest, int nDestWidth, int nDestHeight);
	// �滭λͼ
	void DrawImage(CDC* pDC, int nXDest, int nYDest, int nDestWidth, int nDestHeight, int nXScr, int nYSrc, int nSrcWidth, int nSrcHeight);
	// �滭λͼ
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc);

	// �Ƿ���Ч
	BOOL IsValid() const;
	// �Ƿ���Alphaͨ��
	BOOL IsAlphaImage() const;
	// ��ȡ���
	int GetHeight() const;
	// ��ȡ�߶�
	int GetWidth() const;
	// ��ȡ���
	CSize GetExtent() const;
	// ��ȡ���
	HBITMAP GetBitmap() const;
	// ��ȡλ������
	BOOL GetBitmapPixel(CPoint pt, COLORREF& clrPixel);
	// ��������
	bool CreateBitmapRegion(CRgn& BitmapRgn, COLORREF crTransColor);
	// ��������
	bool CreateBitmapRegion(CRgn& BitmapRgn, int nXSrcPos, int nYSrcPos, int nSrcWidth, int nSrcHeight, COLORREF crTransColor);
	operator HBITMAP() const { return m_hBitmap; }

public:
	// �Ƿ�PNG
	static BOOL IsPngBitmapFile(LPCTSTR pszFileName);
	static BOOL IsPngBitmapResource(HMODULE hModule, LPCTSTR lpBitmapName);
	static BOOL IsPngBitmapMemory(BYTE* pBuffer, DWORD dwSize);
	// �Ƿ����Alpha
	static BOOL IsAlphaBitmapFile(LPCTSTR pszFileName);
	static BOOL IsAlphaBitmapResource(HMODULE hModule, LPCTSTR lpBitmapName);
	static BOOL IsAlphaBitmapMemory(BYTE* pBuffer, DWORD dwSize);
	// ��ȡAlphaλͼ������Alpha��Ϣ
	static BOOL GetBitmapBits(CDC& dcSrc, HBITMAP hBitmap, PBITMAPINFO& pBitmapInfo, LPVOID& pBits, UINT& nSize);
	// ������Ч��Alphaλͼ
	static HBITMAP PreMultiplyAlphaBitmap(HBITMAP hBitmap, BOOL* pbAlpha, LPBYTE* lpBits);
	// ����Alphaλͼ
	static HBITMAP LoadAlphaBitmap(UINT nIDResource);
	static HBITMAP LoadAlphaBitmap(HMODULE hModule, LPCTSTR lpszResource);
	// ����Դ����λͼ
	static HBITMAP LoadBitmapFromResource(HMODULE hModule, LPCTSTR lpszResource, BOOL* lbAlphaBitmap, BitmapFormat bf = BF_UNKNOWN);
	// ���ļ�����λͼ
	static HBITMAP LoadBitmapFromFile(LPCTSTR lpszFileName, BOOL* lbAlphaBitmap, BitmapFormat bf = BF_UNKNOWN);
	// ���ڴ����λͼ
	static HBITMAP ConvertToBitmap(BYTE* pBuffer, DWORD dwSize);
	static HBITMAP LoadBitmapFromMemory(BYTE* pBuffer, DWORD dwSize, BOOL* lbAlphaBitmap, BitmapFormat bf = BF_UNKNOWN);

	// ͸��ת��
	static BOOL TransparentBlt(HDC hdcDest, const CRect& rcDest,HDC hdcSrc, const CRect& rcSrc, UINT crTransparent);

	// ��ʾ����ָ��͸���ȵ�ͼ��
	static BOOL AlphaBlend(HDC hdcDest, const CRect& rcDest,HDC hdcSrc, const CRect& rcSrc);
	static BOOL AlphaBlend2(HDC hdcDest, const CRect& rcDest,HDC hdcSrc, const CRect& rcSrc, BYTE Transparency);

	static HBITMAP Create32BPPDIBSection(HDC hDC, int iWidth, int iHeight, LPBYTE* lpBits = NULL);

	// û��MSIMG32.DLL��ϵͳʹ��
	static BOOL McTransparentBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest,
		int nWidthDest, int nHeightDest, HDC hdcSrc,
		int nXOriginSrc, int nYOriginSrc, int nWidthSrc,
		int nHeightSrc, UINT crTransparent);

private:
	LPBYTE PreMultiply();
	static BOOL BitmapsCompatible(LPBITMAP lpbm1, LPBITMAP lpbm2);
	static BOOL BlendImages(HBITMAP hbmSrc1, BOOL bRTL1, HBITMAP hbmSrc2, BOOL bRTL2, HBITMAP hbmDst);
	static BOOL DoAlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest,
		int nWidthDest, int nHeightDest, HDC hdcSrc, int nXOriginSrc,
		int nYOriginSrc, int nWidthSrc, int nHeightSrc);
	
private:
	LPBYTE m_pBits;
	HBITMAP m_hBitmap;
	BOOL m_bAlpha;
	CSize m_szExtent;
};

#endif // SKIN_IMAGE_H_