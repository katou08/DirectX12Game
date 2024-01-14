#include "Resource/ResMesh.h"

//! @brief ���b�V���̃��[�h
//! @param[in] filename	  �t�@�C����
//! @param[in] meshes  �ǂݍ��ݗp���b�V���̃x�N�^�[
//! @param[in] materials  �ǂݍ��ݗp�}�e���A���̃x�N�^�[
//! @return ���[�h�ɐ���������
bool MeshLoader::Load(const wchar_t* filename, std::vector<ResMesh>& meshes, std::vector<ResMaterial>& materials)
{
	if (filename == nullptr)
		return false;

	// wchar_t����char�^�ɕϊ�
	auto path = Utility::WStringToString(filename);

	Assimp::Importer importer;
	unsigned int flag = 0;
	flag |= aiProcess_Triangulate;				//!< �ʃf�[�^�����ׂĎO�p�`��
	flag |= aiProcess_PreTransformVertices;		//!< �m�[�h�̎����[�J���ϊ��s��őS�Ă̒��_���v�Z
	flag |= aiProcess_CalcTangentSpace;			//!< �C���|�[�g���ꂽ���b�V���ɑ΂��Đڐ��x�N�g���Ə]�ڐ��x�N�g�����v�Z
	flag |= aiProcess_GenSmoothNormals;			//!< �X���[�Y�@���x�N�g���̐���
	flag |= aiProcess_GenUVCoords;				//!< ��UV�}�b�s���O��K�؂ȃe�N�X�`�����W�̃`�����l���ɕϊ�
	flag |= aiProcess_RemoveRedundantMaterials; //!< �璷�C�Q�Ƃ���Ă��Ȃ��}�e���A�����폜
	flag |= aiProcess_OptimizeMeshes;			//!< ���b�V�����̍œK��

	// �t�@�C���̓ǂݍ���
	m_pScene = importer.ReadFile(path, flag);

	// �`�F�b�N
	if (m_pScene == nullptr)
		return false;

	// ���b�V���̃��������m��
	meshes.clear();
	meshes.resize(m_pScene->mNumMeshes);

	// ���b�V���f�[�^��ϊ�
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = m_pScene->mMeshes[i];
		ParseMesh(meshes[i], pMesh);
	}

	materials.resize(m_pScene->mNumMaterials);
	// �}�e���A���f�[�^��ϊ�
	for (size_t i = 0; i < materials.size(); ++i)
	{
		const auto pMaterial = m_pScene->mMaterials[i];
		ParseMaterial(materials[i], pMaterial);
	}

	// �s�v�ɂȂ����̂ŃN���A
	importer.FreeScene();
	m_pScene = nullptr;

	return true;
}

//! @brief ���b�V���f�[�^�̉��
//! @param[in] dstMesh	���b�V���f�[�^
//! @param[in] pSrcMesh	assimp�œ������b�V���f�[�^
void MeshLoader::ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh)
{
	// �}�e���A���ԍ���ݒ�
	dstMesh.MaterialId = pSrcMesh->mMaterialIndex;

	aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// ���_�f�[�^�̃��������m��
	dstMesh.Vertices.resize(pSrcMesh->mNumVertices);

	for (auto i = 0u; i < pSrcMesh->mNumVertices; ++i)
	{
		auto pPosition = &(pSrcMesh->mVertices[i]);
		auto pNormal = &(pSrcMesh->mNormals[i]);
		auto pTexCoord = (pSrcMesh->HasTextureCoords(0)) ? 
							&(pSrcMesh->mTextureCoords[0][i]) : &Zero3D;
		auto pTangent = (pSrcMesh->HasTangentsAndBitangents()) ?
							&(pSrcMesh->mTangents[i]) : &Zero3D;

		dstMesh.Vertices[i] = MeshVertex(
			DirectX::XMFLOAT3(pPosition->x, pPosition->y, pPosition->z),
			DirectX::XMFLOAT3(pNormal->x, pNormal->y, pNormal->z),
			DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y),
			DirectX::XMFLOAT3(pTangent->x, pTangent->y, pTangent->z)
		);
	}

	// ���_�C���f�b�N�X�̃��������m��
	dstMesh.Indices.resize(pSrcMesh->mNumFaces * 3);

	for (auto i = 0; i < pSrcMesh->mNumFaces; ++i)
	{
		const auto& face = pSrcMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		dstMesh.Indices[i * 3 + 0] = face.mIndices[0];
		dstMesh.Indices[i * 3 + 1] = face.mIndices[1];
		dstMesh.Indices[i * 3 + 2] = face.mIndices[2];
	}
}

//! @brief �}�e���A���f�[�^�̉��
//! @param[in] dstMesh	�}�e���A���f�[�^
//! @param[in] pSrcMesh	assimp�œ����}�e���A���f�[�^
void MeshLoader::ParseMaterial(ResMaterial& dstMaterial, const aiMaterial* pSrcMaterial)
{
	// �g�U���ː���
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (pSrcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			dstMaterial.Diffuse.x = color.r;
			dstMaterial.Diffuse.y = color.g;
			dstMaterial.Diffuse.z = color.b;
		}
		else
		{
			dstMaterial.Diffuse.x = 0.5f;
			dstMaterial.Diffuse.y = 0.5f;
			dstMaterial.Diffuse.z = 0.5f;
		}
	}

	// ���ʔ��ː���.
	{
		aiColor3D color(0.0f, 0.0f, 0.0f);

		if (pSrcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
		{
			dstMaterial.Specular.x = color.r;
			dstMaterial.Specular.y = color.g;
			dstMaterial.Specular.z = color.b;
		}
		else
		{
			dstMaterial.Specular.x = 0.0f;
			dstMaterial.Specular.y = 0.0f;
			dstMaterial.Specular.z = 0.0f;
		}
	}

	// ���ʔ��ˋ��x.
	{
		auto shininess = 0.0f;
		if (pSrcMaterial->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
		{
			dstMaterial.Shininess = shininess;
		}
		else
		{
			dstMaterial.Shininess = 0.0f;
		}
	}

	// �f�B�t���[�Y�}�b�v
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
			dstMaterial.DiffuseMap = Utility::aiStringToWString(path);
		else
			dstMaterial.DiffuseMap.clear();
	}

	// �X�y�L�����}�b�v
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS)
			dstMaterial.SpecularMap = Utility::aiStringToWString(path);
		else
			dstMaterial.SpecularMap.clear();
	}

	// �V���C�l�X�}�b�v
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS)
			dstMaterial.ShininessMap = Utility::aiStringToWString(path);
		else
			dstMaterial.ShininessMap.clear();
	}

	// �@���}�b�v
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_NORMALS(0), path) == AI_SUCCESS)
			dstMaterial.NormalMap = Utility::aiStringToWString(path);
		else
		{
			if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_HEIGHT(0), path) == AI_SUCCESS)
				dstMaterial.NormalMap = Utility::aiStringToWString(path);
			else
				dstMaterial.NormalMap.clear();
		}
	}
}

bool LoadMesh
(
	const wchar_t* filename,
	std::vector<ResMesh>& meshes,
	std::vector<ResMaterial>& materials)
{
	MeshLoader loader;
	return loader.Load(filename, meshes, materials);
}

const D3D12_INPUT_ELEMENT_DESC MeshVertex::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};

const D3D12_INPUT_LAYOUT_DESC MeshVertex::InputLayout = { MeshVertex::InputElements, MeshVertex::InputElementCount };
static_assert(sizeof(MeshVertex) == 44, "Vertex struct/layout mismatch");