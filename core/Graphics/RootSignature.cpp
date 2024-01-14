#include "RootSignature.h"

//! @brief ����������
//! @param[in] pDevice �f�o�C�X
//! @param[in] pDesc ���[�g�V�O�l�`���̐ݒ�
//! @return �������ɐ���������
bool RootSignature::Init(ID3D12Device* pDevice,
	const D3D12_ROOT_SIGNATURE_DESC* pDesc)
{
	ComPtr<ID3DBlob> pBlob;
	ComPtr<ID3DBlob> pErrorBlob;

	// �V���A���C�Y����
	auto hr = D3D12SerializeRootSignature(
		pDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		pBlob.GetAddressOf(),
		pErrorBlob.GetAddressOf());
	if (FAILED(hr))
		return false;

	// ���[�g�V�O�l�`���̐���
	hr = pDevice->CreateRootSignature(
		0,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		IID_PPV_ARGS(m_rootSignature.GetAddressOf()));
	if (FAILED(hr))
		return false;

	return true;
}

//! @brief �R���X�g���N�^
RootSignature::Desc::Desc()
	:m_Desc(),
	m_Flags(0)
{
	for (auto i = 0; i < 5; ++i)
	{
		m_DenyStage[i] = true;
	}
}

//! @brief �f�X�g���N�^
RootSignature::Desc::~Desc()
{
	m_Ranges.clear();
	m_Samplers.clear();
	m_Params.clear();
}

//! @brief ���[�g�V�O�l�`���̐ݒ�̃Z�b�g�A�b�v
//! @brief count ���[�g�p�����[�^�̐�(�����W�̐�)
RootSignature::Desc& RootSignature::Desc::Begin(int count)
{
	m_Flags = 0;
	for (auto i = 0; i < 5; ++i)
	{
		m_DenyStage[i] = true;
	}

	memset(&m_Desc, 0, sizeof(m_Desc));

	m_Samplers.clear();
	m_Ranges.resize(count);
	m_Params.resize(count);
	return *this;
}

//! @brief �萔�o�b�t�@�r���[��ݒ�
RootSignature::Desc& RootSignature::Desc::SetCBV(ShaderStage stage, int index, uint32_t reg)
{
	SetParam(stage, index, reg, D3D12_DESCRIPTOR_RANGE_TYPE_CBV);
	return *this;
}

//! @brief �V�F�[�_���\�[�X�r���[��ݒ�
RootSignature::Desc& RootSignature::Desc::SetSRV(ShaderStage stage, int index, uint32_t reg)
{
	SetParam(stage, index, reg, D3D12_DESCRIPTOR_RANGE_TYPE_SRV);
	return *this;
}

