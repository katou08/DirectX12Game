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
	DirectX::XMFLOAT3 Diffuse;	//!< �@�����ː���
	DirectX::XMFLOAT3 Specular; //!< ���ʔ��ː���
	float Alpha;				//!< ���ߐ���
	float Shininess;			//!< ���ʔ��ˋ��x
	std::wstring DiffuseMap;	//!< �f�B�t���[�Y�}�b�v
	std::wstring SpecularMap;	//!< �X�y�L�����}�b�v
	std::wstring ShininessMap;	//!< �V���C�l�X�}�b�v
	std::wstring NormalMap;		//!< �m�[�}���}�b�v
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
	const aiScene* m_pScene = nullptr; //!< �V�[���f�[�^

	void ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh);
	void ParseMaterial(ResMaterial& dstmaterial, const aiMaterial* pSrcMaterial);
};

bool LoadMesh(
	const wchar_t* filename,
	std::vector<ResMesh>& meshes,
	std::vector<ResMaterial>& materials);