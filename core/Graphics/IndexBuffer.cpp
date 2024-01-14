#include "Graphics/IndexBuffer.h"

//! @brief コンストラクタ
IndexBuffer::IndexBuffer()
	:m_pIB(nullptr)
	,m_Count(0)
{
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief デストラクタ
IndexBuffer::~IndexBuffer()
{
	m_pIB.Reset();
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief インデックスバッファの作成
//! @param[in] pDevice		デバイス
//! @param[in] count		バッファの大きさ
//! @param[in] pInitData	マッピングするバッファのポインタ
//! @return インデックスバッファの作成に成功したか
bool IndexBuffer::Init(ID3D12Device* pDevice, size_t count, const void* pInitData)
{
	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_HEAP_TYPE_UPLOAD; //! GPUに転送するためのヒープ
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1; //! 単一のGPUを前提のため1
	prop.VisibleNodeMask		= 1; //! 単一のGPUを前提のため1

	// リソースの設定
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER; //! バッファ
	desc.Alignment			= 0;
	desc.Width				= static_cast<UINT64>(count);
	desc.Height				= 1;
	desc.DepthOrArraySize	= 1;
	desc.MipLevels			= 1;
	desc.Format				= DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count	= 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR; //! 行優先で保存されたテクスチャデータ
	desc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// リソースの生成
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pIB.GetAddressOf()));
	if (FAILED(hr))
		return false;


	// 初期化データがあれば，書き込んでおく.
	if (pInitData != nullptr)
	{
		void* ptr = Map();
		if (ptr == nullptr)
			return false;

		memcpy(ptr, pInitData, count);

		m_pIB->Unmap(0, nullptr);
	}

	// インデックスバッファビューの設定
	m_View.BufferLocation = m_pIB->GetGPUVirtualAddress();
	m_View.Format = DXGI_FORMAT_R32_UINT;
	m_View.SizeInBytes = static_cast<UINT>(count);

	m_Count = count;

	return true;
}

//! @brief インデックスバッファのマッピング
void* IndexBuffer::Map() const
{
	void* ptr;
	auto hr = m_pIB->Map(0, nullptr, &ptr);
	if (FAILED(hr))
		return nullptr;

	return ptr;
}

//! @brief インデックスバッファのアンマップ
void IndexBuffer::Unmap()
{
	m_pIB->Unmap(0, nullptr);
}

