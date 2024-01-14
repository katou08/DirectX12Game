#include "Resource/Mesh.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/ConstantBuffer.h"
#include "Game/Camera.h"
#include "Game/Light.h"

namespace {
	const wchar_t* DummyTag = L"";
}

//! @brief �R���X�g���N�^
//! @param[in] game  �Q�[���N���X
Mesh::Mesh(Game* game) : m_pGame(game)
{
}

//! @brief �f�X�g���N�^
Mesh::~Mesh()
{
}

//! @brief �������֐�
//! @param[in] srcPath obj�t�@�C����
//! @param[in] vsPath vs�̃t�@�C����
//! @param[in] psPath ps�̃t�@�C����
//! @return  �������ɐ���������
bool Mesh::Init(const std::wstring srcPath, const std::wstring vsPath, const std::wstring psPath)
{
	m_pRenderSystem = m_pGame->GetGraphicsEngine()->GetRenderSystem();
	m_pDevice = m_pRenderSystem->GetDevice();
	m_pCmdQueue = m_pRenderSystem->GetCmdQueue();
	frameCount = m_pRenderSystem->GetFrameCount();

	// ���b�V�������[�h
	std::wstring meshPath;

	if (!SearchFilePath(srcPath.c_str(), meshPath))
		ThrowFailed("obj�t�@�C��������܂���.");
	
	std::wstring dir = GetDirectoryPath(meshPath.c_str()); // �f�B���N�g���t�@�C�����擾

	// ���b�V�������[�h
	if(!LoadMesh(meshPath.c_str(), m_resMeshes, m_resMaterials))
		ThrowFailed("���b�V���̃��[�h�����s���܂���.");

	// �������œK��
	m_resMeshes.shrink_to_fit();

	m_VBs.resize(m_resMeshes.size());
	m_IBs.resize(m_resMeshes.size());
	m_MaterialIds.resize(m_resMeshes.size());
	m_IndexCounts.resize(m_resMeshes.size());

	m_Worlds.resize(m_InstanceCount);
	m_boundSpheres.resize(m_InstanceCount);
	m_boundBoxes.resize(m_InstanceCount);

	auto size = m_resMeshes.size();
	for (int i = 0; i < size; ++i)
	{
		// ���_�o�b�t�@�r���[�̍쐬
		m_VBs[i].Init(m_pDevice, sizeof(MeshVertex) * m_resMeshes[i].Vertices.size(), m_resMeshes[i].Vertices.data());

		// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
		m_IBs[i].Init(m_pDevice, sizeof(uint32_t) * m_resMeshes[i].Indices.size(), m_resMeshes[i].Indices.data());
	
		m_MaterialIds[i] = m_resMeshes[i].MaterialId;
		m_IndexCounts[i] = static_cast<uint32_t>(m_resMeshes[i].Indices.size());
	}
	// �Փˌ��m�p�o�E���f�B���O�X�t�B�A�̍쐬
	CalclateBoundingSphereAndBox();

	// ���̃��f���p�̃f�B�X�N���v�^���쐬
	CreateDescriptors();

	// �}�e���A�����Ƃ̃r���[���쐬
	CreateBufferView();

	// �V�F�[�_�[�̐ݒ�
	if (vsPath != L"" && psPath != L"")
	{
		m_ReadVsPath = vsPath;
		m_ReadPsPath = psPath;
	}

	// ���[�g�V�O�l�`���̍쐬
	if (!CreateRootSignature())
		return false;

	// PSO�̍쐬
	if (!CreatePipelineState())
		return false;

	return true;
}

