
// FheWoDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "FheWoDlg.h"
#include "afxdialogex.h"
#include "HandleData.h"
#include "SettingsDlg.h"
#include "WaitDlg.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <afxtaskdialog.h>
#include "StartScreen.h"

using namespace std;

#define USE_LOGITECH_DLL_ONLY

#ifndef USE_LOGITECH_DLL_ONLY
#pragma comment(lib, "LogitechLcd.lib")
#include "LogitechLcd.h"

#else
#define LOGI_LCD_TYPE_COLOR   (0x00000002)

#define LOGI_LCD_COLOR_BUTTON_LEFT   (0x00000100)
#define LOGI_LCD_COLOR_BUTTON_RIGHT  (0x00000200)
#define LOGI_LCD_COLOR_BUTTON_OK     (0x00000400)
#define LOGI_LCD_COLOR_BUTTON_CANCEL (0x00000800)
#define LOGI_LCD_COLOR_BUTTON_UP     (0x00001000)
#define LOGI_LCD_COLOR_BUTTON_DOWN   (0x00002000)
#define LOGI_LCD_COLOR_BUTTON_MENU   (0x00004000)

const int LOGI_LCD_COLOR_WIDTH = 320;
const int LOGI_LCD_COLOR_HEIGHT = 240;

typedef bool (* LPFNDLLINIT)(wchar_t*, int );
typedef bool (* LPFNDLLISCONNECTED)(int);
typedef bool (* LPFNDLLISBUTTONPRESSED)(int);
typedef void (* LPFNDLLUPDATE)();
typedef void (* LPFNDLLSHUTDOWN)();
typedef bool (* LPFNDLLMONOSETBACKGROUND)(byte*);
typedef bool (* LPFNDLLMONOSETTEXT)(int, wchar_t*);
typedef bool (* LPFNDLLCCOLORSETBACKGROUND)(byte*);
typedef bool (* LPFNDLLCOLORSETTITLE)(wchar_t*, int, int, int);
typedef bool (* LPFNDLLCOLORSETTEXT)(int, wchar_t*, int, int, int);


LPFNDLLINIT LogiLcdInit = NULL;
LPFNDLLISCONNECTED LogiLcdIsConnected = NULL;
LPFNDLLISBUTTONPRESSED LogiLcdIsButtonPressed = NULL;
LPFNDLLUPDATE LogiLcdUpdate = NULL;
LPFNDLLSHUTDOWN LogiLcdShutdown = NULL;
LPFNDLLMONOSETBACKGROUND LogiLcdMonoSetBackground = NULL;
LPFNDLLMONOSETTEXT LogiLcdMonoSetText = NULL;
LPFNDLLCCOLORSETBACKGROUND LogiLcdColorSetBackground = NULL;
LPFNDLLCOLORSETTITLE LogiLcdColorSetTitle = NULL;
LPFNDLLCOLORSETTEXT LogiLcdColorSetText = NULL;

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define THREAD_FINISHED (WM_APP + 1)

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFheWoDlg-Dialogfeld




CFheWoDlg::CFheWoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFheWoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFheWoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFheWoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNSETTINGS, &CFheWoDlg::OnBnClickedBtnsettings)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, &CFheWoDlg::OnTcnSelchangeTabcontrol)
	ON_MESSAGE(THREAD_FINISHED, finishedProcess)
	ON_BN_CLICKED(IDC_BTNLOADDATA, &CFheWoDlg::OnBnClickedBtnloaddata)
	ON_BN_CLICKED(IDOK, &CFheWoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFheWoDlg-Meldungshandler

BOOL CFheWoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	//CWinThread* pThread = AfxBeginThread (thrFunction1, this);
	initLCD();
	initAll();
	
	
	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CFheWoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CFheWoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CFheWoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);

}

