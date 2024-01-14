#include "PipelineState.h"

//! @brief �p�C�v���C���X�e�[�g�̍쐬
//! @param[in] pDevice  �f�o�C�X
//! @param[in] desc		�p�C�v���C���X�e�[�g�̐ݒ�
bool PipelineState::Init(ID3D12Device* pDevice, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc)
{
	auto hr = pDevice->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
	if (FAILED(hr))
		return false;

	return true;
}

//! @brief �p�C�v���C���X�e�[�g�̍쐬(�R���s���[�g�p)
//! @param[in] pDevice  �f�o�C�X
//! @param[in] desc		�p�C�v���C���X�e�[�g�̐ݒ�
bool PipelineState::Init(ID3D12Device* pDevice, D3D12_COMPUTE_PIPELINE_STATE_DESC* desc)
{
	auto hr = pDevice->CreateComputePipelineState(desc, IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
	if (FAILED(hr))
		return false;

	return true;
}
