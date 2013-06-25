#pragma once
class CDevice : public CObject
{
private:
	CString hAdress;
	CString sAdress;
	CString deviceState;
	CString room;
	CString typ;
	CString name;

public:
	CDevice(void);
	~CDevice(void);

	CDevice(CString hAdress, CString sAdress, CString deviceState, CString room, CString typ, CString name);

	void set_hAdress(CString hAdress);
	void set_sAdress(CString sAdress);
	void set_deviceState(CString deviceState);
	void set_room(CString room);
	void set_typ(CString typ);
	void set_name(CString name);

	CString get_hAdress();
	CString get_sAdress();
	CString get_deviceState();
	CString get_room();
	CString get_typ();
	CString get_name();
};