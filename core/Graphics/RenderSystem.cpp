#include "pch.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderContext.h"
#include "Game/Display.h"
#include "Graphics/SwapChain.h"
#include "Game/Game.h"

//! @brief コンストラクタ
//! @param[in] game  ゲームクラス
RenderSystem::RenderSystem(Game* game) : m_pGame(game)
{
}

//! @brief デストラクタ
RenderSystem::~RenderSystem()
{
}

//! @brief 初期化
bool RenderSystem::Initialize()
{
	// デバッグフラグの追加
#if defined(DEBUG) || defined(_DEBUG)
	ComPtr<ID3D12Debug> debug;
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));
	// デバッグレイヤーを有効化
	if (SUCCEEDED(hr))
		debug->EnableDebugLayer();
	else
		ThrowIfFailed(hr, "デバッグレイヤーの有効化に失敗しました.");
#endif

	if (!CreateFactory())
		return false;

	if (!CreateDevice())
		return false;

	if (!CreateRenderContext())
		return false;

	if (!CreateSwapChain())
		return false;

	if (!CreateDescriptorHeaps())
		return false;

	// RTVの作成
	if (!CreateRenderTargetView())
		return false;

	// DSVの作成
	if (!CreateDepthStencilView())
		return false;

	// フェンスの作成
	m_pFence = std::make_unique<Fence>(m_pDevice.Get());
	if (m_pFence == nullptr)
		return false;

	// ビューポートの設定
	auto winSize = m_pGame->GetDisplay()->getClientSize();
	m_Viewport.Width = static_cast<FLOAT>(winSize.width);
	m_Viewport.Height = static_cast<FLOAT>(winSize.height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	return true;
}

//! @param[in] 描画前処理
void RenderSystem::BeginRender()
{
	// 現在のフレームインデックスを取得
	m_FrameIndex = m_pGame->GetDisplay()->m_pSwapChain->GetCurrentBackBufferIndex();

	// コマンドリスト,コマンドアロケーターをリセット
	m_pRenderContext->Reset();

	// リソースバリアのステートをレンダーターゲットに変更
	m_pRenderContext->SetResourceBarrier(m_pBackBuffers[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// レンダーターゲットをセット
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_RTVs[m_FrameIndex].GetCPUDescHandle(); //!< RTVのCPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_DSVs[0].GetCPUDescHandle(); //!< DSVのCPUハンドル
	m_pRenderContext->SetRenderTargets(rtvH, dsvH);

	// セットしたレンダーターゲットの画面をクリア
	float clearColor[] = { 0.15f, 0.15f, 0.15f, 1.0f };
	m_pRenderContext->ClearRenderTargetView(rtvH, clearColor);

	// 深度ステンシルビューをクリア
	m_pRenderContext->ClearDepthStencilView(dsvH, 1.0f);

	// ビューポートの設定
	auto winSize = m_pGame->GetDisplay()->getClientSize();
	m_Viewport.Width = static_cast<FLOAT>(winSize.width);
	m_Viewport.Height = static_cast<FLOAT>(winSize.height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_pRenderContext->SetViewportAndScissor(m_Viewport);
}

void RenderSystem::Render()
{

}

//! @param[in] 描画後の処理
void RenderSystem::EndRender()
{
	// リソースバリアのステートをプレゼントに変更
	m_pRenderContext->SetResourceBarrier(m_pBackBuffers[m_FrameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//コマンドリストを閉じる
	m_pRenderContext->Close();

	// コマンドを実行
	auto pCmd = m_pRenderContext->GetCommandList();
	ID3D12CommandList* pCmdLists[] = { pCmd };
	m_pCommandQueue->ExecuteCommandLists(1, pCmdLists);

	// 画面表示
	Present(1);
}

void RenderSystem::WaitRender()
{
}

//! @param[in] バックバッファのフォーマットを取得
const DXGI_FORMAT& RenderSystem::GetBackBufferFormat() const
{
	auto swapChain = m_pGame->GetDisplay()->m_pSwapChain->GetSwapChain();
	if (swapChain == nullptr)
		ThrowFailed("スワップチェーンが作成されていません.");

	DXGI_SWAP_CHAIN_DESC desc;
	swapChain->GetDesc(&desc);

	return desc.BufferDesc.Format;
}

//! @param[in] 深度バッファのフォーマットを取得
const DXGI_FORMAT& RenderSystem::GetDepthBufferFormat() const
{
	return mDepthBuffer.GetResource()->GetDesc().Format;
}

//! @param[in] バックバッファの解放
void RenderSystem::ReleaseBackBuffers()
{
	for (int i = 0; i < frameCount; ++i)
	{
		if (m_pBackBuffers[i])
			m_pBackBuffers[i]->Release();
	}
}

//! @brief ファクトリーの生成関数
bool RenderSystem::CreateFactory()
{
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG; //!< DXGIDebug.dllがある場合読み込む
	auto hr = CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_pFactory.GetAddressOf()));
	ThrowIfFailed(hr, "Factoryの作成に失敗しました.");

	return true;
}

//! @brief デバイス生成関数
bool RenderSystem::CreateDevice()
{
	// フィーチャーレベルが合致するものを選択
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	// デバイスの生成
	for (auto lv : levels)
	{
		if (SUCCEEDED(D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(m_pDevice.GetAddressOf()))))
			break;
	}
	if (!m_pDevice)
	{
		ThrowFailed("Deviceの作成に失敗しました.");
		return false;
	}

	return true;
}

//! @brief レンダーコンテキスト生成関数
bool RenderSystem::CreateRenderContext()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT; //! GPUが実行可能なコマンドバッファを指定
	desc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask	= 0; //! GPUが一つなら0

	auto hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()));
	ThrowIfFailed(hr, "コマンドキューの作成に失敗しました.");

	m_pRenderContext = std::make_shared<RenderContext>();
	if(!m_pRenderContext->Initialize(m_pDevice.Get(), desc.Type, frameCount))
		ThrowFailed("レンダーコンテキストの作成に失敗しました.");

	return true;
}