//! @brief �`�揈��
void Mesh::Draw()
{
	auto renderContext = m_pRenderSystem->GetRenderContext();
	auto frameIndex = m_pRenderSystem->GetFrameIndex();

	renderContext->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderContext->SetDescriptorHeap(m_ModelHeap.GetHeap());
	renderContext->SetRootSignature(m_rootSignature.GetPtr());
	renderContext->SetPipelineState(m_pipelineState.GetPtr());

	auto& view = m_pGame->GetCamera()->GetView();
	auto& proj = m_pGame->GetCamera()->GetProj();
	auto size = m_resMeshes.size();
	auto light = m_pGame->GetLight();
	auto cameraPos = m_pGame->GetCamera()->GetPosition();

	for (int i = 0; i < size; ++i)
	{
		// �R���X�^���g�o�b�t�@�̍X�V
		auto pTransform = m_CBForWVPs[i][frameIndex].GetPtr<ViewProjTransform>();
		pTransform->View = view;
		pTransform->Proj = proj;

		auto pInstWorld = m_CBForInstanceWorlds[i][frameIndex].GetPtr<WorldTransform>();
		for (int j = 0; j < m_InstanceCount; ++j)
		{
			pInstWorld->WorldInstanceData[j] = m_Worlds[j];
		}

		auto ptr = m_CBForLights[i][frameIndex].GetPtr<LightBuffer>();
		ptr->LightPosition	= light->GetPosition();
		ptr->LightColor		= light->GetColor();
		ptr->CameraPosition = Vector4(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);

		auto matptr = m_CBForMaterials[i][frameIndex].GetPtr<MaterialBuffer>();
		matptr->Difuuse = m_resMaterials[i].Diffuse;
		matptr->Alpha = m_resMaterials[i].Alpha;
		matptr->Specular = m_resMaterials[i].Specular;
		matptr->Shininess = m_resMaterials[i].Shininess;

		// �f�B�X�N���v�^�e�[�u���̃Z�b�g
		auto address = m_CBVForWVPs[i][frameIndex].GetGPUDescHandle();
		renderContext->SetGraphicsRootDescriptorTable(0, address);
		address = m_CBVForInstanceWorlds[i][frameIndex].GetGPUDescHandle();
		renderContext->SetGraphicsRootDescriptorTable(1, address);
		address = m_CBVForLights[i][frameIndex].GetGPUDescHandle();
		renderContext->SetGraphicsRootDescriptorTable(2, address);
		address = m_CBVForMaterials[i][frameIndex].GetGPUDescHandle();
		renderContext->SetGraphicsRootDescriptorTable(3, address);
		address = m_SRVsForMaterials[i].GetGPUDescHandle();
		renderContext->SetGraphicsRootDescriptorTable(4, address);

		// ���_�o�b�t�@�C�C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g
		renderContext->SetVertexBuffer(m_VBs[i]);
		renderContext->SetIndexBuffer(m_IBs[i]);
		// �`��
		renderContext->DrawIndexedInstanced(m_IndexCounts[i], m_InstanceCount);
	}

}

//! @brief �f�B�t���[�Y�J���[�̕ύX
//! @param[in] color �J���[
void Mesh::SetAllBaseColor(const Vector3& color)
{
	if (m_resMeshes.size() > 0)
	{
		for (int i = 0; i < m_resMaterials.size(); ++i)
		{
			m_resMaterials[i].Diffuse = color;
		}
	}
}

//! @brief �C���X�^���X���̕ύX
//! @pamam[in] �C���X�^���X��
void Mesh::ChangeInstanceCount(const uint32_t count)
{
	m_InstanceCount = count;
	m_Worlds.resize(m_InstanceCount);
	m_Positions.resize(m_InstanceCount);
	m_Rotations.resize(m_InstanceCount);
	m_Scales.resize(m_InstanceCount);
	m_boundSpheres.resize(m_InstanceCount);
	m_boundBoxes.resize(m_InstanceCount);
}

void Mesh::CreateDescriptors()
{
	// �f�B�X�N���v�^�̍쐬
	m_ModelHeap.Create(m_pDevice, "CBVSRVUAVForMaterial", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numDescCBVSRVUAV);
}

