#include "Graphics/Fence.h"

//! @brief コンストラクタ
//! @param[in] pDevice  デバイス
Fence::Fence(ID3D12Device* pDevice)
	:m_pFence(nullptr)
	,m_Event(nullptr)
	,m_Counter(0)
{
	if(pDevice == nullptr)
		ThrowFailed("フェンスの作成に失敗しました.");

	// イベントの作成
	m_Event = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if(m_Event == nullptr)
		ThrowFailed("フェンスの作成に失敗しました.");

	// フェンスの作成
	auto hr = pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(m_pFence.GetAddressOf()));
	ThrowIfFailed(hr, "フェンスの作成に失敗しました.");

	// カウンター設定
	m_Counter = 1;
}

Fence::~Fence()
{
	// ハンドルを閉じる.
	if (m_Event != nullptr)
	{
		CloseHandle(m_Event);
		m_Event = nullptr;
	}

	// フェンスオブジェクトを破棄.
	m_pFence.Reset();

	// カウンターリセット.
	m_Counter = 0;
}

//! @brief シグナル状態になるまで指定された時間待機する
//! @param[in] pQueue  コマンドキュー
//! @param[in] timeout タイムアウト時間[ms]
void Fence::Wait(ID3D12CommandQueue* pQueue, uint32_t timeout)
{
	if (pQueue == nullptr)
		return;

	const auto fenceValue = m_Counter; //! 現在のフレームのフェンスカウンターを取得

	// シグナル処理
	auto hr = pQueue->Signal(m_pFence.Get(), fenceValue); //! フェンスに値をセット
	if (FAILED(hr))
		return;

	// カウンターを増やす
	m_Counter++;

	// 次のフレーム描画処理がまだであれば待機
	if (m_pFence->GetCompletedValue() < fenceValue)
	{
		// 完了時にイベントを設定
		auto hr = m_pFence->SetEventOnCompletion(fenceValue, m_Event);
		if (FAILED(hr))
			return;

		// 待機処理
		if (WAIT_OBJECT_0 != WaitForSingleObjectEx(m_Event, timeout, FALSE))
			return;
	}
}

//! @brief シグナル状態になるまでずっと待機する
//! @param[in] pQueue  コマンドキュー
//! @param[in] timeout タイムアウト時間[ms]
void Fence::Sync(ID3D12CommandQueue* pQueue)
{
	if (pQueue == nullptr)
		return;

	// シグナル処理
	auto hr = pQueue->Signal(m_pFence.Get(), m_Counter);
	if (FAILED(hr))
		return;

	// 完了時にイベントを設定
	hr = m_pFence->SetEventOnCompletion(m_Counter, m_Event);
	if (FAILED(hr))
		return;

	// 待機処理
	if (WAIT_OBJECT_0 != WaitForSingleObjectEx(m_Event, INFINITE, FALSE))
		return;

	// カウンターを増やす
	m_Counter++;
}
