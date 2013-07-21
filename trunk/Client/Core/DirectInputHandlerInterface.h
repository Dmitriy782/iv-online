#pragma once

enum eDIDeviceType
{
	DIDEVICE_TYPE_UNKNOWN,
	DIDEVICE_TYPE_MOUSE,
	DIDEVICE_TYPE_KEYBOARD,
	DIDEVICE_TYPE_JOYSTICK
};


class DirectInputHandlerInterface
{
	public:
		virtual bool	ProcessInputDeviceState(eDIDeviceType dt, char * keyBuffer, int size, HRESULT oldHResult)=0;
};