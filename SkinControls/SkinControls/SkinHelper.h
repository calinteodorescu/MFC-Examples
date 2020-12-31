
#ifndef SKIN_HELPER_H_
#define SKIN_HELPER_H_
#pragma once

#include "SkinControlsDefine.h"

// ������
class SKIN_CONTROLS_EXPORT CSkinHelper
{
public:
	// Ĭ������
	static CFont& GetDefaultFont();
	// ��������
	static CFont& GetCaptionFont();

	// ���ι��
	static HCURSOR GetSysHandCursor();

private:
	static CFont m_DefaultFont; // Ĭ������
	static CFont m_CaptionFont; // ��������

private:
	CSkinHelper() {}
	~CSkinHelper() {}
};

#endif // SKIN_HELPER_H_