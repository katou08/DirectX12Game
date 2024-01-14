#include "Game/Scene.h"
#include "Entity/Entity.h"

Scene::Scene(Game* game, const std::wstring& sceneName)
	: m_pGame(game)
	, m_SceneName(sceneName)
{
}

Scene::~Scene()
{
}

void Scene::Update(float deltaTime)
{
}

void Scene::UpdateInternal(float deltaTime)
{
	// 削除待ちのエンティティを削除
	for (auto e : m_pEntitiesToDestroy)
	{
		// 型に対応するマップから削除
		m_pEntities[e->m_TypeId].erase(e);
	}
	m_pEntitiesToDestroy.clear(); //!< 削除待ちリストをクリア

	// 各エンティティの OnUpdate メソッドを呼び出し
	// auto&& ユニバーサル参照(完全転送) 
	// [typeId, entities] 構造化束縛 
	// typeId : エンティティのid
	// entities : std::map<Entity*, std::unique_ptr<Entity>>
	for (auto&& [typeId, entities] : m_pEntities)
	{
		// [ptr, entity]
		// ptr : Entity*
		// entity : std::unique_ptr<Entity>
		for (auto&& [ptr, entity] : entities)
		{
			ptr->OnUpdate(deltaTime);
		}
	}
}

void Scene::AddEntityInternal(Entity* entity, size_t id)
{
	// エンティティの所有権を std::unique_ptr でラップし、m_pEntities マップに挿入
	auto entityPtr = std::unique_ptr<Entity>(entity);
	m_pEntities[id].emplace(entity, std::move(entityPtr));

	entity->m_TypeId = id;
	entity->m_pScene = this;

	// エンティティの OnCreate メソッドを呼び出し（エンティティが初期化されたことを通知）
	entity->OnCreate();
}

void Scene::RemoveEntity(Entity* entity)
{
	m_pEntitiesToDestroy.emplace(entity);
}