//! @brief �A���I�[�_�[�h�A�N�Z�X�r���[��ݒ�
RootSignature::Desc& RootSignature::Desc::SetUAV(ShaderStage stage, int index, uint32_t reg)
{
	SetParam(stage, index, reg, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
	return *this;
}

//! @brief �T���v���[�X�e�[�g��ݒ�
RootSignature::Desc& RootSignature::Desc::SetSmp(ShaderStage stage, int index, uint32_t reg)
{
	SetParam(stage, index, reg, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER);
	return *this;
}

//! @brief �X�^�e�B�b�N�T���v���[��ǉ�
RootSignature::Desc& RootSignature::Desc::AddStaticSmp(ShaderStage stage, uint32_t reg, SamplerState state)
{
	// �X�^�e�B�b�N�T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC sampler = {};

	sampler.MipLODBias		 = D3D12_DEFAULT_MIP_LOD_BIAS;
	sampler.MaxAnisotropy	 = 1;
	sampler.ComparisonFunc	 = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor		 = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD			 = 0.0f;
	sampler.MaxLOD			 = +D3D12_FLOAT32_MAX;
	sampler.ShaderRegister	 = reg;
	sampler.RegisterSpace	 = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY(stage);

	switch (state)
	{
	case SamplerState::PointWrap:
		{
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		}
		break;

	case SamplerState::PointClamp:
		{
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}
		break;

	case SamplerState::LinearWrap:
		{
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		}
		break;

	case SamplerState::LinearClamp:
		{
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}
		break;

	case SamplerState::AnisotropicWrap:
		{
			sampler.Filter = D3D12_FILTER_ANISOTROPIC;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			sampler.MaxAnisotropy = D3D12_MAX_MAXANISOTROPY;
		}
		break;

	case SamplerState::AnisotropicClamp:
		{
			sampler.Filter = D3D12_FILTER_ANISOTROPIC;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			sampler.MaxAnisotropy = D3D12_MAX_MAXANISOTROPY;
		}
		break;
	}

	m_Samplers.push_back(sampler);
	return *this;
}

//! @brief ���̓��C�A�E�g������
RootSignature::Desc& RootSignature::Desc::AllowIL()
{
	m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	return *this;
}

//! @brief �X�g���[���A�E�g�v�b�g������
RootSignature::Desc& RootSignature::Desc::AllowSO()
{
	m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
	return *this;
}

//! @brief ���[�g�V�O�l�`���̐ݒ���I�����C�ݒ����Ԃ�
//! @param[in] Desc Class�̃C���X�^���X
RootSignature::Desc& RootSignature::Desc::End()
{
	if (m_DenyStage[0])
		m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
	if (m_DenyStage[1])
		m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	if (m_DenyStage[2])
		m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	if (m_DenyStage[3])
		m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	if (m_DenyStage[4])
		m_Flags |= D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	// ���[�g�V�O�l�`���̐ݒ�
	m_Desc.NumParameters		= static_cast<UINT>(m_Params.size());
	m_Desc.pParameters			= m_Params.data();
	m_Desc.NumStaticSamplers	= static_cast<UINT>(m_Samplers.size());
	m_Desc.pStaticSamplers		= m_Samplers.data();
	m_Desc.Flags				= D3D12_ROOT_SIGNATURE_FLAGS(m_Flags);

	return *this;
}

//! @brief �w�肵���V�F�[�_�[�X�e�[�W�̋��ېݒ������
//! @param[in] stage �V�F�[�_�[�^�C�v
void RootSignature::Desc::CheckStage(ShaderStage stage)
{
	auto index = static_cast<int>(stage - 1);
	if (0 <= index && index < 5)
		m_DenyStage[index] = false;
}

//! @brief �f�B�X�N���v�^�����W�̐ݒ�
//! @param[in] stage �V�F�[�_�[�^�C�v
//! @param[in] index �Z�b�g����f�B�X�N���v�^�����W�̔ԍ�
//! @param[in] reg   ���W�X�^�[
//! @param[in] type  �r���[�̃^�C�v
void RootSignature::Desc::SetParam(ShaderStage stage, int index, uint32_t reg, D3D12_DESCRIPTOR_RANGE_TYPE type)
{
	if (index >= m_Params.size())
		return;

	// �f�B�X�N���v�^�����W��ݒ�.
	m_Ranges[index].RangeType						  = type;
	m_Ranges[index].NumDescriptors					  = 1;
	m_Ranges[index].BaseShaderRegister				  = reg;
	m_Ranges[index].RegisterSpace					  = 0;
	m_Ranges[index].OffsetInDescriptorsFromTableStart = 0;

	// ���[�g�p�����[�^�̐ݒ�
	m_Params[index].ParameterType						= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	m_Params[index].DescriptorTable.NumDescriptorRanges = 1;
	m_Params[index].DescriptorTable.pDescriptorRanges	= &m_Ranges[index];
	m_Params[index].ShaderVisibility					= D3D12_SHADER_VISIBILITY(stage);

	// �w�肷��V�F�[�_�[�̋��ېݒ������
	CheckStage(stage);
}
