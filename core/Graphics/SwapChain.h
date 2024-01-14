#pragma once
#include "pch.h"

class RenderSystem;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool Initialize(HWND hwnd, UINT width, UINT height, RenderSystem* m_renderer);
	void Present(UINT vsync);
	void Resize(UINT width, UINT height);
	const bool& GetIsChangeScreenSize() const { return m_bChangeScreenSize; }
	UINT GetCurrentBackBufferIndex();
	HRESULT GetBuffer(UINT i, ID3D12Resource* pBuffer);
	HRESULT GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc);
	IDXGISwapChain4* GetSwapChain() const { return m_pSwapChain.Get(); }

private:
	ComPtr<IDXGISwapChain4> m_pSwapChain = nullptr;
	bool m_bChangeScreenSize = false;
	RenderSystem* m_pRenderSystem = nullptr;
};