void Mesh::CreateBufferView()
{
    // �_�~�[�e�N�X�`���̍쐬
    {
        auto pTexture = std::make_shared<Texture>();
        if (pTexture == nullptr)
			ThrowFailed("�_�~�[�e�N�X�`���̍쐬�Ɏ��s���܂���.");

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        desc.Width              = 1;
        desc.Height             = 1;
        desc.DepthOrArraySize   = 1;
        desc.MipLevels          = 1;
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;

        if (!pTexture->Init(m_pDevice, &desc, false, false))
            ThrowFailed("�_�~�[�e�N�X�`���̍쐬�Ɏ��s���܂���.");

        m_pTextures[DummyTag] = pTexture;
    }

	// �t���[�������o�b�t�@���쐬
	for (int i = 0; i < m_resMaterials.size(); ++i)
	{
		for (unsigned int j = 0; j < frameCount; ++j)
		{
			ConstantBuffer lightcb = {};
			lightcb.Init(m_pDevice, sizeof(LightBuffer), "LightBuffer");
			m_CBForLights[i].push_back(lightcb);
			ConstantBuffer wvpcb = {};
			wvpcb.Init(m_pDevice, sizeof(ViewProjTransform), "TransformBuffer");
			m_CBForWVPs[i].push_back(wvpcb);
			ConstantBuffer instcb = {};
			instcb.Init(m_pDevice, sizeof(WorldTransform), "InstWorldBuffer");
			m_CBForInstanceWorlds[i].push_back(instcb);
			ConstantBuffer matcb = {};
			matcb.Init(m_pDevice, sizeof(MaterialBuffer), "MaterialBuffer");
			m_CBForMaterials[i].push_back(matcb);
		}
	}

	// ���\�[�X�o�b�`��p��
	DirectX::ResourceUploadBatch batch(m_pDevice);
	batch.Begin();

	auto cameraPos = m_pGame->GetCamera()->GetPosition();
	auto light = m_pGame->GetLight();

	// �}�e���A���̐� * �t���[�������r���[�̍쐬
	for (unsigned int i = 0; i < m_resMaterials.size(); ++i)
	{
		for (unsigned int j = 0; j < frameCount; ++j)
		{

			// View, Proj�s��p�̃R���X�^���g�o�b�t�@�r���[���쐬
			ConstantBufferView cbv = {};
			m_ModelHeap.AllocDescriptor(1, &cbv);
			m_CBVForWVPs[i].push_back(cbv);
			auto viewDesc = m_CBForWVPs[i][j].GetDesc();
			m_pDevice->CreateConstantBufferView(&viewDesc, cbv.GetCPUDescHandle());

			auto viewprojTransformptr = m_CBForWVPs[i][j].GetPtr<ViewProjTransform>();
			viewprojTransformptr->View = Matrix::Identity;
			viewprojTransformptr->Proj = Matrix::Identity;

			// �C���X�^���V���O�p�̃��[���h�s��̃R���X�^���g�o�b�t�@�r���[���쐬
			ConstantBufferView instcbv = {};
			m_ModelHeap.AllocDescriptor(1, &instcbv);
			m_CBVForInstanceWorlds[i].push_back(instcbv);
			viewDesc = m_CBForInstanceWorlds[i][j].GetDesc();
			m_pDevice->CreateConstantBufferView(&viewDesc, instcbv.GetCPUDescHandle());

			auto instWorldptr = m_CBForInstanceWorlds[i][j].GetPtr<WorldTransform>();
			for (int j = 0; j < InstCount::InstanceDataMax; ++j)
			{
				instWorldptr->WorldInstanceData[j] = Matrix::Identity;
			}

			// ���C�g�p�̃R���X�^���g�o�b�t�@�r���[���쐬
			ConstantBufferView licbv = {};
			m_ModelHeap.AllocDescriptor(1, &licbv);
			m_CBVForLights[i].push_back(licbv);
			viewDesc = m_CBForLights[i][j].GetDesc();
			m_pDevice->CreateConstantBufferView(&viewDesc, licbv.GetCPUDescHandle());

			auto ptr = m_CBForLights[i][j].GetPtr<LightBuffer>();
			ptr->LightPosition	= light->GetPosition();
			ptr->LightColor		= light->GetColor();
			ptr->CameraPosition = Vector4(cameraPos.x, cameraPos.y,cameraPos.z, 0.0f);

			// �}�e���A���p�̃R���X�^���g�o�b�t�@�r���[���쐬
			ConstantBufferView matcbv = {};
			m_ModelHeap.AllocDescriptor(1, &matcbv);
			m_CBVForMaterials[i].push_back(matcbv);
			viewDesc = m_CBForMaterials[i][j].GetDesc();
			m_pDevice->CreateConstantBufferView(&viewDesc, matcbv.GetCPUDescHandle());

			auto matptr = m_CBForMaterials[i][j].GetPtr<MaterialBuffer>();
			matptr->Difuuse	  = m_resMaterials[i].Diffuse;
			matptr->Alpha	  = m_resMaterials[i].Alpha;
			matptr->Specular  = m_resMaterials[i].Specular;
			matptr->Shininess = m_resMaterials[i].Shininess;
		}

		// �}�e���A���p�e�N�X�`���̃V�F�[�_�[���\�[�X�r���[���쐬
		auto pTexture = CreateTextureMap(m_resMaterials[i].DiffuseMap, batch);
		if (pTexture != nullptr)
		{
			auto viewDesc = pTexture->GetDesc();
			ShaderResourceView srv = {};
			m_SRVsForMaterials[i] = srv;
			m_ModelHeap.AllocDescriptor(1, &m_SRVsForMaterials[i]);
			m_pDevice->CreateShaderResourceView(pTexture->GetBuffer(), &viewDesc, m_SRVsForMaterials[i].GetCPUDescHandle());
		}
	}
	auto future = batch.End(m_pCmdQueue); // �o�b�`�I��
	future.wait(); 	// �o�b�`������ҋ@
}

