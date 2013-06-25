
// FheWo.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CFheWoApp:
// Siehe FheWo.cpp für die Implementierung dieser Klasse
//

class CFheWoApp : public CWinApp
{
public:
	CFheWoApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CFheWoApp theApp;