void CFheWoDlg::initAll()
{
	countItem = 0;
	tabspainted = false;

	pbLoading = (CProgressCtrl*)GetDlgItem(IDC_PBPROCESS);
	status = (CStatic*)GetDlgItem(IDC_STATUS);

	pbLoading->ShowWindow(false);
	pbLoading->SetRange(0,100);
	status->ShowWindow(false);
	status->SetWindowTextW(_T("FEHLER"));

	CTabCtrl* tabCtrl = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
	tab1 = new CTab1();
	tab2 = new CTab2();
	addTabToTabCTRL(tabCtrl, tab1, IDD_TAB1, L"Gesamt");
	addTabToTabCTRL(tabCtrl, tab2, IDD_TAB2, L"Räume");
	tab1->ShowWindow(SW_SHOW);
	tab2->ShowWindow(SW_HIDE);
	tabCtrl->SetCurSel(0);
	tab1->phandleData = &_handleData;
	tab2->phandleData = &_handleData;
	_handleData.set_Tab1(tab1);
	_handleData.set_Tab2(tab2);
	_handleData.set_progressBar(pbLoading);
	_handleData.set_status(status);
	loadSettings();
	showStartScreen();
	if(_handleData.get_loadDeviceonStart())
	{
		refreshDataOnly = false;
		DownloadAndProcessed();
		//this->SetBackgroundColor(RGB(155,0,0));
	}
}

void CFheWoDlg::showStartScreen()
{
	CStartScreen startScreen;
	startScreen.DoModal();
}


bool CFheWoDlg::loadSettings()
{	
	fstream stream;
	stream.open("config.dat", ios_base::in);
	if(!stream.is_open())
		return false;
	char buffer[4096];
	while(!stream.eof())
		stream >> buffer;
	stream.close();
	CString erg(buffer);
	int nTokenPos = 0;
	CString url = L"";
	url = erg.Tokenize(_T(";"), nTokenPos);
	url = url.Left(url.GetLength() -1 );
	CString username = L"";
	username = erg.Tokenize(_T(";"), nTokenPos);
	username = username.Left(username.GetLength() -1 );
	CString password = L"";
	password = erg.Tokenize(_T(";"), nTokenPos);
	password = password.Left(password.GetLength() -1 );
	CString state = L"false";
	state = erg.Tokenize(_T(";"), nTokenPos);
	_handleData.set_fhemURL(url);
	_handleData.set_username(username);
	_handleData.set_password(password);
	if(state == L"true")
		_handleData.set_loadDeviceonStart(true);
	else
		_handleData.set_loadDeviceonStart(false);
	return true;
}

bool CFheWoDlg::saveSettings()
{
	fstream stream;
	stream.open("config.dat", ios_base::out);
	CString url = _handleData.get_fhemURL() + L"\\";
	CString user = _handleData.get_username() + L"\\";
	CString pw = _handleData.get_password() + L"\\";
	CString loadDevice = L"";
	if(_handleData.get_loadDeviceonStart())
		loadDevice = "true";
	else
		loadDevice = "false";
	stream.write(CT2A(url + ";"), url.GetLength() + 1);
	stream.write(CT2A(user + ";"), user.GetLength() + 1);
	stream.write(CT2A(pw + ";"), pw.GetLength() + 1 );
	stream.write(CT2A(loadDevice + ";"), loadDevice.GetLength() + 1);
	stream.close();
	return true;
}

void CFheWoDlg::initLCD()
{
	isLCD = false;
	// Start Gdiplus 
    Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
    Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL); 

	HINSTANCE logiDllHandle = LoadLibrary(L"LogitechLcd.dll");
    if (logiDllHandle != NULL)
    {
        LogiLcdInit = (LPFNDLLINIT)GetProcAddress(logiDllHandle, "LogiLcdInit");
        LogiLcdIsConnected = (LPFNDLLISCONNECTED)GetProcAddress(logiDllHandle, "LogiLcdIsConnected");
        LogiLcdIsButtonPressed = (LPFNDLLISBUTTONPRESSED)GetProcAddress(logiDllHandle, "LogiLcdIsButtonPressed");
        LogiLcdUpdate = (LPFNDLLUPDATE)GetProcAddress(logiDllHandle, "LogiLcdUpdate");
        LogiLcdShutdown = (LPFNDLLSHUTDOWN)GetProcAddress(logiDllHandle, "LogiLcdShutdown");
        LogiLcdMonoSetBackground = (LPFNDLLMONOSETBACKGROUND)GetProcAddress(logiDllHandle, "LogiLcdMonoSetBackground");
        LogiLcdMonoSetText = (LPFNDLLMONOSETTEXT)GetProcAddress(logiDllHandle, "LogiLcdMonoSetText");
        LogiLcdColorSetBackground = (LPFNDLLCCOLORSETBACKGROUND)GetProcAddress(logiDllHandle, "LogiLcdColorSetBackground");
        LogiLcdColorSetTitle = (LPFNDLLCOLORSETTITLE)GetProcAddress(logiDllHandle, "LogiLcdColorSetTitle");
        LogiLcdColorSetText = (LPFNDLLCOLORSETTEXT)GetProcAddress(logiDllHandle, "LogiLcdColorSetText");
		_handleData.LogiLcdColorSetText = LogiLcdColorSetText;
		_handleData.LogiLcdUpdate = LogiLcdUpdate;
		_handleData.LogiLcdColorSetTitle = LogiLcdColorSetTitle;
    }
	else
	{
		return;
	}
	type = 0;
	type = type | LOGI_LCD_TYPE_COLOR;
	isLCD = LogiLcdInit(L"FheWo", type);
	isLCD = LogiLcdIsConnected(type);
	if(isLCD)
	{
		LogiLcdColorSetTitle(L"FehWo", 255, 255, 255);
		LogiLcdUpdate();
	}
}

