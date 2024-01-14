#pragma once
#include "pch.h"

class MeshVertex
{
public:
	MeshVertex() {}

	MeshVertex(
		DirectX::XMFLOAT3 const& position,
		DirectX::XMFLOAT3 const& normal,
		DirectX::XMFLOAT2 const& texcoord,
		DirectX::XMFLOAT3 const& tangent)
		: Position(position)
		, Normal(normal)
		, TexCoord(texcoord)
		, Tangent(tangent)
	{}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
	DirectX::XMFLOAT3 Tangent;
	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

private:
	static const uint32_t InputElementCount = 4;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

struct ResMaterial
{
	DirectX::XMFLOAT3 Diffuse;	//!< 法線反射成分
	DirectX::XMFLOAT3 Specular; //!< 鏡面反射成分
	float Alpha;				//!< 透過成分
	float Shininess;			//!< 鏡面反射強度
	std::wstring DiffuseMap;	//!< ディフューズマップ
	std::wstring SpecularMap;	//!< スペキュラマップ
	std::wstring ShininessMap;	//!< シャイネスマップ
	std::wstring NormalMap;		//!< ノーマルマップ
};

struct ResMesh
{
	std::vector<MeshVertex> Vertices;
	std::vector<uint32_t>	Indices;
	uint32_t MaterialId;
};

class MeshLoader
{
public:
	MeshLoader() {}
	bool Load(const wchar_t* filename, std::vector<ResMesh>& meshes, std::vector<ResMaterial>& materials);

private:
	const aiScene* m_pScene = nullptr; //!< シーンデータ

	void ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh);
	void ParseMaterial(ResMaterial& dstmaterial, const aiMaterial* pSrcMaterial);
};

bool LoadMesh(
	const wchar_t* filename,
	std::vector<ResMesh>& meshes,
	std::vector<ResMaterial>& materials);