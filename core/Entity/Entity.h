#pragma once
#include "pch.h"

class Game;
class Component;
class TransformComponent;
class Scene;
class InputSystem;

class Entity
{
public:
	Entity();
	virtual ~Entity();
	void Release();

	Scene* GetScene() { return m_pScene; }
	TransformComponent* GetTransform() { return m_Transform; };
	InputSystem* GetInputSystem();

	template <typename T>
	T* AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Tは必ずComponentクラスから派生したものに限る");
		auto c = GetComponent<T>();
		if (!c)
		{
			auto id = typeid(T).hash_code(); //!< T型のハッシュコードを取得
			c = new T();
			AddComponentInternal(c, id);
			return c;
		}

		return nullptr;
	}

	template <typename T>
	T* GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Tは必ずComponentクラスから派生したものに限る");
		auto id = typeid(T).hash_code(); //!< T型のハッシュコードを取得
		return (T*)GetComponentInternal(id);
	}

protected:
	virtual void OnCreate() {}
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnCollision(Component* body1, Component* body2) {}

	Scene* m_pScene = nullptr;
	TransformComponent* m_Transform = nullptr;
	size_t m_TypeId = 0;
	std::map<size_t, std::unique_ptr<Component>> m_Components;

private:
	void AddComponentInternal(Component* component, size_t id);
	Component* GetComponentInternal(size_t id);
	void RemoveComponent(size_t id);

	friend class Component;
	friend class Scene;
};
