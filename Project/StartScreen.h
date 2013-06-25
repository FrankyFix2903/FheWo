#pragma once


// CStartScreen-Dialogfeld

class CStartScreen : public CDialogEx
{
	DECLARE_DYNAMIC(CStartScreen)

public:
	CStartScreen(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CStartScreen();

// Dialogfelddaten
	enum { IDD = IDD_STARTSCREEN };

private:
	int pos;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
