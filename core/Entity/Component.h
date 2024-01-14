#pragma once
#include "pch.h"

class Entity;
class Component
{
public:
	Component();
	virtual ~Component();

	void Release();
	Entity* GetEntity() { return m_pEnity; }

protected:
	virtual void onCreateInternal();

	Entity* m_pEnity = nullptr;  //!< 所有エンティティ
	size_t m_TypeId = 0;

	friend class Entity;
};