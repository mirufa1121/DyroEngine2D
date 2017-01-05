#ifndef _WINDOW_H
#define _WINDOW_H

#include "System.h"
#ifndef _IWINDOW_H
#include "Core/Helpers/Interfaces/IWindow.h"
#endif

class Window : public System, public IWindow
{
public:
	Window();
	virtual ~Window();

	virtual bool initialize();
	virtual void update();
	virtual bool shutdown();

	virtual LRESULT handleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	virtual BYTE getWindowBitsPerPixel() const;

	virtual const std::tstring getWindowTitle() const;
	virtual const std::tstring getWindowClassName() const;

	virtual const POINT getWindowPosition() const;

private:
};

#endif //_WINDOW_H