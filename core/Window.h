#pragma once
#include "pch.h"
#include "Rect.h"

class Window
{
public:
	Window();
	virtual ~Window();

	Rect getClientSize();
	Rect getScreenSize();

	// ƒCƒxƒ“ƒg
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize(const Rect& size);

protected:
	void* m_hwnd;
	Rect m_size = { 0, 0, 1920, 1080 };
};