void CFheWoDlg::OnBnClickedBtnsettings()
{
	CSettingsDlg settingsDlg;
	settingsDlg._handleData = &_handleData;
	if(isLCD)
	{
		LogiLcdColorSetTitle(L"Einstellungen", 255, 255, 255);
		LogiLcdColorSetText(0, L"Server:", 255,255,255);
		LogiLcdColorSetText(1, T2W(_handleData.get_fhemURL().GetBuffer()), 255,255,255);
		LogiLcdColorSetText(2, L"Benutzername:", 255,255,255);
		LogiLcdColorSetText(3, T2W(_handleData.get_username().GetBuffer()), 255,255,255);
		LogiLcdColorSetText(4, L"Passwort:", 255,255,255);
		CString pw("");
		for(int i = 0 ; i < _handleData.get_password().GetLength() ; i++)
			pw += "*";
		LogiLcdColorSetText(5, T2W(pw.GetBuffer()), 255,255,255);
		CString tmp = L"Lade Daten beim Start: ";
		tmp += _handleData.get_loadDeviceonStart() ? L"Ja" : L"Nein";
		LogiLcdColorSetText(6, T2W(tmp.GetBuffer()), 255,255,255);
		LogiLcdUpdate();
	}
	if(settingsDlg.DoModal() == IDOK)
	{
		saveSettings();
		if(isLCD)
		{
			
			ClearLCD(false);
			LogiLcdColorSetText(0, L"Gespeichert", 255,255,255);
			LogiLcdUpdate();
			Sleep(1000);
			LogiLcdColorSetTitle(L"FheWo", 255,255,255);
			ClearLCD(false);
			LogiLcdUpdate();
		}
		return;
	}
	if(isLCD)
	{
		ClearLCD(false);
		LogiLcdColorSetText(0, L"Abgebrochen", 255,255,255);
		LogiLcdUpdate();
		Sleep(1000);
		LogiLcdColorSetTitle(L"FheWo", 255,255,255);
		ClearLCD(false);
		LogiLcdUpdate();
	}
}

void CFheWoDlg::addTabToTabCTRL(CTabCtrl* pTabCtrl, CDialogEx* tab, UINT nID, LPWSTR title)
{
	tab->Create(nID, pTabCtrl);
	TCITEM item;
	item.mask    = TCIF_TEXT | TCIF_PARAM;
	item.lParam  = (LPARAM)& tab;
	item.pszText = title;
	pTabCtrl->InsertItem(countItem, &item);
	CRect rcItem;
	CRect rcWindow;
	pTabCtrl->GetItemRect(0, &rcItem);
	pTabCtrl->GetWindowRect(rcWindow);
	tab->MoveWindow(rcItem.left, rcItem.bottom + 1, rcWindow.Width() - 5, rcWindow.Height() - 23);
	tab->ShowWindow(SW_SHOW);
	countItem++;
}

void CFheWoDlg::OnTcnSelchangeTabcontrol(NMHDR *pNMHDR, LRESULT *pResult)
{
	tab1->ShowWindow(SW_HIDE);  
	tab2->ShowWindow(SW_HIDE); 
	CTabCtrl* tabCtrl = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
	for(int i=0;i<2;i++)
	{
		tabCtrl->HighlightItem(i,FALSE); 
	}
	switch(tabCtrl->GetCurSel())
	{
		case 0: tab1->ShowWindow(SW_SHOW);
			break;
		case 1: tab2->ShowWindow(SW_SHOW);
			break;
	}   
	tabCtrl->HighlightItem(tabCtrl->GetCurSel(),TRUE); 

	*pResult = 0;

}

