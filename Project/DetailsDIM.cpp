// DetailsDIM.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "DetailsDIM.h"
#include "afxdialogex.h"
#include "Device.h"


// CDetailsDIM-Dialogfeld

IMPLEMENT_DYNAMIC(CDetailsDIM, CDialogEx)

CDetailsDIM::CDetailsDIM(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetailsDIM::IDD, pParent)
{

}

CDetailsDIM::~CDetailsDIM()
{
}

void CDetailsDIM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDetailsDIM, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNON, &CDetailsDIM::OnBnClickedBtnon)
	ON_BN_CLICKED(IDC_BTNOFF, &CDetailsDIM::OnBnClickedBtnoff)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDetailsDIM-Meldungshandler

void CDetailsDIM::OnPaint()
{
	CPaintDC dc(this);
	CString typ = L"";
	SetWindowTextW(device->get_name());
	GetDlgItem(IDC_HADRESS)->SetWindowTextW(device->get_hAdress());
	GetDlgItem(IDC_SADRESS)->SetWindowTextW(device->get_sAdress());
	GetDlgItem(IDC_STATE)->SetWindowTextW(device->get_deviceState());
	typ = device->get_typ();
	GetDlgItem(IDC_TYP)->SetWindowTextW(typ);
	GetDlgItem(IDC_ROOM)->SetWindowTextW(device->get_room());
	if(typ == L"fs20rsu")
	{
		GetDlgItem(IDC_BTNON)->SetWindowTextW(L"Auf");
		GetDlgItem(IDC_BTNOFF)->SetWindowTextW(L"Ab");
	}
	if(typ != L"fs20du")
		return;
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->ShowWindow(true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(0, 100);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetTicFreq(25);
}


void CDetailsDIM::OnBnClickedBtnon()
{
	phandleData->sendCommandtoFhem(device->get_name(), L"on");
}


void CDetailsDIM::OnBnClickedBtnoff()
{
	phandleData->sendCommandtoFhem(device->get_name(), L"off");
}


void CDetailsDIM::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER1)
        HandleSlider();
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDetailsDIM::HandleSlider() // Geht noch nicht gnaz sauber
{
    // Akt. Position des Schiebereglers auslesen
	int nActPos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
	CString tmp;
	tmp.Format(L"%d", nActPos);
	tmp += "%";
	phandleData->sendCommandtoFhem(device->get_name(), L"dim" + tmp);
}
