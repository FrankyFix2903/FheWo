#pragma once
#include "HandleData.h"

// CTab1-Dialogfeld

class CTab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1)

private:
	 CScrollBar* pSB;
public:
	CTab1(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CTab1();


// Dialogfelddaten
	enum { IDD = IDD_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	afx_msg void OnAnyButton(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CHandleData* phandleData;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
