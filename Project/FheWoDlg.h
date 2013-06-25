
// FheWoDlg.h: Headerdatei
//

#pragma once
#include "HandleData.h"
#include "Tab1.h"
#include "Tab2.h"

// CFheWoDlg-Dialogfeld
typedef bool (* LPFNDLLCOLORSETTEXT)(int, wchar_t*, int, int, int);
class CFheWoDlg : public CDialogEx
{
// Konstruktion
public:
	CFheWoDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_FHEWO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
private:
	struct ThreadParam
	{
		HWND mDlg;
		CHandleData* handle;
		LPFNDLLCOLORSETTEXT LogiLcdColorSetText;
	};
	int countItem;
	bool refreshDataOnly;
	bool tabspainted;
	bool isLCD;

	CTab1* tab1;
	CTab2* tab2;
	CProgressCtrl* pbLoading;
	CStatic* status;
	LRESULT OnUpdateControl(WPARAM wParam, LPARAM lParam);
	CHandleData _handleData;

	ULONG_PTR g_gdiplusToken;
	int type;
	void ClearLCD(bool title);

	void DownloadAndProcessed();
	static UINT Thread(LPVOID pParam);
	LRESULT finishedProcess(WPARAM wp, LPARAM lp);
	void initAll();
	void initLCD();
	bool loadSettings();
	bool saveSettings();
	void showStartScreen();
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void addTabToTabCTRL(CTabCtrl* tabCtrl, CDialogEx* tab, UINT nID, LPWSTR title);
	afx_msg void OnBnClickedBtnsettings();
	afx_msg void OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult);



	afx_msg void OnBnClickedBtnloaddata();
	afx_msg void OnBnClickedOk();
};
