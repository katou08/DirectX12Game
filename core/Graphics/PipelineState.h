#pragma once
#include "pch.h"

class PipelineState
{
public:
	PipelineState() {};

	bool Init(ID3D12Device* pDevice, D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc);
	bool Init(ID3D12Device* pDevice, D3D12_COMPUTE_PIPELINE_STATE_DESC* desc);
	ID3D12PipelineState* GetPtr() const
	{
		return m_pipelineState.Get();
	}

private:
	ComPtr<ID3D12PipelineState> m_pipelineState = nullptr;
};