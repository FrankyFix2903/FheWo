#pragma once


// CWaitDlg-Dialogfeld

class CWaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaitDlg)

public:
	CWaitDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CWaitDlg();

// Dialogfelddaten
	enum { IDD = IDD_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterst�tzung

	DECLARE_MESSAGE_MAP()
};
