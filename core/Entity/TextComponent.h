#pragma once
#include "pch.h"
#include "Entity/Component.h"

class Text;

class TextComponent : public Component
{
public:
	TextComponent();
	virtual ~TextComponent();

	void SetText(const std::wstring& textPath);
	const std::shared_ptr<Text>& GetText();
	const bool& GetActive() const;
	void SetActive(const bool& isActive);

protected:
	virtual void onCreateInternal();

private:
	std::shared_ptr<Text> m_pText = nullptr;
	bool m_IsActive = true;
};