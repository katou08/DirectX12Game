#pragma once
#include "pch.h"
#include "Graphics/ConstData.h"

class Game;

class Light
{
public:
	Light(Game* game);
	~Light();
	void SetPosition(Vector3& pos) { m_LightPosition = Vector4(pos.x, pos.y, pos.z, 0.0f); }
	void SetColor(Color& color) { m_LightColor = color; }
	const Vector4& GetPosition() const { return m_LightPosition; }
	const Color& GetColor() const { return m_LightColor; }

private:
	Game* m_pGame = nullptr;
	Vector4 m_LightPosition;	//!< ライト位置
	Color	m_LightColor;		//!< ライトカラー
};