#pragma once
#include "pch.h"
#include "Entity/Entity.h"

class TextComponent;
class SpriteComponent;

class ResultScreen : public Entity
{
public:

	ResultScreen();
	virtual ~ResultScreen();
	const bool& GetPause() const { return isPause; }
	void SetPause(const bool& pause) { isPause = pause; }
	void SetTotalBullets(const int& total) { totalFireBullets = total; }

protected:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

	void UpdatePosition(int width, int height);

private:
	std::vector<Entity*> m_pEntities = { nullptr };
	TextComponent* m_pRestartTextComp = nullptr;
	TextComponent* m_pResumeTextComp = nullptr;
	TextComponent* m_pTitleTextComp = nullptr;
	TextComponent* m_pStageSelectComp = nullptr;
	TextComponent* m_pClearTextComp = nullptr;
	TextComponent* m_pGameOverTextComp = nullptr;
	TextComponent* m_pPauseTextComp = nullptr;
	TextComponent* m_pTotalShotTextComp = nullptr;
	TextComponent* m_pFontMoveTextComp = nullptr;
	TextComponent* m_pFontFireTextComp = nullptr;

	SpriteComponent* m_pSelectSpriteComp = nullptr;
	SpriteComponent* m_pSelectedSpriteComp = nullptr;
	SpriteComponent* m_pSelectSpriteComp1 = nullptr;
	SpriteComponent* m_pSelectedSpriteComp1 = nullptr;
	SpriteComponent* m_pSelectSpriteComp2 = nullptr;
	SpriteComponent* m_pSelectedSpriteComp2 = nullptr;
	SpriteComponent* m_pBackGroundSpriteComp = nullptr;
	SpriteComponent* m_pMoveSpriteComp = nullptr;
	SpriteComponent* m_pLeftClickSpriteComp = nullptr;

	float totalTime = 60.0f;
	float showTotalTime = 60.0f;

	int selectCount = 0;
	int selectAudioCount = 0;
	int limitCount = 0;
	float selectDelayTime = 0.0f;
	bool isPause = false;
	int totalFireBullets = 0;
};