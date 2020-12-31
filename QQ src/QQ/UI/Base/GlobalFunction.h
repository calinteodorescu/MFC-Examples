#pragma once

// ����ͼƬ
bool LoadImage(const CString strPathFile, CBitmap &bitmap, CSize &size);	
// ����ͼƬ
bool ImageFromIDResource(UINT nID, CString strType, Image * & pImg);
// ȡ��ͼƬƽ����ɫ
bool GetAverageColor(CDC *pDC, CBitmap &bitmap, const CSize &sizeImage, COLORREF &clrImage);
// ȡ��ͼƬ��С
bool GetSize(CBitmap &bitmap, CSize &size);


// �滭����

// ����ֱ����
int DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rcDes, const CRect &rcSrc, int nBeginTransparent = 0, int nEndTransparent = 100);
// ��ˮƽ����
int DrawHorizontalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rcDes, const CRect &rcSrc, int nBeginTransparent = 0, int nEndTransparent = 100);
// �����½ǹ���
void DrawRightBottomTransition(CDC &dc, CDC &dcTemp, CRect rc, const int nOverRegio, const COLORREF clrBackground);
// ��ͼƬ�߿�
void DrawImageFrame(Graphics &graphics, Image *pIamge, const CRect &rcControl, int nX, int nY, int nW, int nH, int nFrameSide = 4);
// ������Բ�Ǿ���
void DrawRectangle(CDC &dcDes, const CRect &rcDes, BOOL bUp = TRUE, int nBeginTransparent = 60, int nEndTransparent = 90);