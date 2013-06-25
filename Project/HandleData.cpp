#include "StdAfx.h"
#include "HandleData.h"
#include "Device.h"
#include <WinInet.h>
#include <algorithm>        // std::sort
#include <afxtaskdialog.h>



CHandleData::CHandleData(void)
{
	_device = new CObList();
	loadDeviceonStart = false;
}

CHandleData::~CHandleData(void)
{

}

CString CHandleData::get_fhemURL()
{
	return fhemURL;

}

void CHandleData::set_fhemURL(CString fhemURL)
{
	this->fhemURL = fhemURL;
}

CString CHandleData::get_username()
{
	return username;
}

void CHandleData::set_username(CString username)
{
	this->username = username;
}

CString CHandleData::get_password()
{
	return password;
}

void CHandleData::set_password(CString password)
{
	this->password = password;
}

void CHandleData::set_Tab1(CWnd* tab)
{
	this->tab1 = tab;
}

void CHandleData::set_Tab2(CWnd* tab)
{
	this->tab2 = tab;
}

CObList* CHandleData::get_deviceList()
{
	return _device;
}

void CHandleData::set_loadDeviceonStart(bool loadDeviceonStart)
{
	this->loadDeviceonStart = loadDeviceonStart;
}

bool CHandleData::get_loadDeviceonStart()
{
	return loadDeviceonStart;
}

void CHandleData::set_progressBar(CProgressCtrl* ctrl)
{
	this->ctrl = ctrl;
}

void CHandleData::set_status(CStatic* status)
{
	this->status = status;
}

CProgressCtrl* CHandleData::get_progressBar()
{
	return ctrl;
}

CStatic* CHandleData::get_status()
{
	return status;
}


