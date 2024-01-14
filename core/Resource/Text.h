#pragma once
#include "pch.h"
#include "Graphics/ResourceView.h"
#include "Graphics/ResourceHeap.h"

class Game;
class RenderSystem;
class Text
{
public:
	Text(Game* game);
	~Text();
	bool Initialize(const std::wstring& textPath);
	void Draw();

	const DirectX::XMFLOAT2 GetTextPosition() const { return m_TextPosition; }
	void SetTextPosition(const DirectX::XMFLOAT2& texpos) { m_TextPosition = texpos; }
	const float GetTextScale() const { return m_Scale; }
	void SetTextScale(const float& scale) { m_Scale = scale; }
	const std::wstring GetTextString() const { return textString; }
	void SetTextString(const std::wstring& text) { textString = text; }

private:
	Game* m_pGame = nullptr;
	RenderSystem* m_pRenderSystem = nullptr;
	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch = nullptr;
	std::unique_ptr<DirectX::SpriteFont> m_pSpriteFont = nullptr;
	uint32_t frameCount = 0;

	DirectX::XMFLOAT2 m_TextPosition = { 0.0f, 0.0f };
	Vector2 m_Origin = {};
	float m_Rotation = 0.0f;
	float m_Scale = 1.0f;
	std::wstring textString = L"";

	ResourceHeap m_FontHeap;
};