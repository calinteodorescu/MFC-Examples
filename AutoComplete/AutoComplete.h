// AutoComplete.h : Haupt-Header-Datei f�r die Anwendung AUTOCOMPLETE
//

#if !defined(AFX_AUTOCOMPLETE_H__CB5E1423_4E09_4E4C_803C_3910C27D9A90__INCLUDED_)
#define AFX_AUTOCOMPLETE_H__CB5E1423_4E09_4E4C_803C_3910C27D9A90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CAutoCompleteApp:
// Siehe AutoComplete.cpp f�r die Implementierung dieser Klasse
//

class CAutoCompleteApp : public CWinApp
{
public:
	CAutoCompleteApp();

// �berladungen
	// Vom Klassenassistenten generierte �berladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CAutoCompleteApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CAutoCompleteApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingef�gt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // !defined(AFX_AUTOCOMPLETE_H__CB5E1423_4E09_4E4C_803C_3910C27D9A90__INCLUDED_)
