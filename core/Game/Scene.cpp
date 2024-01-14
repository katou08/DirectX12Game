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
	// �폜�҂��̃G���e�B�e�B���폜
	for (auto e : m_pEntitiesToDestroy)
	{
		// �^�ɑΉ�����}�b�v����폜
		m_pEntities[e->m_TypeId].erase(e);
	}
	m_pEntitiesToDestroy.clear(); //!< �폜�҂����X�g���N���A

	// �e�G���e�B�e�B�� OnUpdate ���\�b�h���Ăяo��
	// auto&& ���j�o�[�T���Q��(���S�]��) 
	// [typeId, entities] �\�������� 
	// typeId : �G���e�B�e�B��id
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
	// �G���e�B�e�B�̏��L���� std::unique_ptr �Ń��b�v���Am_pEntities �}�b�v�ɑ}��
	auto entityPtr = std::unique_ptr<Entity>(entity);
	m_pEntities[id].emplace(entity, std::move(entityPtr));

	entity->m_TypeId = id;
	entity->m_pScene = this;

	// �G���e�B�e�B�� OnCreate ���\�b�h���Ăяo���i�G���e�B�e�B�����������ꂽ���Ƃ�ʒm�j
	entity->OnCreate();
}

void Scene::RemoveEntity(Entity* entity)
{
	m_pEntitiesToDestroy.emplace(entity);
}
