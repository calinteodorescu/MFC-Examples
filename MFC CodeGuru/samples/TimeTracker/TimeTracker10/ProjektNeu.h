#ifndef AFX_PROJEKTNEU_H__523A4A01_50A6_11D1_94CD_0020187099A1__INCLUDED_
#define AFX_PROJEKTNEU_H__523A4A01_50A6_11D1_94CD_0020187099A1__INCLUDED_

// ProjektNeu.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CProjektNeu 

class CProjektNeu : public CDialog
{
// Konstruktion
public:
	CProjektNeu(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CProjektNeu)
	enum { IDD = IDD_PROJEKT };
	CString	m_Datei;
	CString	m_Projekt;
	//}}AFX_DATA


// �berschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions�berschreibungen
	//{{AFX_VIRTUAL(CProjektNeu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CProjektNeu)
		// HINWEIS: Der Klassen-Assistent f�gt hier Member-Funktionen ein
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio f�gt zus�tzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_PROJEKTNEU_H__523A4A01_50A6_11D1_94CD_0020187099A1__INCLUDED_
