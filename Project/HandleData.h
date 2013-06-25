#pragma once
#include "Device.h"
typedef bool (* LPFNDLLCOLORSETTEXT)(int, wchar_t*, int, int, int);
typedef void (* LPFNDLLUPDATE)();
typedef bool (* LPFNDLLCOLORSETTITLE)(wchar_t*, int, int, int);
class CHandleData
{
private:
	CObList *_device;
	CString fhemURL; // http:// + fhemURL + /fhem...
	CString username;
	CString password;
	CWnd* tab1;
	CWnd* tab2;
	CProgressCtrl* ctrl;
	CStatic* status;
	bool isLCD;
	int buttonIDs[500][3];
	bool loadDeviceonStart;
	bool parseDeviceExtra(CString string, CDevice* device);
	bool parseDevice(CString string);
	bool readDeviceExtras(CDevice* device);
	bool deleteContentList();
public:
	CHandleData(void);
	~CHandleData(void);

	LPFNDLLCOLORSETTEXT LogiLcdColorSetText;
	LPFNDLLUPDATE LogiLcdUpdate;
	LPFNDLLCOLORSETTITLE LogiLcdColorSetTitle;
	void ClearLCD(bool tmp);

	CString get_fhemURL();
	void set_fhemURL(CString fhemURL);
	CString get_username();
	void set_username(CString fhemURL);
	CString get_password();
	void set_password(CString fhemURL);
	void set_progressBar(CProgressCtrl* ctrl);
	CProgressCtrl* get_progressBar();
	void set_status(CStatic* status);
	CStatic*  get_status();

	void set_Tab1(CWnd* tab);
	void set_Tab2(CWnd* tab);
	void set_loadDeviceonStart(bool loadDeviceonStart);
	bool get_loadDeviceonStart();
	CObList* get_deviceList();
	bool readDeviceFromFhem();
	int searchRowFromId(UINT nID);
	CString get_windowsVersion();
	bool drawTab1();
	bool drawTab2();
	bool sendCommandtoFhem(CString name, CString command);

};