//! @brief スワップチェーンの作成
bool RenderSystem::CreateSwapChain()
{
	auto display = m_pGame->GetDisplay();
	if (!display->CreateSwapChain())
	{
		ThrowFailed("スワップチェーンの作成に失敗しました");
		return false;
	}

	return true;
}

//! @brief 各リソースのディスクリプタヒープ生成関数
bool RenderSystem::CreateDescriptorHeaps()
{
	constexpr uint32_t numDescRTV = 100;
	constexpr uint32_t numDescDSV = 100;
	constexpr bool   bCPUVisible = false;
	
	// ヒープの作成
	m_heapRTV.Create(m_pDevice.Get(), "HeapRTV", D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numDescRTV);
	m_heapDSV.Create(m_pDevice.Get(), "HeapDSV", D3D12_DESCRIPTOR_HEAP_TYPE_DSV, numDescDSV);

	return true;
}

//! @brief RTVの作成
bool RenderSystem::CreateRenderTargetView()
{
	m_pBackBuffers.resize(frameCount);
	auto swapChain = m_pGame->GetDisplay()->m_pSwapChain->GetSwapChain();
	if (swapChain == nullptr)
		ThrowFailed("スワップチェーンが作成されていません.");

	for (uint32_t i = 0u; i < frameCount; ++i)
	{
		auto hr = swapChain->GetBuffer(i, IID_PPV_ARGS(m_pBackBuffers[i].GetAddressOf()));
		ThrowIfFailed(hr, "RTV用のバッファの作成に失敗しました.");

		if (m_pBackBuffers[i] == nullptr)
			ThrowFailed("バックバッファがありません");

		DXGI_SWAP_CHAIN_DESC desc;
		swapChain->GetDesc(&desc);

		DXGI_FORMAT format = desc.BufferDesc.Format;

		D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format				  = format;
		viewDesc.ViewDimension		  = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice   = 0;
		viewDesc.Texture2D.PlaneSlice = 0;

		if (m_pGame->GetDisplay()->m_pSwapChain->GetIsChangeScreenSize() == false)
		{
			RenderTargetView rtv = {};
			m_heapRTV.AllocDescriptor(1, &rtv);
			m_RTVs.push_back(rtv);
		}

		m_pDevice->CreateRenderTargetView(m_pBackBuffers[i].Get(), &viewDesc, m_RTVs[i].GetCPUDescHandle());
	}

	return true;
}

//! @brief DSVの作成
bool RenderSystem::CreateDepthStencilView()
{
	auto width = m_pGame->GetDisplay()->getClientSize().width;
	auto height = m_pGame->GetDisplay()->getClientSize().height;
	DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;

	if (mDepthBuffer.GetResource())
		mDepthBuffer.Release();

	if (!mDepthBuffer.Create(m_pDevice.Get(), width, height, format, 1.0f, 0))
		ThrowFailed("デプスバッファの作成に失敗しました.");

	// DSVの構成設定
	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.ViewDimension		 = D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Format				 = format;
	viewDesc.Texture2D.MipSlice  = 0;
	viewDesc.Flags				 = D3D12_DSV_FLAG_NONE;

	if (m_pGame->GetDisplay()->m_pSwapChain->GetIsChangeScreenSize() == false)
	{
		// DSV用のディスクリプタヒープに割り当てる
		DepthStencilView dsv = {};
		m_heapDSV.AllocDescriptor(1, &dsv);
		m_DSVs.push_back(dsv);
	}

	// DSVの生成
	m_pDevice->CreateDepthStencilView(mDepthBuffer.GetResource(), &viewDesc, m_DSVs[0].GetCPUDescHandle());
	return true;
}

//! @brief Present
//! //! @param[in] interval 垂直同期するかどうか
void RenderSystem::Present(uint32_t interval)
{
	// スワップチェーンにプレゼント
	m_pGame->GetDisplay()->m_pSwapChain->Present(interval);
	// 完了待ち
	m_pFence->Wait(m_pCommandQueue.Get(), INFINITE);
}
