// WaitDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "WaitDlg.h"
#include "afxdialogex.h"


// CWaitDlg-Dialogfeld

IMPLEMENT_DYNAMIC(CWaitDlg, CDialogEx)

CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaitDlg::IDD, pParent)
{

}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialogEx)
END_MESSAGE_MAP()


// CWaitDlg-Meldungshandler
