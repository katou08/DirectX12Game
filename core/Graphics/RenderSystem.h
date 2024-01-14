#pragma once
#include "pch.h"
#include "Graphics/ResourceHeap.h"
#include "Graphics/ResourceView.h"
#include "DepthBuffer.h"
#include "Graphics/ConstantBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "Resource/ResMesh.h"
#include "Graphics/Fence.h"

// 前方宣言
class Game;
class RenderContext;
class SwapChain;
class DepthBuffer;

class RenderSystem
{
public:
	RenderSystem(Game* game);
	~RenderSystem();
	bool Initialize();
	void BeginRender();
	void Render();
	void EndRender();
	void WaitRender();

	ID3D12Device* GetDevice() const { return m_pDevice.Get(); }
	ID3D12CommandQueue* GetCmdQueue() const { return m_pCommandQueue.Get(); }
	RenderContext* GetRenderContext() const { return m_pRenderContext.get(); }
	uint32_t GetFrameCount() const { return frameCount; }
	uint32_t GetFrameIndex() const { return m_FrameIndex; }
	const DXGI_FORMAT& GetBackBufferFormat() const;
	const DXGI_FORMAT& GetDepthBufferFormat() const;
	const D3D12_VIEWPORT& GetViewport() const { return m_Viewport; }
	void ReleaseBackBuffers();

private:
	// ゲームクラス
	Game* m_pGame = nullptr;

	// DX12関連
	ComPtr<ID3D12Device>			m_pDevice		 = nullptr;
	ComPtr<IDXGIFactory6>			m_pFactory		 = nullptr;
	ComPtr<ID3D12CommandQueue>		m_pCommandQueue	 = nullptr;
	std::shared_ptr<RenderContext>  m_pRenderContext = nullptr;
	uint32_t frameCount   = 2;
	uint32_t m_FrameIndex = 0; // バックバッファのインデックス

	// ディスクリプタヒープ
	ResourceHeap m_heapRTV = {};
	ResourceHeap m_heapDSV = {};

	// リソース，ビュー関連
	std::vector<RenderTargetView> m_RTVs;
	std::vector<DepthStencilView> m_DSVs;

	// バッファ関連
	std::vector<ComPtr<ID3D12Resource>> m_pBackBuffers = { nullptr };
	DepthBuffer mDepthBuffer = {};

	D3D12_VIEWPORT m_Viewport = {}; //!< ビューポート
	D3D12_RECT m_ScissorRect = {}; //!< シザー矩形
	std::unique_ptr<Fence> m_pFence = nullptr;


	bool CreateFactory();
	bool CreateDevice();
	bool CreateRenderContext();
	bool CreateSwapChain();
	bool CreateDescriptorHeaps();
	bool CreateRenderTargetView();
	bool CreateDepthStencilView();
	void Present(uint32_t interval);
private:
	friend class SwapChain;
};