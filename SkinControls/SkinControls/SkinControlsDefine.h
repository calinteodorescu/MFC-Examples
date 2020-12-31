
#ifndef SKIN_CONTROLS_DEFINE_H_
#define SKIN_CONTROLS_DEFINE_H_
#pragma once

#ifndef SKIN_CONTROLS_EXPORT
#ifdef SKIN_CONTROLS_DLL
#define SKIN_CONTROLS_EXPORT _declspec(dllexport)
#else 
#define SKIN_CONTROLS_EXPORT _declspec(dllimport)
#endif
#endif

#ifdef _DEBUG
#define SKIN_CONTROLS_DLL_NAME TEXT("SkinControlsD.dll")
#else
#define SKIN_CONTROLS_DLL_NAME TEXT("SkinControls.dll")
#endif

#define SKIN_CHILD_WINDOW (WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)
#define SKIN_POPUP_WINDOW (WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS)

// ����ά��
#define CountArray(Array) (sizeof(Array) / sizeof(Array[0]))

// ɾ��ָ��
#define  SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData = NULL; }

// ɾ������
#define  SafeDeleteArray(pData) { try { delete []pData; } catch (...) { ASSERT(FALSE); } pData=NULL; }

// �ӿ��ͷ�
#define  SafeRelease(pObject) { if (pObject != NULL) { pObject->Release(); pObject=NULL; } }

// ͸��ɫ
#define MASK_COLOR	(RGB(255,0,255))

#endif // SKIN_CONTROLS_DEFINE_H_