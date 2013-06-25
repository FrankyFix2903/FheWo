// Tab2.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "Tab2.h"
#include "afxdialogex.h"
#include "Room.h"


// CTab2-Dialogfeld

IMPLEMENT_DYNAMIC(CTab2, CDialogEx)

CTab2::CTab2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTab2::IDD, pParent)
{

}

CTab2::~CTab2()
{
}

void CTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab2, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED,9000,9500,OnAnyButton)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CTab2-Meldungshandler
void CTab2::OnAnyButton( UINT nID ) 
{
	CString buttonText;
	GetDlgItem(nID)->GetWindowTextW(buttonText);
	CString tmp;
	tmp.Format(L"%d", nID);
	CRoom dlg;
	dlg.phandleData = phandleData;
	dlg.room = buttonText;
	dlg.DoModal();
}

void CTab2::OnPaint()
{
	CPaintDC dc(this);
	CRect rcWindow;
	this->GetWindowRect(rcWindow);
	pSB = (CScrollBar*) GetDlgItem(IDC_SCROLLBAR1);
	pSB->SetScrollRange(0, 100);
	pSB->MoveWindow(rcWindow.Width() - 16, 0, 16, rcWindow.Height());
}


void CTab2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos = pScrollBar->GetScrollPos();
    switch(nSBCode) 
	{
	case SB_LINEDOWN:
		if(pos + 1 < phandleData->get_deviceList()->GetCount() * 10)
		{
			ScrollWindow(0,-1,0);
			pos += 1;
			pScrollBar->SetScrollPos(pos);
		}
		
        break;
	case SB_LINEUP:
		if(pos - 1 >= 0)
		{
			ScrollWindow(0,1,0);
			pos -= 1;
			pScrollBar->SetScrollPos(pos);
		}
        break;
	case SB_THUMBPOSITION:
		//pScrollBar->SetScrollPos(nPos);
		break;
	default:
		return;
	}
	return;
}
