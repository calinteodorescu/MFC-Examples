// Demo.h : Haupt-Header-Datei f�r die Anwendung DEMO
//

#if !defined(AFX_DEMO_H__D9D4DCA3_C324_11D1_91F5_B4375BB20A38__INCLUDED_)
#define AFX_DEMO_H__D9D4DCA3_C324_11D1_91F5_B4375BB20A38__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CApp:
// Siehe Demo.cpp f�r die Implementierung dieser Klasse
//

class CApp : public CWinApp
{
public:
	CApp();

// �berladungen
	// Vom Klassenassistenten generierte �berladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CApp)
	afx_msg void OnAppAbout();
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingef�gt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VER�NDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f�gt zus�tzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_DEMO_H__D9D4DCA3_C324_11D1_91F5_B4375BB20A38__INCLUDED_)
