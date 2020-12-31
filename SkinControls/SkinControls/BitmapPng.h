
#ifndef BITMAP_PNG_H_
#define BITMAP_PNG_H_
#pragma once

#include "SkinControlsDefine.h"

// ��������PNG
class SKIN_CONTROLS_EXPORT CBitmapPng : public CBitmap
{
	struct CCallback;
public:
	CBitmapPng();

public:

	// ���ļ�����
	BOOL LoadFromFile(LPCTSTR lpszFileName);

	// ����Դ����
	BOOL LoadFromResource(HMODULE hModule, HRSRC hRes);

	// ���ڴ����
	BOOL LoadFromMemory(BYTE* pBuffer, DWORD dwSize);

	BOOL LoadFromFile(CFile* pFile);

	// �Ƿ���Alphaͨ��
	BOOL IsAlpha() const;

private:
	HBITMAP ConvertToBitmap(BYTE* pbImage, CSize szImage,  int cImgChannels) const;

protected:
	BOOL m_bAlpha;
};

AFX_INLINE BOOL CBitmapPng::IsAlpha() const {
	return m_bAlpha;
}

SKIN_CONTROLS_EXPORT int ZLibCompress(BYTE *dest, ULONG* destLen, const BYTE *source, ULONG sourceLen);
SKIN_CONTROLS_EXPORT ULONG ZLibCompressBound(ULONG sourceLen);
SKIN_CONTROLS_EXPORT int ZLibUncompress(BYTE *dest, ULONG* destLen, const BYTE* source, ULONG sourceLen);

#endif // BITMAP_PNG_H_
