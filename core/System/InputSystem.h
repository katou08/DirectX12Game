#pragma once
#include "pch.h"

class Game;
class InputSystem
{
public:
	InputSystem(Game* game);
	~InputSystem();
	bool Initialize();
	void Update();
	DirectX::Keyboard* GetKeybord() const { return m_pKeybord.get(); }
	DirectX::Mouse* GetMouse() const { return m_pMouse.get(); }
	const DirectX::Keyboard::KeyboardStateTracker& GetKeyLog() const { return m_KeyLog; }

private:
	Game* m_pGame = nullptr;
	std::unique_ptr<DirectX::Keyboard> m_pKeybord = nullptr;
	DirectX::Keyboard::KeyboardStateTracker m_KeyLog;
	std::unique_ptr<DirectX::Mouse> m_pMouse = nullptr;
	DirectX::Mouse::ButtonStateTracker m_MouseLog;
};