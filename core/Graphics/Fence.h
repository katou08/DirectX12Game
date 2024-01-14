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
	ComPtr<ID3D12Fence> m_pFence;	//!< フェンス
	HANDLE				m_Event;	//!< イベント
	uint32_t			m_Counter;	//!< 現在のカウンター
};