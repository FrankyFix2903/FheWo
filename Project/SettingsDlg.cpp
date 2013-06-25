// SettingsDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"
#include "HandleData.h"


// CSettingsDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingsDlg::IDD, pParent)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSettingsDlg-Meldungshandler


void CSettingsDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	CString tmp = _T("");

	GetDlgItem(IDC_EDITURL)->GetWindowTextW(tmp);
	(*_handleData).set_fhemURL(tmp);
	tmp = _T("");
	GetDlgItem(IDC_EDITUSERNAME)->GetWindowTextW(tmp);
	(*_handleData).set_username(tmp);
	tmp = _T("");
	GetDlgItem(IDC_EDITPASSWORD)->GetWindowTextW(tmp);
	(*_handleData).set_password(tmp);
	(*_handleData).set_loadDeviceonStart(((CButton*)GetDlgItem(IDC_CHECKLOADONSTART))->GetCheck());
}


void CSettingsDlg::OnPaint()
{
	CPaintDC dc(this);
	GetDlgItem(IDC_EDITURL)->SetWindowTextW((*_handleData).get_fhemURL());
	GetDlgItem(IDC_EDITUSERNAME)->SetWindowTextW((*_handleData).get_username());
	GetDlgItem(IDC_EDITPASSWORD)->SetWindowTextW((*_handleData).get_password());
	((CButton*)GetDlgItem(IDC_CHECKLOADONSTART))->SetCheck((*_handleData).get_loadDeviceonStart());
}