bool CHandleData::readDeviceFromFhem()
{
	status->SetWindowTextW(_T("Verbindungsaufbau..."));
	if(isLCD)
	{
		LogiLcdColorSetText(0,L"Verbindungsaufbau...",255,255,255);
		LogiLcdUpdate();
	}
	ctrl->SetPos(0);
	status->ShowWindow(true);
	ctrl->ShowWindow(true);
	CString inetResult = _T("");
    char  buff[512];
	DWORD charsRead;
	CString httpCall;
    HINTERNET hInet = InternetOpen(L"FhemRequest", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if(username == _T("") && password == _T(""))
		httpCall = _T("http://") + fhemURL + _T("/fhem?cmd=list+TYPE=FS20");
	else
		httpCall = _T("http://") + username + _T(":") + password + _T("@") + fhemURL + _T("/fhem?cmd=list+TYPE=FS20");
    HINTERNET hFile = InternetOpenUrl(hInet, httpCall, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION| INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0 );
    if( hFile != INVALID_HANDLE_VALUE ) 
	{
		status->SetWindowTextW(_T("Downloade Geräte..."));
		if(isLCD)
		{
			LogiLcdColorSetText(0,L"Verbindungsaufbau... OK",255,255,255);
			LogiLcdColorSetText(1,L"Downloade Geräte...",255,255,255);
			LogiLcdUpdate();
		}
        while(InternetReadFile(hFile, buff, 511, &charsRead) && charsRead > 0) 
		{
            buff[charsRead] = '\0';
            inetResult += buff;
        }
        InternetCloseHandle(hFile);

    }
    InternetCloseHandle(hInet);
	
	if(inetResult != _T(""))
	{
		ctrl->SetPos(10);
		status->SetWindowTextW(_T("Download abgeschlossen"));
		if(isLCD)
		{
			LogiLcdColorSetText(1,L"Downloade Geräte...  OK",255,255,255);
			LogiLcdUpdate();
		}
		if(parseDevice(inetResult))
			return true;
	}
	else
	{
		status->SetWindowTextW(_T("Download Fehler"));
		if(isLCD)
		{
			LogiLcdColorSetText(1,L"Downloade Geräte...  Fehler",255,255,255);
			LogiLcdUpdate();
		}
		ctrl->SetPos(0);
	}
	return false;
}

bool CHandleData::readDeviceExtras(CDevice *device)
{
	CString inetResult = _T("");
    char  buff[512];
	DWORD charsRead;
	CString httpCall;
    HINTERNET hInet = InternetOpen(L"FhemRequest", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if(username == _T("") && password == _T(""))
		httpCall = _T("http://") + fhemURL + _T("/fhem?cmd=list " + device->get_name());
	else
		httpCall = _T("http://") + username + _T(":") + password + _T("@") + fhemURL + _T("/fhem?cmd=list " + device->get_name());
    HINTERNET hFile = InternetOpenUrl(hInet, httpCall, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION| INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0 );
    if( hFile != INVALID_HANDLE_VALUE ) 
	{
        while(InternetReadFile(hFile, buff, 511, &charsRead) && charsRead > 0) 
		{
            buff[charsRead] = '\0';
            inetResult += buff;
        }
        InternetCloseHandle(hFile);
    }
	InternetCloseHandle(hInet);
	if(inetResult != "")
	{
		if(parseDeviceExtra(inetResult, device));
			return true;
	}
	return false;
}

CString CHandleData::get_windowsVersion()
{
	OSVERSIONINFO OSversion;
	OSversion.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	::GetVersionEx(&OSversion);
	if(OSversion.dwMajorVersion == 6 && OSversion.dwMinorVersion == 1)
		return _T("Windows 7");
	else if(OSversion.dwMajorVersion == 6 && OSversion.dwMinorVersion == 2)
		return _T("Windows 8");
	else
		return _T("Version unbekannt");
}

bool CHandleData::parseDeviceExtra(CString string, CDevice *device)
{
	if(device != NULL)
	{
		string.Delete(0, string.Find(_T("<pre>")) + 5);
		string.Delete(string.Find(_T("</pre>")), string.GetLength());
		int nTokenPos = 0;
		int i = 0;
		CString strToken = string.Tokenize(_T("\n"), nTokenPos);
		CString specs[200];
		while (!strToken.IsEmpty())
		{
			if(i > 200)
			{
				AfxMessageBox(_T("Es ist ein interner Fehler aufgetreten"),MB_ICONWARNING|MB_OK);
				break;
			}
			strToken = string.Tokenize(_T("\n"), nTokenPos);
			specs[i] = strToken;
			i++;
		}
		for(int i = 0 ; specs[i] != "" && i <= 49 ; i++)
		{
			if(specs[i].Find(_T("DEF")) != -1)
			{
				specs[i].Replace(L"DEF", L"");
				specs[i].Replace(L" ", L"");
				specs[i].Replace(L"\n", L"");
				device->set_hAdress(specs[i].Mid(0, 8));
				device->set_sAdress(specs[i].Mid(8, 12));
			}
			else if(specs[i].Find(_T("STATE")) != -1)
			{
				specs[i].Replace(L"STATE", L"");
				specs[i].Replace(L" ", L"");
				specs[i].Replace(L"\n", L"");
				device->set_deviceState(specs[i]);
			}
			else if(specs[i].Find(_T("model")) != -1)
			{
				specs[i].Replace(L"model", L"");
				specs[i].Replace(L" ", L"");
				specs[i].Replace(L"\n", L"");
				device->set_typ(specs[i]);
			}
			else if(specs[i].Find(_T("room")) != -1)
			{
				specs[i].Replace(L"room", L"");
				specs[i].Replace(L" ", L"");
				specs[i].Replace(L"\n", L"");
				device->set_room(specs[i]);
			}	
		}
		return true;
	}
	return false;
}

bool CHandleData::parseDevice(CString string)
{
	CString tmp;
	ctrl->SetPos(10);
	if(isLCD)
	{
		LogiLcdColorSetText(2,L"Parse Geräte... 10%",255,255,255);
		LogiLcdUpdate();
	}
	status->SetWindowTextW(_T("Parse Geräte"));
	string.Delete(0, string.Find(_T("<pre>")) + 5);
	string.Delete(string.Find(_T("</pre>")), string.GetLength());
	int nTokenPos = 0;
	CDevice* newDevice;
	CString strToken = string.Tokenize(_T("\n"), nTokenPos);
	if(!_device->IsEmpty())
	{
		deleteContentList();
	}
	while (!strToken.IsEmpty())
	{
		newDevice = new CDevice(_T(""), _T(""), _T(""), _T(""), _T(""), strToken);
		readDeviceExtras(newDevice);
		_device->AddTail(newDevice);
		if(ctrl->GetPos() + 1 <= 90)
			ctrl->SetPos(ctrl->GetPos()+1);
		if(isLCD)
		{
			CString strIrgendwas; 
			strIrgendwas.Format(L"%d", ctrl->GetPos());
			tmp = L"Parse Geräte...     ";
			tmp += strIrgendwas + L"%";
			LogiLcdColorSetText(2,T2W(tmp.GetBuffer()),255,255,255);
			LogiLcdUpdate();
		}
		strToken = string.Tokenize(_T("\n"), nTokenPos);
	}
	if(isLCD)
	{
		LogiLcdColorSetText(2,L"Parse Geräte...      OK",255,255,255);
		LogiLcdColorSetText(3,L"Geräte geladen...    OK",255,255,255);
		LogiLcdUpdate();
	}
	ctrl->SetMarquee(false,0);
	return true;
}

bool CHandleData::drawTab1()
{
	POSITION pos;
	CButton* Button;
	CStatic* staticText;
	CString btnText[2];
	CDevice* tmpDevice;
	int top = 10;
	int bottom = 30;
	int i = 0;
	int countButtonID = 9001;
	int countStaticID = 8001;
	for (pos = _device->GetHeadPosition(); pos != NULL; countButtonID++, countStaticID++, i++)
	{	
		tmpDevice = (CDevice*)_device->GetNext(pos);
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
		staticText->Create(tmpDevice->get_name(),dwStyleStatic,CRect(10,top,260,bottom),tab1,countStaticID);
		staticText->ShowWindow(TRUE);

		Button = new CButton();
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		Button->Create(btnText[0], dwStyle, CRect(300,top,340,bottom), tab1, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		buttonIDs[i][i] = countButtonID;
		countButtonID++;

		Button = new CButton();
		Button->Create(btnText[1], dwStyle, CRect(360,top,400,bottom), tab1, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		buttonIDs[i][i+1] = countButtonID;
		countButtonID++;

		Button = new CButton();
		Button->Create(_T("Details"), dwStyle, CRect(420,top,480,bottom), tab1, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		buttonIDs[i][i+2] = countButtonID;

		top += 30;
		bottom += 30;
		staticText = NULL;
	}
	return false;
}

bool CHandleData::drawTab2()
{
	POSITION pos;
	CString tmp;
	CStatic* staticText;
	int top = 10;
	int bottom = 30;
	CButton* Button;
	CRect rcWindow;
	CString drawRooms[50];
	int i = 0;
	int c = 0;
	tab2->GetWindowRect(&rcWindow);
	for (pos = _device->GetHeadPosition(); pos != NULL; i++)
	{	
		tmp = ((CDevice*)_device->GetNext(pos))->get_room();
		for(int j = 0 ; j < 50 ; j++)
		{
			if(drawRooms[j] == tmp)
			{
				break;
			}
			else if(drawRooms[j] == "")
			{
				c++;
				drawRooms[j] = tmp;
				break;
			}
		}
	}
	std::sort(drawRooms, drawRooms + c);
	int countButtonID = 9001;
	for(int i = 0 ; i <= c ; i++, countButtonID++)
	{
		Button = new CButton();
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		Button->Create(drawRooms[i], dwStyle, CRect(10,top,rcWindow.Width() - 30,bottom), tab2, countButtonID);
		Button->ShowWindow(TRUE);
		Button = NULL;
		top += 30;
		bottom += 30;
	}
	return true;
}

int CHandleData::searchRowFromId(UINT nID)
{
	for(int i = 0 ; i < 500 ; i++)
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

bool CHandleData::sendCommandtoFhem(CString name, CString command)
{
	CString httpCall;
    HINTERNET hInet = InternetOpen(L"FhemCommand", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if(username == _T("") && password == _T(""))
		httpCall = _T("http://") + fhemURL + _T("/fhem?cmd=set " + name + _T(" ") + command);
	else
		httpCall = _T("http://") + username + _T(":") + password + _T("@") + fhemURL + _T("/fhem?cmd=set " + name + _T(" ") + command);
    HINTERNET hFile = InternetOpenUrl(hInet, httpCall, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION| INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0 );
	InternetCloseHandle(hFile);
	return true;
}

bool CHandleData::deleteContentList()
{
	CDevice* tmp;
	POSITION pos;
	for (pos = _device->GetHeadPosition(); pos != NULL;)
	{	
		tmp = ((CDevice*)_device->GetNext(pos));
		delete tmp;
	}
	delete _device;
	_device = new CObList();
	return true;
}

void CHandleData::ClearLCD(bool tmp)
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