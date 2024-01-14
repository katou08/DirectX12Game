#pragma once
#include "pch.h"
#include "Entity/Component.h"

class Sprite;

class SpriteComponent : public Component
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

	void SetSprite(const std::wstring& texPath);
	const std::shared_ptr<Sprite>& GetSprite();
	const bool& GetActive() const;
	void SetActive(const bool& isActive);

protected:
	virtual void onCreateInternal();

private:
	std::shared_ptr<Sprite> m_pSprite = nullptr;
	bool m_IsActive = true;
};