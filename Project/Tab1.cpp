// Tab1.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "Tab1.h"
#include "HandleData.h"
#include "afxdialogex.h"
#include "DetailsDIM.h"
#include "Device.h"


// CTab1-Dialogfeld

IMPLEMENT_DYNAMIC(CTab1, CDialogEx)

CTab1::CTab1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTab1::IDD, pParent)
{

}

CTab1::~CTab1()
{
}

void CTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab1, CDialogEx)
	ON_WM_PAINT()
	ON_CONTROL_RANGE(BN_CLICKED,9000,9500,OnAnyButton)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CTab1-Meldungshandler


void CTab1::OnPaint()
{
	CPaintDC dc(this);
	CRect rcWindow;
	this->GetWindowRect(rcWindow);
	pSB = (CScrollBar*) GetDlgItem(IDC_SCROLLBAR1);
	pSB->SetScrollRange(0, 100);
	pSB->MoveWindow(rcWindow.Width() - 16, 0, 16, rcWindow.Height());
	//pSB->EnableWindow(false);
}

// EventHändler für gezeichnete Buttons
void CTab1::OnAnyButton( UINT nID ) 
{
	CString deviceName;
	CString buttonText;
	int row = phandleData->searchRowFromId(nID);
	GetDlgItem(nID)->GetWindowTextW(buttonText);
	GetDlgItem(8000 + row)->GetWindowTextW(deviceName);
	CDevice* tmpDevice = NULL;
	if(buttonText == _T("Details"))
	{
		POSITION pos;
		CObList* list = phandleData->get_deviceList();
		for (pos = list->GetHeadPosition(); pos != NULL;)
		{	
			tmpDevice = ((CDevice*)list->GetNext(pos));
			if(tmpDevice->get_name() == deviceName)
			{
				CDetailsDIM dlg;
				dlg.device = tmpDevice;
				dlg.phandleData = phandleData;
				dlg.DoModal();
			}
		}
		
	}
	else
	{
		if(buttonText == _T("An") || buttonText == _T("Auf"))
			phandleData->sendCommandtoFhem(deviceName, _T("on"));
		else if(buttonText == _T("Aus") || buttonText == _T("Ab"))
			phandleData->sendCommandtoFhem(deviceName, _T("off"));
	}
	//AfxMessageBox(buttonText + _T(" für Gerät ") + deviceName);
}

void CTab1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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