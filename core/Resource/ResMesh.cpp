#include "Resource/ResMesh.h"

//! @brief メッシュのロード
//! @param[in] filename	  ファイル名
//! @param[in] meshes  読み込み用メッシュのベクター
//! @param[in] materials  読み込み用マテリアルのベクター
//! @return ロードに成功したか
bool MeshLoader::Load(const wchar_t* filename, std::vector<ResMesh>& meshes, std::vector<ResMaterial>& materials)
{
	if (filename == nullptr)
		return false;

	// wchar_tからchar型に変換
	auto path = Utility::WStringToString(filename);

	Assimp::Importer importer;
	unsigned int flag = 0;
	flag |= aiProcess_Triangulate;				//!< 面データをすべて三角形化
	flag |= aiProcess_PreTransformVertices;		//!< ノードの持つローカル変換行列で全ての頂点を計算
	flag |= aiProcess_CalcTangentSpace;			//!< インポートされたメッシュに対して接線ベクトルと従接線ベクトルを計算
	flag |= aiProcess_GenSmoothNormals;			//!< スムーズ法線ベクトルの生成
	flag |= aiProcess_GenUVCoords;				//!< 非UVマッピングを適切なテクスチャ座標のチャンネルに変換
	flag |= aiProcess_RemoveRedundantMaterials; //!< 冗長，参照されていないマテリアルを削除
	flag |= aiProcess_OptimizeMeshes;			//!< メッシュ数の最適化

	// ファイルの読み込み
	m_pScene = importer.ReadFile(path, flag);

	// チェック
	if (m_pScene == nullptr)
		return false;

	// メッシュのメモリを確保
	meshes.clear();
	meshes.resize(m_pScene->mNumMeshes);

	// メッシュデータを変換
	for (size_t i = 0; i < meshes.size(); ++i)
	{
		const auto pMesh = m_pScene->mMeshes[i];
		ParseMesh(meshes[i], pMesh);
	}

	materials.resize(m_pScene->mNumMaterials);
	// マテリアルデータを変換
	for (size_t i = 0; i < materials.size(); ++i)
	{
		const auto pMaterial = m_pScene->mMaterials[i];
		ParseMaterial(materials[i], pMaterial);
	}

	// 不要になったのでクリア
	importer.FreeScene();
	m_pScene = nullptr;

	return true;
}

//! @brief メッシュデータの解析
//! @param[in] dstMesh	メッシュデータ
//! @param[in] pSrcMesh	assimpで得たメッシュデータ
void MeshLoader::ParseMesh(ResMesh& dstMesh, const aiMesh* pSrcMesh)
{
	// マテリアル番号を設定
	dstMesh.MaterialId = pSrcMesh->mMaterialIndex;

	aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// 頂点データのメモリを確保
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

	// 頂点インデックスのメモリを確保
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

//! @brief マテリアルデータの解析
//! @param[in] dstMesh	マテリアルデータ
//! @param[in] pSrcMesh	assimpで得たマテリアルデータ
void MeshLoader::ParseMaterial(ResMaterial& dstMaterial, const aiMaterial* pSrcMaterial)
{
	// 拡散反射成分
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

	// 鏡面反射成分.
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

	// 鏡面反射強度.
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

	// ディフューズマップ
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
			dstMaterial.DiffuseMap = Utility::aiStringToWString(path);
		else
			dstMaterial.DiffuseMap.clear();
	}

	// スペキュラマップ
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SPECULAR(0), path) == AI_SUCCESS)
			dstMaterial.SpecularMap = Utility::aiStringToWString(path);
		else
			dstMaterial.SpecularMap.clear();
	}

	// シャイネスマップ
	{
		aiString path;
		if (pSrcMaterial->Get(AI_MATKEY_TEXTURE_SHININESS(0), path) == AI_SUCCESS)
			dstMaterial.ShininessMap = Utility::aiStringToWString(path);
		else
			dstMaterial.ShininessMap.clear();
	}

	// 法線マップ
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