void CFheWoDlg::DownloadAndProcessed()
{
	if(isLCD)
	{
		LogiLcdColorSetTitle(L"Vorgang läuft", 255, 255, 255);
		ClearLCD(false);
		LogiLcdUpdate();
	}
	ThreadParam* param = new ThreadParam;
    param->mDlg = m_hWnd;
	param->handle = &_handleData;
	param->LogiLcdColorSetText = LogiLcdColorSetText;
    AfxBeginThread(Thread, param);
    param = 0;
	delete param;
}

UINT CFheWoDlg::Thread(LPVOID pParam)
{
	ThreadParam* p = static_cast<ThreadParam*> (pParam);
	if(p->handle->readDeviceFromFhem())
		::SendMessage(p->mDlg, THREAD_FINISHED, 0, 0);
	else
	{
		if(CTaskDialog::IsSupported())
		{
			CString Title(_T("Download fehlgeschlagen"));
			CString Heading(_T("Download fehlgeschlagen"));
			CString Content(_T("Verbindungsaufbau fehlgeschlagen"));
			CString Collapsed(_T("Weitere Information zeigen"));
			CString Expanded(_T("Der Verbindungsaufbau zu ") + p->handle->get_fhemURL() + _T(" ist fehlgeschlagen\nBitte prüfen Sie die Einstellungen\nbzw. Ihre Netzwerkkonnektivität"));
			CString Information(_T("Weitere Informationen verbergen"));
			CTaskDialog dia(Content, Heading, Title);
			dia.SetMainIcon(TD_ERROR_ICON);
			dia.AddCommandControl(200,_T("Vorgang wiederholen"));
			dia.AddCommandControl(201,_T("Abbrechen"));
			dia.SetExpansionArea(Expanded,Collapsed,Information);
			dia.DoModal();
			switch(dia.GetSelectedCommandControlID())
			{
			case 200:
				if(p->handle->readDeviceFromFhem())
					::SendMessage(p->mDlg, THREAD_FINISHED, 0, 0);
				else
				{
					AfxMessageBox(_T("Der Verbindungsaufbau schlug fehl\nVorgang vom System abgebrochen"), MB_ICONERROR);
				}
				break;
			case 201:
				AfxMessageBox(_T("Vorgang vom Benutzer abgebrochen"));
				break;
			}
		}
	}
	Sleep(5000);
	::SendMessage(p->mDlg, THREAD_FINISHED, 5, 0);
	p->handle->get_progressBar()->ShowWindow(false);
	p->handle->get_status()->ShowWindow(false);
	delete p;
	return 0;
}

LRESULT CFheWoDlg::finishedProcess(WPARAM wp, LPARAM lp)
{
	if((!refreshDataOnly || !tabspainted) && wp != 5)
	{
		_handleData.drawTab1();
		_handleData.drawTab2();
		status->SetWindowTextW(_T("Geräte geladen"));
		tabspainted = true;
	}
	else if(wp == 5)
	{
		if(isLCD)
		{
			ClearLCD(false);
			LogiLcdColorSetTitle(L"FheWo", 255, 255, 255);
			LogiLcdUpdate();
		}
	}
	else
	{
		status->SetWindowTextW(_T("Geräte aktualisiert"));
	}
	pbLoading->SetPos(100);
	
	return 0;
}


void CFheWoDlg::OnBnClickedBtnloaddata()
{
	refreshDataOnly = true;
	DownloadAndProcessed();
}


void CFheWoDlg::OnBnClickedOk()
{
	if(isLCD)
		LogiLcdShutdown();
	CDialogEx::OnOK();
}

void CFheWoDlg::ClearLCD(bool tmp)
{
	if(isLCD)
	{
		if(tmp)
			LogiLcdColorSetTitle(L"", 255, 255, 255);
		for(int i = 0 ; i < 8 ; i++)
			LogiLcdColorSetText(i, L"", 255,255,255);
		LogiLcdUpdate();
	}
}
