#pragma once
#include "pch.h"
#include "Resource/ResMesh.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/ResourceView.h"
#include "Graphics/ResourceHeap.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/ConstData.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderContext.h"

class Game;

class Mesh
{
public:
	Mesh(Game* game);
	virtual ~Mesh();

	bool Init(const std::wstring srcPath, const std::wstring vsPath = L"", const std::wstring psPath = L"");;
	void Draw();
	void SetAllBaseColor(const Vector3& color);

	void ChangeInstanceCount(const uint32_t count);
	const uint32_t& GetInstanceCount() { return m_InstanceCount; }

	const Vector3& GetPosition() const { return m_Positions[0]; }
	const Vector3& GetPosition(uint32_t index) const { return m_Positions[index]; }
	void SetPosition(const Vector3& pos)
	{
		m_Positions[0] = pos;
		m_boundBoxes[0].Center = pos;
		m_boundSpheres[0].Center = pos;
		ComputeWorldTransform(0);
	}
	void SetPosition(const Vector3& pos, uint32_t index)
	{
		m_Positions[index] = pos;
		m_boundBoxes[index].Center = pos;
		m_boundSpheres[index].Center = pos;
		ComputeWorldTransform(index);
	}

	const Vector3& GetScale() const { return m_Scales[0]; }
	const Vector3& GetScale(uint32_t index) const { return m_Scales[index]; }
	void SetScale(const Vector3& scale)
	{
		m_Scales[0] = scale;
		m_boundBoxes[0].Extents.x *= scale.x;
		m_boundBoxes[0].Extents.y *= scale.y;
		m_boundBoxes[0].Extents.z *= scale.z;
		m_boundSpheres[0].Radius *= scale.x;
		ComputeWorldTransform(0);
	}
	void SetScale(const Vector3& scale, uint32_t index)
	{
		m_Scales[index] = scale;
		m_boundBoxes[index].Extents.x *= scale.x;
		m_boundBoxes[index].Extents.y *= scale.y;
		m_boundBoxes[index].Extents.z *= scale.z;
		m_boundSpheres[index].Radius *= scale.x;
		ComputeWorldTransform(index);
	}

	// BoundSphere, BoundBoxに対しては使えない
	const Quaternion& GetRotation() const { return m_Rotations[0]; }
	const Quaternion& GetRotation(uint32_t index) const { return m_Rotations[index]; }
	void SetRotation(const Quaternion& rotation)
	{
		m_Rotations[0] = rotation;
		ComputeWorldTransform(0);
	}
	void SetRotation(const Quaternion& rotation, uint32_t index)
	{
		m_Rotations[index] = rotation;
		ComputeWorldTransform(index);
	}

	const Matrix& GetWorldTransform() const { return m_Worlds[0]; }
	const Matrix& GetWorldTransform(uint32_t index) const { return m_Worlds[index]; }
	void SetWorldTransform(const Matrix& world){ m_Worlds[0] = world; }
	void SetWorldTransform(const Matrix& world, uint32_t index) { m_Worlds[index] = world; }

	const DirectX::BoundingSphere& GetBoundSphere() const { return m_boundSpheres[0]; }
	const DirectX::BoundingSphere& GetBoundSphere(uint32_t index) const { return m_boundSpheres[index]; }
	const DirectX::BoundingBox& GetBoundBox() const { return m_boundBoxes[0]; }
	const DirectX::BoundingBox& GetBoundBox(uint32_t index) const { return m_boundBoxes[index]; }
	void SetBoundSphereRadius(const float& value) { m_boundSpheres[0].Radius = value; }
	void SetBoundSphereRadius(const float& value, uint32_t index) { m_boundSpheres[index].Radius = value; }
	void SetBoundBoxExtents(const DirectX::XMFLOAT3& value) { m_boundBoxes[0].Extents = value; }
	void SetBoundBoxExtents(const DirectX::XMFLOAT3& value, uint32_t index) { m_boundBoxes[index].Extents = value; }

private:
	void CreateDescriptors();
	void CreateBufferView();
	std::shared_ptr<Texture>& CreateTextureMap(std::wstring& path, DirectX::ResourceUploadBatch& batch);
	bool CreateRootSignature();
	bool CreatePipelineState();

	void ComputeWorldTransform(uint32_t index);
	void ComputeWorldTransforms();

	ID3D12Device* m_pDevice = nullptr;
	ID3D12CommandQueue* m_pCmdQueue = nullptr;
	RenderSystem* m_pRenderSystem = nullptr;

	Game* m_pGame = nullptr;

	std::vector<VertexBuffer> m_VBs;
	std::vector<IndexBuffer>  m_IBs;
	std::vector<uint32_t> m_MaterialIds;
	std::vector<uint32_t> m_IndexCounts;

	std::vector<ResMesh> m_resMeshes;
	std::vector<ResMaterial> m_resMaterials;

	std::unordered_map<uint32_t, std::vector<ConstantBufferView>> m_CBVForLights;
	std::unordered_map<uint32_t, std::vector<ConstantBufferView>> m_CBVForWVPs;
	std::unordered_map<uint32_t, std::vector<ConstantBufferView>> m_CBVForMaterials;
	std::unordered_map<uint32_t, std::vector<ConstantBufferView>> m_CBVForInstanceWorlds;
	std::unordered_map<uint32_t, std::vector<ConstantBuffer>> m_CBForLights;
	std::unordered_map<uint32_t, std::vector<ConstantBuffer>> m_CBForWVPs;
	std::unordered_map<uint32_t, std::vector<ConstantBuffer>> m_CBForInstanceWorlds;
	std::unordered_map<uint32_t, std::vector<ConstantBuffer>> m_CBForMaterials;
	std::unordered_map<uint32_t, ShaderResourceView> m_SRVsForMaterials;
	std::unordered_map<std::wstring, std::shared_ptr<Texture>> m_pTextures;
	ResourceHeap m_ModelHeap; // モデル用のヒープ

	RootSignature m_rootSignature;
	PipelineState m_pipelineState;
	std::wstring m_ReadVsPath = L"shader/CSO/PhongVS.cso";
	std::wstring m_ReadPsPath = L"shader/CSO/PhongPS.cso";

	// ワールド座標
	std::vector<Matrix> m_Worlds = { Matrix::Identity };
	std::vector<Vector3> m_Positions = { Vector3::Zero };
	std::vector<Quaternion> m_Rotations = { Quaternion::Identity };
	std::vector<Vector3> m_Scales = { Vector3::One };

	// 衝突検知用(バウンディングスフィア)
	std::vector<DirectX::BoundingSphere> m_boundSpheres = {}; //!< バウンディングスフィア
	std::vector<DirectX::BoundingBox> m_boundBoxes = {}; //!< バウンディングボックス
	void CalclateBoundingSphereAndBox();
	DirectX::XMFLOAT3 MatrixToFloat3(const DirectX::XMMATRIX& matrix);
	DirectX::XMFLOAT3 MatrixToFloat3Scale(const DirectX::XMMATRIX& matrix);
	Quaternion MatrixToFloat4Rot(const DirectX::XMMATRIX& matrix);
	
	// インスタンス関連
	uint32_t m_InstanceCount = 1;

	static const uint32_t numDescCBVSRVUAV = 1000;
	uint32_t frameCount = 0;

	Mesh(const Mesh&) = delete;     // アクセス禁止.
	void operator = (const Mesh&) = delete;     // アクセス禁止.
};