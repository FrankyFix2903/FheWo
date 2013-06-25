#pragma once
#include "Device.h"
#include "HandleData.h"

// CDetailsDIM-Dialogfeld

class CDetailsDIM : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailsDIM)

public:
	CDetailsDIM(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CDetailsDIM();

// Dialogfelddaten
	enum { IDD = IDD_DetailsDIM };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	CDevice* device;
	CHandleData* phandleData;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnon();
	afx_msg void OnBnClickedBtnoff();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void HandleSlider();
};
