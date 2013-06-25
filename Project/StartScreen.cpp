// StartScreen.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "StartScreen.h"
#include "afxdialogex.h"
#define TIMER_EVENT WM_APP+1

// CStartScreen-Dialogfeld

IMPLEMENT_DYNAMIC(CStartScreen, CDialogEx)

CStartScreen::CStartScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartScreen::IDD, pParent)
{

}

CStartScreen::~CStartScreen()
{
}

void CStartScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartScreen, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStartScreen-Meldungshandler


void CStartScreen::OnPaint()
{
	pos = 0;
	CPaintDC dc(this);
	CImage image;
	image.Load(_T("FheWoStartScreen.png"));
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,800,400,SWP_NOZORDER);
	image.Draw(::GetDC(this->GetSafeHwnd()), 0, 0, 800, 400);
	CWnd::CenterWindow();
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetRange(0,90);
	SetTimer(TIMER_EVENT, 250, NULL);
}


void CStartScreen::OnTimer(UINT_PTR nIDEvent)
{
	srand(time(0));
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(pos);
		pos += rand()%9;
	if(pos > 120)
	{
		KillTimer(TIMER_EVENT);
		Sleep(1000);
		this->EndDialog(0);
	}
	CDialogEx::OnTimer(nIDEvent);
}
