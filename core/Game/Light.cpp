#include "Game/Light.h"

Light::Light(Game* game) : m_pGame(game)
{
	m_LightPosition = Vector4(0.0f, 50.0f, -100.0f, 0.0f);
	m_LightColor = Color(1.0f, 1.0f, 1.0f, 0.0f);
}

Light::~Light()
{
}
