#pragma once
#include "HandleData.h"


// CRoom-Dialogfeld

class CRoom : public CDialogEx
{
	DECLARE_DYNAMIC(CRoom)

public:
	CRoom(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CRoom();

// Dialogfelddaten
	enum { IDD = IDD_ROOM };
private:
	int buttonIDs[50][3];
	int searchRowFromId(UINT nID);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()

public:
	CHandleData* phandleData;
	CString room;
	afx_msg void OnPaint();
	void draw();
	void OnAnyButton( UINT nID );
};
