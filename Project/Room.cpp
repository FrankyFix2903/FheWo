// Room.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "FheWo.h"
#include "Room.h"
#include "afxdialogex.h"
#include "Device.h"
#include "DetailsDIM.h"


// CRoom-Dialogfeld

IMPLEMENT_DYNAMIC(CRoom, CDialogEx)

CRoom::CRoom(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRoom::IDD, pParent)
{

}

CRoom::~CRoom()
{
}

void CRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoom, CDialogEx)
	ON_WM_PAINT()
	ON_CONTROL_RANGE(BN_CLICKED,3000,3200,OnAnyButton)
END_MESSAGE_MAP()


// CRoom-Meldungshandler


void CRoom::OnPaint()
{
	CPaintDC dc(this);
	SetWindowTextW(room);
	draw();
}

void CRoom::draw()
{
	POSITION pos;
	CButton* Button;
	CStatic* staticText;
	CString btnText[2];
	CDevice* tmpDevice;
	int top = 10;
	int bottom = 30;
	int i = 0;
	int countButtonID = 3001;
	int countStaticID = 2001;
	CObList* list = phandleData->get_deviceList();
	for (pos = list->GetHeadPosition(); pos != NULL;)
	{	
		tmpDevice = (CDevice*)list->GetNext(pos);
		if(tmpDevice->get_room() != room)
			continue;
		if(tmpDevice->get_typ() == _T("fs20rsu"))
		{
			btnText[0] = _T("Auf");
			btnText[1] = _T("Ab");
		}
		else
		{
			btnText[0] = _T("An");
			btnText[1] = _T("Aus");
		}
		staticText = new CStatic();
		DWORD dwStyleStatic  = WS_EX_STATICEDGE;
		staticText->Create(tmpDevice->get_name(),dwStyleStatic,CRect(10,top,260,bottom),this,countStaticID);
		staticText->ShowWindow(TRUE);

		Button = new CButton();
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		Button->Create(btnText[0], dwStyle, CRect(300,top,340,bottom), this, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		buttonIDs[i][i] = countButtonID;
		countButtonID++;

		Button = new CButton();
		Button->Create(btnText[1], dwStyle, CRect(360,top,400,bottom), this, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		buttonIDs[i][i+1] = countButtonID;
		countButtonID++;

		Button = new CButton();
		Button->Create(_T("Details"), dwStyle, CRect(420,top,480,bottom), this, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		buttonIDs[i][i+2] = countButtonID;

		top += 30;
		bottom += 30;
		staticText = NULL;
		countButtonID++;
		countStaticID++;
		i++;
	}
}

void CRoom::OnAnyButton( UINT nID )
{
	CString deviceName;
	CString buttonText;
	int row = searchRowFromId(nID);
	GetDlgItem(nID)->GetWindowTextW(buttonText);
	GetDlgItem(2000 + row)->GetWindowTextW(deviceName);
	CDevice* tmpDevice = NULL;
	if(buttonText == _T("Details"))
	{
		POSITION pos;
		for (pos = phandleData->get_deviceList()->GetHeadPosition(); pos != NULL;)
		{	
			tmpDevice = ((CDevice*)phandleData->get_deviceList()->GetNext(pos));
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

int CRoom::searchRowFromId(UINT nID)
{
	for(int i = 0 ; i < 50 ; i++)
	{
		if(buttonIDs[i][i] == nID)
			return i+1;
		else if(buttonIDs[i][i+1] == nID)
			return i+1;
		else if(buttonIDs[i][i+2] == nID)
			return i+1;
	}
	return -1;
}
