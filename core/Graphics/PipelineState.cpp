#include "PipelineState.h"

//! @brief パイプラインステートの作成
//! @param[in] pDevice  デバイス
//! @param[in] desc		パイプラインステートの設定
bool PipelineState::Init(ID3D12Device* pDevice, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc)
{
	auto hr = pDevice->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
	if (FAILED(hr))
		return false;

	return true;
}

//! @brief パイプラインステートの作成(コンピュート用)
//! @param[in] pDevice  デバイス
//! @param[in] desc		パイプラインステートの設定
bool PipelineState::Init(ID3D12Device* pDevice, D3D12_COMPUTE_PIPELINE_STATE_DESC* desc)
{
	auto hr = pDevice->CreateComputePipelineState(desc, IID_PPV_ARGS(m_pipelineState.GetAddressOf()));
	if (FAILED(hr))
		return false;

	return true;
}