std::shared_ptr<Texture>& Mesh::CreateTextureMap(std::wstring& path, DirectX::ResourceUploadBatch& batch)
{
	// ���ɓo�^�ς݂��ǂ���
	if (m_pTextures.find(path) != m_pTextures.end())
		return m_pTextures[path];

	// �t�@�C���p�X�����݂��邩�ǂ����`�F�b�N
	std::wstring findPath;
	if (!SearchFilePath(path.c_str(), findPath))
		return m_pTextures[DummyTag]; //!< ���݂��Ȃ��ꍇ�̓_�~�[�e�N�X�`����ݒ�

	// �t�@�C�����ł��邱�Ƃ��`�F�b�N
	if (PathIsDirectory(findPath.c_str()) != FALSE)
		return m_pTextures[DummyTag]; //!< ���݂��Ȃ��ꍇ�̓_�~�[�e�N�X�`����ݒ�

	// �C���X�^���X�𐶐�
	auto pTexture = std::make_shared<Texture>();
	if (pTexture == nullptr)
		ThrowFailed("�e�N�X�`���̐����Ɏ��s.");

	// ������
	if (!pTexture->Init(m_pDevice, findPath.c_str(), false, batch, false))
		ThrowFailed("�e�N�X�`���̐����Ɏ��s.");

	// �o�^
	m_pTextures[path] = pTexture;

	return m_pTextures[path];
}

bool Mesh::CreateRootSignature()
{
	RootSignature::Desc desc;
	desc.Begin(5)
		.SetCBV(ShaderStage::VS, 0, 0) //!< WVP�o�b�t�@
		.SetCBV(ShaderStage::VS, 1, 1) //!< InstWorld�o�b�t�@
		.SetCBV(ShaderStage::PS, 2, 2) //!< light�o�b�t�@
		.SetCBV(ShaderStage::PS, 3, 3) //!< Material�o�b�t�@
		.SetSRV(ShaderStage::PS, 4, 0) //!< Material(DiffuseTex)
		.AddStaticSmp(ShaderStage::PS, 0, SamplerState::LinearWrap)
		.AllowIL() //���̓��C�A�E�g������
		.End();

	if (!m_rootSignature.Init(m_pDevice, desc.GetDesc()))
		ThrowFailed("���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���.");

	return true;
}

