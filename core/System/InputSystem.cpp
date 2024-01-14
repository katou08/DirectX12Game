#include "System/InputSystem.h"
#include "Game/Game.h"
#include "Game/Display.h"

InputSystem::InputSystem(Game* game)
	:m_pGame(game)
{
}

InputSystem::~InputSystem()
{
}

bool InputSystem::Initialize()
{
	m_pKeybord = std::make_unique<DirectX::Keyboard>();
	m_pMouse = std::make_unique<DirectX::Mouse>();

	auto window = static_cast<HWND>(m_pGame->GetDisplay()->GetWindow());
	if(window != nullptr)
		m_pMouse->SetWindow(window);

	return true;
}

void InputSystem::Update()
{
	auto kb = m_pKeybord->GetState();
	m_KeyLog.Update(kb);
}
