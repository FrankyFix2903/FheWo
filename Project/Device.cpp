#include "stdafx.h"
#include "Device.h"


CDevice::CDevice(void)
{
}


CDevice::~CDevice(void)
{
}

CDevice::CDevice(CString hAdress, CString sAdress, CString deviceState, CString room, CString typ, CString name)
{
	this->hAdress = hAdress;
	this->sAdress = sAdress;
	this->deviceState = deviceState;
	this->room = room;
	this->typ = typ;
	this->name = name;
#ifdef _DEBUG
	if(hAdress == _T(""))
		this->hAdress = _T("12341234");
	if(sAdress == _T(""))
		this->sAdress = _T("1234");
	if(deviceState == _T(""))
		this->deviceState = _T("Aus");
	if(room == _T(""))
		this->room = _T("Test");
	if(typ == _T(""))
		this->typ = _T("DU");
	if(name == _T(""))
		this->name = _T("ERROR");
#endif

}

void CDevice::set_hAdress(CString hAdress)
{
	this->hAdress = hAdress;
}
 
void CDevice::set_sAdress(CString sAdress)
{
	this->sAdress = sAdress;
}
 
void CDevice::set_deviceState(CString deviceState)
{
	this->deviceState = deviceState;
}
 
void CDevice::set_room(CString room)
{
	this->room = room;
}
 
void CDevice::set_typ(CString typ)
{
	this->typ = typ;
}
 
void CDevice::set_name(CString name)
{
	this->name = name;
}
 

CString CDevice::get_hAdress()
{
	return hAdress;
}
 
CString CDevice::get_sAdress()
{
	return sAdress;	
}
 
CString CDevice::get_deviceState()
{
	return deviceState;
}
 
CString CDevice::get_room()
{
	return room;
}
 
CString CDevice::get_typ()
{
	return typ;
}
 
CString CDevice::get_name()
{
	return name;
}
 