bool Mesh::CreatePipelineState()
{
	// ���X�^���C�Y�X�e�[�g�̐ݒ�
	D3D12_RASTERIZER_DESC descRS = {};
	descRS.FillMode = D3D12_FILL_MODE_SOLID;
	descRS.CullMode = D3D12_CULL_MODE_NONE;
	descRS.FrontCounterClockwise = FALSE;
	descRS.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	descRS.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	descRS.DepthClipEnable = FALSE;
	descRS.MultisampleEnable = FALSE;
	descRS.AntialiasedLineEnable = FALSE;
	descRS.ForcedSampleCount = 0;
	descRS.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {
		FALSE, FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL
	};

	// �u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS = {};
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		descBS.RenderTarget[i] = descRTBS;

	// �[�x�X�e���V���X�e�[�g�̐ݒ�
	D3D12_DEPTH_STENCIL_DESC descDSS = {};
	descDSS.DepthEnable = TRUE;
	descDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	descDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	descDSS.StencilEnable = FALSE;

	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;

	std::wstring vsPath;
	std::wstring psPath;
	// �V�F�[�_�[�ǂݍ���
	// �t�@�C���p�X������
	if (!SearchFilePath(m_ReadVsPath.c_str(), vsPath))
		ThrowFailed("���_�V�F�[�_�[�ǂݍ��݂Ɏ��s���܂���");
	if (!SearchFilePath(m_ReadPsPath.c_str(), psPath))
		ThrowFailed("�s�N�Z���V�F�[�_�[�ǂݍ��݂Ɏ��s���܂���");

	auto hr = D3DReadFileToBlob(vsPath.c_str(), vsBlob.GetAddressOf());
	ThrowIfFailed(hr, "���_�V�F�[�_�[�ǂݍ��݂Ɏ��s���܂���.");
	hr = D3DReadFileToBlob(psPath.c_str(), psBlob.GetAddressOf());
	ThrowIfFailed(hr, "�s�N�Z���V�F�[�_�[�ǂݍ��݂Ɏ��s���܂���.");

	// �p�C�v���C���X�e�[�g�̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.InputLayout = MeshVertex::InputLayout;
	desc.pRootSignature = m_rootSignature.GetPtr();
	desc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
	desc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };
	desc.RasterizerState = descRS;
	desc.BlendState = descBS;
	desc.DepthStencilState = descDSS;
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	if (!m_pipelineState.Init(m_pDevice, &desc))
		ThrowFailed("�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���.");

	return true;
}

void Mesh::CalclateBoundingSphereAndBox()
{
	if (m_resMeshes.size() == 0)
		return;

	// X, Y, Z���ꂼ��̍ŏ��E�ő���W���擾
	float minX, minY, minZ, maxX, maxY, maxZ;
	minX = maxX = m_resMeshes[0].Vertices[0].Position.x;
	minY = maxY = m_resMeshes[0].Vertices[0].Position.y;
	minZ = maxZ = m_resMeshes[0].Vertices[0].Position.z;

	auto meshSize = m_resMeshes.size();
	for (unsigned int i = 0; i < meshSize; ++i)
	{
		auto vertSize = m_resMeshes[i].Vertices.size();
		for (unsigned int j = 0; j < vertSize; ++j)
		{
			auto vertex = m_resMeshes[i].Vertices[j].Position;

			minX = std::min(minX, vertex.x);
			minY = std::min(minY, vertex.y);
			minZ = std::min(minZ, vertex.z);

			maxX = std::max(maxX, vertex.x);
			maxY = std::max(maxY, vertex.y);
			maxZ = std::max(maxZ, vertex.z);
		}
	}

	// Bounding Sphere �̒��S���W���v�Z
	float centerX = (minX + maxX) * 0.5f;
	float centerY = (minY + maxY) * 0.5f;
	float centerZ = (minZ + maxZ) * 0.5f;

	// Bounding Sphere �̔��a���v�Z
	float radius = std::max({ maxX - centerX, maxY - centerY, maxZ - centerZ });

	// Bounding Box�̒��S���W
	Vector3 BoxCenter = Vector3(centerX, centerY, centerZ);
	//Bounding Box�� ���S����e�ӂ܂ł̋���
	Vector3 BoxExtents = Vector3(maxX - centerX, maxY - centerY, maxZ - centerZ);

	for (int i = 0; i < m_InstanceCount; ++i)
	{
		m_boundSpheres[i] = DirectX::BoundingSphere(DirectX::XMFLOAT3(centerX, centerY, centerZ), radius);
		m_boundBoxes[i] = DirectX::BoundingBox(BoxCenter, BoxExtents);
	}
}

