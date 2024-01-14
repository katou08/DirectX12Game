#pragma once
#include "pch.h"
#include "Entity/Component.h"

class Mesh;

class MeshComponent : public Component
{
public:

	MeshComponent();
	virtual ~MeshComponent();

	void SetMesh(const std::wstring& meshPath, const std::wstring& vsPath = L"", const std::wstring& psPath = L"");
	const std::shared_ptr<Mesh>& GetMesh();
	const bool& GetActive() const;
	void SetActive(const bool& isActive);
protected:
	virtual void onCreateInternal();

private:
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	bool m_IsActive = true;
};