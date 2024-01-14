#pragma once
#include "pch.h"
#include "Window.h"
#include "Rect.h"

class Game;
class SwapChain;

class Display : public Window
{
public:
	Display(Game* game);
	~Display();
	bool Initialize();
	bool CreateSwapChain();
	void* GetWindow() const { return m_hwnd; }

protected:
	virtual void onSize(const Rect& size);

private:
	Game* m_pGame = nullptr;
	std::shared_ptr<SwapChain> m_pSwapChain = nullptr;

private:
	friend class RenderSystem;
};