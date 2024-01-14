#pragma once
#include "pch.h"
#include "Graphics/ResourceView.h"
#include "Graphics/ResourceHeap.h"

class Game;
class Texture;
class RenderSystem;

class Sprite
{
public:
	Sprite(Game* game);
	~Sprite();
	bool Initialize(const std::wstring texPath);
	void Draw();

	const DirectX::XMUINT2 GetSpriteSize() const { return m_SpriteSize; }
	void SetSpriteSize(const DirectX::XMUINT2& texsize) { m_SpriteSize = texsize; }
	const DirectX::XMFLOAT2 GetSpritePosition() const { return m_SpritePosition; }
	void SetSpritePosition(const DirectX::XMFLOAT2& texpos) { m_SpritePosition = texpos; }
	const float GetSpriteScale() const { return m_Scale; }
	void SetSpriteScale(const float& scale) { m_Scale = scale; }
	void SetLayerDepth(const float& layerdepth) { m_LayerDepth = layerdepth; }

private:
	Game* m_pGame = nullptr;
	RenderSystem* m_pRenderSystem = nullptr;
	std::unique_ptr<DirectX::SpriteBatch> m_pSpriteBatch = nullptr;
	std::shared_ptr<Texture> m_pTexture = nullptr;
	ShaderResourceView m_SRV;
	uint32_t frameCount = 0;
	DirectX::XMUINT2 m_SpriteSize = { 1, 1 };
	DirectX::XMFLOAT2 m_SpritePosition = { 0.0f, 0.0f };
	Vector2 m_Origin = {};
	float m_Rotation = 0.0f;
	float m_Scale = 1.0f;
	float m_LayerDepth = 0.0f;

	ResourceHeap m_SpriteHeap;
};