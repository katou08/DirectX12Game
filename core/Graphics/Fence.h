#pragma once
#include "pch.h"

class Fence
{
public:
	Fence(ID3D12Device* pDevice);
	~Fence();
	void Wait(ID3D12CommandQueue* pQueue, uint32_t timeout);
	void Sync(ID3D12CommandQueue* pQueue);

private:
	ComPtr<ID3D12Fence> m_pFence;	//!< �t�F���X
	HANDLE				m_Event;	//!< �C�x���g
	uint32_t			m_Counter;	//!< ���݂̃J�E���^�[
};