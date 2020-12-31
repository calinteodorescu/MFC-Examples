========================================================================
       MICROSOFT FOUNDATION CLASS BIBLIOTHEK : MyDemo
========================================================================


Diese MyDemo-Anwendung hat der Klassen-Assistent f�r Sie erstellt. Diese Anwendung
zeigt nicht nur die prinzipielle Verwendung der Microsoft Foundation Classes, 
sondern dient auch als Ausgangspunkt f�r die Erstellung Ihrer eigenen DLLs.

Diese Datei enth�lt die Zusammenfassung der Bestandteile aller Dateien, die 
Ihre MyDemo-Anwendung bilden.

MyDemo.h
    	Hierbei handelt es sich um die Haupt-Header-Datei der Anwendung. Diese enth�lt 
	andere projektspezifische Header (einschlie�lich Resource.h) und deklariert die
	Anwendungsklasse CMyDemoApp.

MyDemo.cpp
    	Hierbei handelt es sich um die Haupt-Quellcodedatei der Anwendung. Diese enth�lt die
    	Anwendungsklasse CMyDemoApp.

MyDemo.rc
	Hierbei handelt es sich um eine Auflistung aller Ressourcen von Microsoft Windows, die 
	vom Programm verwendet werden. Sie enth�lt die Symbole, Bitmaps und Cursors, die im 
	Unterverzeichnis RES abgelegt sind. Diese Datei l�sst sich direkt im Microsoft
	Developer Studio bearbeiten.

res\MyDemo.ico
    	Dies ist eine Symboldatei, die als Symbol f�r die Anwendung verwendet wird. Dieses 
	Symbol wird durch die Haupt-Ressourcendatei MyDemo.rc eingebunden.

res\MyDemo.rc2
    	Diese Datei enth�lt Ressourcen, die nicht vom Microsoft Developer Studio bearbeitet wurden.
	In diese Datei werden alle Ressourcen abgelegt, die vom Ressourcen-Editor nicht bearbeitet 
	werden k�nnen.

MyDemo.clw
    	Diese Datei enth�lt Informationen, mit denen der Klassen-Assistent bestehende
    	Klassen bearbeitet oder neue Klassen einf�gt. Au�erdem verwendet der Klassen-Assistent 
	diese Datei, um Informationen zu speichern, die er f�r das Erstellen und Bearbeiten von
	Nachrichtentabellen und Dialogfeld-Nachrichtentabellen sowie f�r das Erstellen von 
	Prototypen vom Member-Funktionen ben�tigt.


/////////////////////////////////////////////////////////////////////////////

Der Klassen-Assistent erstellt eine Dialogklasse:

MyDemoDlg.h, MyDemoDlg.cpp - das Dialogfeld
    	Diese Dateien enthalten die Klasse CMyDemoDlg. Diese Klasse legt das
    	Verhalten des Haupt-Dialogfelds der Anwendung fest. Die Vorlage des Dialog-
	felds befindet sich in MyDemo.rc, die mit Microsoft Developer Studio
	bearbeitet werden kann.


/////////////////////////////////////////////////////////////////////////////
Andere Standarddateien:

StdAfx.h, StdAfx.cpp
    	Mit diesen Dateien werden vorkompilierte Header-Dateien (PCH) mit der Bezeichnung 
	MyDemo.pch und eine vorkompilierte Typdatei mit der Bezeichnung StdAfx.obj
	erstellt.

Resource.h
    	Dies ist die Standard-Header-Datei, die neue Ressourcen-IDs definiert.
    	Microsoft Developer Studio liest und aktualisiert diese Datei.

/////////////////////////////////////////////////////////////////////////////
Weitere Hinweise:

Der Klassen-Assistent f�gt "ZU ERLEDIGEN:" im Quellcode ein, um die Stellen anzuzeigen, 
an denen Sie Erweiterungen oder Anpassungen vornehmen k�nnen.

Wenn Ihre Anwendung die MFC in einer gemeinsam genutzten DLL verwendet und Ihre Anwendung
eine andere als die aktuell auf dem Betriebssystem eingestellte Sprache verwendet, muss 
die entsprechend lokalisierte Ressource MFC40XXX.DLL von der Microsoft Visual C++ CD-ROM 
in das Verzeichnis system oder system32 kopiert und in MFCLOC.DLL umbenannt werden ("XXX" 
steht f�r die Abk�rzung der Sprache. So enth�lt beispielsweise MFC40DEU.DLL die ins Deutsche 
�bersetzten Ressourcen). Anderenfalls erscheinen einige Oberfl�chenelemente Ihrer Anwendung 
in der Sprache des Betriebssystems.

/////////////////////////////////////////////////////////////////////////////
