#include "Graphics/VertexBuffer.h"

//! @brief コンストラクタ
VertexBuffer::VertexBuffer()
	:m_pVB(nullptr)
{
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief デストラクタ
VertexBuffer::~VertexBuffer()
{
	m_pVB.Reset();
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief 頂点バッファの作成
//! @param[in] pDevice		デバイス
//! @param[in] size			バッファの大きさ
//! @param[in] stride		ストライド
//! @param[in] pInitData	マッピングするバッファのポインタ
//! @return 頂点バッファの作成に成功したか
bool VertexBuffer::Init(ID3D12Device* pDevice, size_t size, size_t stride, const void* pInitData)
{
	// 引数チェック.
	if (pDevice == nullptr || size == 0 || stride == 0)
		return false;

	// ヒーププロパティの作成
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_HEAP_TYPE_UPLOAD; // GPUに転送するためのヒープ
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1; //! 単一のGPUを前提のため1
	prop.VisibleNodeMask		= 1;

	// リソースの設定
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER; // バッファとして使用
	resDesc.Alignment			= 0;
	resDesc.Width				= UINT64(size);
	resDesc.Height				= 1;
	resDesc.DepthOrArraySize	= 1;
	resDesc.MipLevels			= 1;
	resDesc.Format				= DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count	= 1;
	resDesc.SampleDesc.Quality	= 0;
	resDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR; //! 行優先で保存されたデータ
	resDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// リソースの作成
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pVB.GetAddressOf()));
	if (FAILED(hr))
		return false;

	// マッピング
	if (pInitData != nullptr)
	{
		// マッピング(頂点データ内容をリソースに書き込む)
		void* ptr = Map();
		if (ptr == nullptr)
			return false;

		// 頂点データをマッピング先に設定
		memcpy(ptr, pInitData, size);

		m_pVB->Unmap(0, nullptr); //!< マッピング解除

	}

	// 頂点バッファビューの設定
	m_View.BufferLocation = m_pVB->GetGPUVirtualAddress();
	m_View.SizeInBytes = static_cast<UINT>(size);
	m_View.StrideInBytes = static_cast<UINT>(stride);

	return true;
}

void* VertexBuffer::Map() const
{
	void* ptr;
	auto hr = m_pVB->Map(0, nullptr, &ptr);
	if (FAILED(hr))
		return nullptr;

	return ptr;
}

void VertexBuffer::Unmap()
{
	m_pVB->Unmap(0, nullptr);
}
