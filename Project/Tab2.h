#pragma once
#include "HandleData.h"

// CTab2-Dialogfeld

class CTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTab2)
private:
	CScrollBar* pSB;
public:
	CTab2(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CTab2();

// Dialogfelddaten
	enum { IDD = IDD_TAB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	afx_msg void OnAnyButton(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CHandleData* phandleData;
};
