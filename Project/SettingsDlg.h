#pragma once
#include "HandleData.h"

// CSettingsDlg-Dialogfeld

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CSettingsDlg();

// Dialogfelddaten
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CHandleData* _handleData;
	afx_msg void OnPaint();
};