DirectX::XMFLOAT3 Mesh::MatrixToFloat3(const DirectX::XMMATRIX& matrix)
{
	// �s��𕽍s�ړ��A��]�A�X�P�[�����O�ɕ���
	DirectX::XMVECTOR scale, rotationQuat, translation;
	DirectX::XMMatrixDecompose(&scale, &rotationQuat, &translation, matrix);

	// �������ꂽ����������W�x�N�g�����쐬
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, translation);

	return result;
}

DirectX::XMFLOAT3 Mesh::MatrixToFloat3Scale(const DirectX::XMMATRIX& matrix)
{
	// �s��𕽍s�ړ��A��]�A�X�P�[�����O�ɕ���
	DirectX::XMVECTOR scale, rotationQuat, translation;
	DirectX::XMMatrixDecompose(&scale, &rotationQuat, &translation, matrix);

	// �������ꂽ��������X�P�[�����O�x�N�g�����쐬
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, scale);

	return result;
}

Quaternion Mesh::MatrixToFloat4Rot(const DirectX::XMMATRIX& matrix)
{
	// �s��𕽍s�ړ��A��]�A�X�P�[�����O�ɕ���
	DirectX::XMVECTOR scale, rotationQuat, translation;
	DirectX::XMMatrixDecompose(&scale, &rotationQuat, &translation, matrix);

	// �������ꂽ����������W�x�N�g�����쐬
	Quaternion result;
	DirectX::XMStoreFloat4(&result, rotationQuat);

	return result;
}

void Mesh::ComputeWorldTransform(uint32_t index)
{
	Matrix temp = Matrix::Identity;

	m_Worlds[index] = Matrix::Identity;
	// �܂��̓X�P�[�����O�C���ɉ�]�C�Ō�ɕ��s�ړ�
	temp = DirectX::XMMatrixScaling(m_Scales[index].x, m_Scales[index].y, m_Scales[index].z);
	m_Worlds[index] *= temp;

	temp = Matrix::Identity;
	temp = DirectX::XMMatrixRotationQuaternion(m_Rotations[index]);
	m_Worlds[index] *= temp;

	temp = Matrix::Identity;
	temp = DirectX::XMMatrixTranslation(m_Positions[index].x, m_Positions[index].y, m_Positions[index].z);
	m_Worlds[index] *= temp;
}

void Mesh::ComputeWorldTransforms()
{
	for (int i = 0; i < m_InstanceCount; ++i)
	{
		Matrix temp = Matrix::Identity;

		m_Worlds[i] = Matrix::Identity;
		// �܂��̓X�P�[�����O�C���ɉ�]�C�Ō�ɕ��s�ړ�
		temp = DirectX::XMMatrixScaling(m_Scales[i].x, m_Scales[i].y, m_Scales[i].z);
		m_Worlds[i] *= temp;

		temp = Matrix::Identity;
		temp = DirectX::XMMatrixRotationQuaternion(m_Rotations[i]);
		m_Worlds[i] *= temp;

		temp = Matrix::Identity;
		temp = DirectX::XMMatrixTranslation(m_Positions[i].x, m_Positions[i].y, m_Positions[i].z);
		m_Worlds[i] *= temp;
	}
}
