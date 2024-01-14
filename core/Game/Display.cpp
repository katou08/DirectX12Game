#include "pch.h"
#include "Game/Display.h"
#include "Graphics/SwapChain.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/GraphicsEngine.h"
#include "Game/Game.h"

Display::Display(Game* game) : m_pGame(game)
{
}

Display::~Display()
{
}

bool Display::Initialize()
{
	return true;
}

bool Display::CreateSwapChain()
{
	auto size = getClientSize();
	auto renderSystem = m_pGame->GetGraphicsEngine()->GetRenderSystem();

	// �X���b�v�`�F�[���̍쐬
	m_pSwapChain = std::make_shared<SwapChain>();
	if (!m_pSwapChain->Initialize(static_cast<HWND>(m_hwnd), size.width, size.height, renderSystem))
	{
		ThrowFailed("�X���b�v�`�F�[���̍쐬�Ɏ��s���܂���.");
		return false;
	}

	return true;
}

void Display::onSize(const Rect& size)
{
	m_pSwapChain->Resize(size.width, size.height);
}
