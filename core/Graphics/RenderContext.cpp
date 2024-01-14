#include "Graphics/RenderContext.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

//! @brief コンストラクタ
//! @param[in] game  ゲームクラス
RenderContext::RenderContext()
{
}

//! @brief デストラクタ
RenderContext::~RenderContext()
{
	Term();
}

//! @brief 初期化関数
//! @param[in] pDevice  デバイス
//! @param[in] type		コマンドリストのタイプ
//! @param[in] count	コマンドアロケータの数
bool RenderContext::Initialize(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE type, uint32_t count)
{
	if (pDevice == nullptr || count == 0)
		return false;

	m_pCommandAllocators.resize(count); // アロケータの数分リサイズ
	// コマンドアロケータの生成(コマンドリストが使用するメモリの割り当て用)
	for (auto i = 0u; i < count; ++i)
	{
		auto hr = pDevice->CreateCommandAllocator(
			type, IID_PPV_ARGS(m_pCommandAllocators[i].GetAddressOf()));
		ThrowIfFailed(hr, "CommandAllocatorの作成に失敗しました.");
	}

	// コマンドリストの生成
	auto hr = pDevice->CreateCommandList(
		0,
		type,
		m_pCommandAllocators[0].Get(), //! バックバッファに対応するアロケータ
		nullptr,
		IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
	ThrowIfFailed(hr, "CommandListの作成に失敗しました.");

	m_pCommandList->Close();

	m_Index = 0;

	return true;
}

//! @brief コマンドリスト，アロケータ終了処理
void RenderContext::Term()
{
	m_pCommandList.Reset();

	for (size_t i = 0; i < m_pCommandAllocators.size(); ++i)
	{
		m_pCommandAllocators[i].Reset();
	}

	m_pCommandAllocators.clear();
	m_pCommandAllocators.shrink_to_fit();
}

//! @brief コマンドリストを閉じる
void RenderContext::Close()
{
	m_pCommandList->Close();
}

//! @brief  リセット処理を行ったコマンドリストを取得
//! @return リセット処理を行ったコマンドリストを返す
ID3D12GraphicsCommandList* RenderContext::Reset()
{
	auto hr = m_pCommandAllocators[m_Index]->Reset();
	if (FAILED(hr))
		return nullptr;

	hr = m_pCommandList->Reset(m_pCommandAllocators[m_Index].Get(), nullptr);
	if (FAILED(hr))
		return nullptr;

	m_Index = (m_Index + 1) % static_cast<uint32_t>(m_pCommandAllocators.size());

	return m_pCommandList.Get();
}

//! @brief シザー矩形の設定とビューポートのセット
//! @param[in] viewport ビューポート
void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
	//シザリング矩形も設定する。
	D3D12_RECT scissorRect;
	scissorRect.bottom = static_cast<LONG>(viewport.Height);
	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = static_cast<LONG>(viewport.Width);
	SetScissorRect(scissorRect);

	m_pCommandList->RSSetViewports(1, &viewport);
	m_currentViewport = viewport;
}

//! @brief 描画処理
//! @param[in] viewport ビューポート
//! @param[in] indexCount インデックス数
void RenderContext::DrawIndexed(UINT indexCount)
{
	m_pCommandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}

//! @brief 描画処理 (インスタンシング描画)
//! //! @param[in] indexCount インデックス数
//! //! @param[in] instanceCount インスタンス数
void RenderContext::DrawIndexedInstanced(UINT indexCount, UINT instanceCount)
{
	m_pCommandList->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

//! @brief 描画処理 (頂点描画)
//! @param[in] verticesCount 頂点数
void RenderContext::DrawVertices(UINT verticesCount)
{
	m_pCommandList->DrawInstanced(verticesCount, 1, 0, 0);
}

//! @brief シザー矩形のセット
//! @param[in] rect Rect
void RenderContext::SetScissorRect(D3D12_RECT& rect)
{
	m_pCommandList->RSSetScissorRects(1, &rect);
}

//! @brief ルートシグネチャのセット
//! @param[in] rootsig ルートシグネチャ
void RenderContext::SetRootSignature(ID3D12RootSignature* rootsig)
{
	m_pCommandList->SetGraphicsRootSignature(rootsig);
}

//! @brief パイプラインステートのセット
//! @param[in] pso パイプラインステート
void RenderContext::SetPipelineState(ID3D12PipelineState* pso)
{
	m_pCommandList->SetPipelineState(pso);
}

//! @brief プリミティブトポロジーのセット
//! @param[in] topology トポロジー
void RenderContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	m_pCommandList->IASetPrimitiveTopology(topology);
}

//! @brief リソースバリア
//! @param[in] commandList	コマンドリスト
//! @param[in] pResource	リソース
//! @param[in] before		前のリソースの状態
//! @param[in] after		次のリソースの状態
void RenderContext::SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	m_pCommandList->ResourceBarrier(1, &barrier);
}

//! @brief 頂点バッファのセット
//! @param[in] vb	頂点バッファ
void RenderContext::SetVertexBuffer(VertexBuffer& vb)
{
	auto VBV = vb.GetView();
	m_pCommandList->IASetVertexBuffers(0, 1, &VBV);
}

//! @brief インデックスバッファのセット
//! @param[in] ib	インデックスバッファ
void RenderContext::SetIndexBuffer(IndexBuffer& ib)
{
	auto IBV = ib.GetView();
	m_pCommandList->IASetIndexBuffer(&IBV);
}

//! @brief ディスクリプタヒープのセット
//! @param[in] pHeap	ディスクリプタヒープ
void RenderContext::SetDescriptorHeap(ID3D12DescriptorHeap* pHeap)
{
	ID3D12DescriptorHeap* pDescHeaps[] = { pHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(pDescHeaps), pDescHeaps);
}

//! @brief ディスクリプタテーブルのセット
//! @param[in] RootParameterIndex ルートパラメタの番号
//! @param[in] BaseDescriptor ディスクリプタのベースGPUハンドル
void RenderContext::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
{
	m_pCommandList->SetGraphicsRootDescriptorTable(
		RootParameterIndex,
		BaseDescriptor
	);
}

//! @brief レンダーターゲットのセット
//! @param[in] rtvHandle RTVのCPUハンドル
//! @param[in] dsvHandle DSVのCPUハンドル
void RenderContext::SetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
{
	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
}

//! @brief レンダーターゲットビューのクリア
//! @param[in] rtvHandle RTVのCPUハンドル
//! @param[in] clearColor クリア値
void RenderContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
{
	m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

//! @brief 深度バッファビューのクリア
//! @param[in] dsvHandle DSVのCPUハンドル
//! @param[in] clearColor クリア値
void RenderContext::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
{
	m_pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		clearValue, 0, 0, nullptr);
}
