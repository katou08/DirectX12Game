#include "Graphics/SwapChain.h"
#include "Graphics/RenderSystem.h"

//! @brief コンストラクタ
SwapChain::SwapChain()
{
}

//! @brief デストラクタ
SwapChain::~SwapChain()
{
}

//! @brief スワップチェーンの作成
//! @param[in] pDevice		デバイス
//! @param[in] width		横幅
//! @param[in] height		縦幅
//! @param[in] m_renderer   レンダーシステム
//! @return スワップチェーンの作成に成功したか
bool SwapChain::Initialize(HWND hwnd, UINT width, UINT height, RenderSystem* m_renderer)
{
	m_pRenderSystem = m_renderer;

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width					= (width > 1) ? width : 1;
	desc.BufferDesc.Height					= (height > 1) ? height : 1;
	desc.BufferDesc.RefreshRate.Numerator	= 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //< 走査線描画モードの設定
	desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count					= 1; //! ピクセル単位のマルチサンプル数
	desc.SampleDesc.Quality					= 0;
	desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount						= m_renderer->frameCount;
	desc.OutputWindow						= hwnd;
	desc.Windowed							= TRUE;
	desc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD; //! Presentの後バックバッファは破棄
	desc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //! 表示モード切り替え可能

	// スワップチェインの生成.
	ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	auto hr = m_renderer->m_pFactory->CreateSwapChain(m_renderer->m_pCommandQueue.Get(), &desc, pSwapChain.GetAddressOf());
	ThrowIfFailed(hr, "スワップチェインの生成に失敗しました.");

	// IDXGISwapChain4を取得
	hr = pSwapChain.As(&m_pSwapChain);
	ThrowIfFailed(hr, "スワップチェインの生成に失敗しました.");

	// 不要になったので解放.
	pSwapChain.Reset();

	return true;
}

//! @brief プレゼント
//! @param[in] vsync 垂直同期するかどうか
void SwapChain::Present(UINT vsync)
{
	m_pSwapChain->Present(vsync, 0);
}

//! @brief リサイズ
//! @param[in] width  横幅
//! @param[in] height 縦幅
void SwapChain::Resize(UINT width, UINT height)
{
	m_bChangeScreenSize = true;
	// バックバッファを解放
	m_pRenderSystem->ReleaseBackBuffers();
	// スワップチェーンの持つバッファ分サイズ変更
	m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	// サイズの変更に伴い，RTV,DSVの作り直し
	m_pRenderSystem->CreateRenderTargetView();
	m_pRenderSystem->CreateDepthStencilView();
}

//! @brief 現在のバックバッファの番号を取得
UINT SwapChain::GetCurrentBackBufferIndex()
{
	return m_pSwapChain->GetCurrentBackBufferIndex();
}

//! @brief バックバッファの取得
//! @param[in] i		バックバッファの番号
//! @param[in] pBuffer	バッファ情報
HRESULT SwapChain::GetBuffer(UINT i, ID3D12Resource* pBuffer)
{
	auto hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBuffer));

	return hr;
}
//! @brief バックバッファの設定情報の取得
//! @param[in] pDesc	デスク
HRESULT SwapChain::GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc)
{
	auto hr = m_pSwapChain->GetDesc(pDesc);
	return hr;
}
