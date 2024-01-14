#pragma once
#include "pch.h"

class Game;
class Entity;

class Scene
{
public:
	Scene(Game* game, const std::wstring& sceneName = L"");
	virtual ~Scene();
	template <typename T>
	T* AddEntity()
	{
		static_assert(std::is_base_of<Entity, T>::value, "TÇÕEntityÉNÉâÉXÇ©ÇÁîhê∂Ç≥ÇÍÇΩÇ‡ÇÃÇæÇØ.");
		auto id = typeid(T).hash_code();
		auto e = new T();
		AddEntityInternal(e, id);
		return e;
	}
	void RemoveEntity(Entity* entity);

	Game* GetGame() { return m_pGame; }
	const std::wstring& GetName() const { return m_SceneName; }
	const std::wstring& GetNextSceneName() const { return m_NextSceneName; }
	const bool& GetIsQuit() const { return IsQuit; }
	const bool& GetIsPause() const { return IsPause; }
	const bool& GetIsClear() const { return IsClear; }
	const bool& GetIsGameOver() const { return IsGameOver; }
	const bool& GetIsStart() const { return IsStart; }

	void SetNextSceneName(const std::wstring& nextname) { m_NextSceneName = nextname; }
	void SetIsQuit(const bool& isquit) { IsQuit = isquit; }
	void SetIsPause(const bool& ispause) { IsPause = ispause; }
	void SetIsClear(const bool& isclear) { IsClear = isclear; }
	void SetIsGameOver(const bool& isgameover) { IsGameOver = isgameover; }
	void SetIsStart(const bool& isstart) { IsStart = isstart; }

	virtual void Update(float deltaTime) = 0;
	virtual bool LoadData() = 0;

protected:
	void UpdateInternal(float deltaTime);
	Game* m_pGame = nullptr;
	std::wstring m_SceneName = L"";
	std::wstring m_NextSceneName = L"";
	bool IsQuit = false;
	bool IsPause = false;
	bool IsClear = false;
	bool IsGameOver = false;
	bool IsStart = false;

private:
	void AddEntityInternal(Entity* entity, size_t id);

	std::map<size_t, std::map<Entity*, std::unique_ptr<Entity>>> m_pEntities;
	std::set<Entity*> m_pEntitiesToDestroy;

	friend class Entity;
};