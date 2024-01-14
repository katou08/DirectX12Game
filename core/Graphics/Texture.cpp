#include "Graphics/Texture.h"

//! @brief �e�N�X�`���̍쐬
//! @param[in] pDevice	�f�o�C�X
//! @param[in] filename �t�@�C����(DDS�̂�)
//! @param[in] isSRGB	SRGB�ɂ��邩
//! @param[in] batch    �o�b�`
//! @param[in] isCube   Cube�ɂ��邩
//! @return �e�N�X�`���̍쐬�ɐ���������
bool Texture::Init(ID3D12Device* pDevice,
    const wchar_t* filename, bool isSRGB, DirectX::ResourceUploadBatch& batch, bool isCube)
{
    // �����`�F�b�N
    if (pDevice == nullptr || filename == nullptr)
        return false;

    // ���\�[�X�𐶐�
    auto hr = DirectX::CreateDDSTextureFromFile(
        pDevice,
        batch,
        filename,
        m_pBuffer.GetAddressOf(),
        true);
    ThrowIfFailed(hr, "�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���");

    // �V�F�[�_�[���\�[�X�r���[�̐ݒ�����߂�
    m_Desc = GetViewDesc(isCube);

    return true;
}

bool Texture::Init(ID3D12Device* pDevice, const D3D12_RESOURCE_DESC* pDesc, bool isSRGB, bool isCube)
{
    // �����`�F�b�N
    if (pDevice == nullptr || pDesc == nullptr)
        return false;

    D3D12_RESOURCE_DESC desc = *pDesc;

    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_DEFAULT;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.CreationNodeMask = 0;
    prop.VisibleNodeMask = 0;

    auto hr = pDevice->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(m_pBuffer.GetAddressOf()));
    ThrowIfFailed(hr, "�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���");

    // �V�F�[�_�[���\�[�X�r���[�̐ݒ�����߂�
    m_Desc = GetViewDesc(isCube);

    return true;
}

//! @brief �e�N�X�`�����̎擾
//! @param[in] isCube   Cube�ɂ��邩
D3D12_SHADER_RESOURCE_VIEW_DESC Texture::GetViewDesc(bool isCube)
{
    auto desc = m_pBuffer->GetDesc();
    D3D12_SHADER_RESOURCE_VIEW_DESC viewDesc = {};

    viewDesc.Format = desc.Format;
    viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    switch (desc.Dimension)
    {
    case D3D12_RESOURCE_DIMENSION_BUFFER:
    {
        ThrowFailed("�o�b�t�@�͑ΏۊO");
    }
    break;

    case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
    {
        if (desc.DepthOrArraySize > 1)
        {
            viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;

            viewDesc.Texture1DArray.MostDetailedMip = 0;
            viewDesc.Texture1DArray.MipLevels = desc.MipLevels;
            viewDesc.Texture1DArray.FirstArraySlice = 0;
            viewDesc.Texture1DArray.ArraySize = desc.DepthOrArraySize;
            viewDesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
        }
        else
        {
            viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;

            viewDesc.Texture1D.MostDetailedMip = 0;
            viewDesc.Texture1D.MipLevels = desc.MipLevels;
            viewDesc.Texture1D.ResourceMinLODClamp = 0.0f;
        }
    }
    break;

    case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
    {
        if (isCube)
        {
            if (desc.DepthOrArraySize > 6)
            {
                viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;

                viewDesc.TextureCubeArray.MostDetailedMip = 0;
                viewDesc.TextureCubeArray.MipLevels = desc.MipLevels;
                viewDesc.TextureCubeArray.First2DArrayFace = 0;
                viewDesc.TextureCubeArray.NumCubes = (desc.DepthOrArraySize / 6);
                viewDesc.TextureCubeArray.ResourceMinLODClamp = 0.0f;
            }
            else
            {
                viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;

                viewDesc.TextureCube.MostDetailedMip = 0;
                viewDesc.TextureCube.MipLevels = desc.MipLevels;
                viewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
            }
        }
        else
        {
            if (desc.DepthOrArraySize > 1)
            {
                if (desc.MipLevels > 1)
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;

                    viewDesc.Texture2DMSArray.FirstArraySlice = 0;
                    viewDesc.Texture2DMSArray.ArraySize = desc.DepthOrArraySize;
                }
                else
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;

                    viewDesc.Texture2DArray.MostDetailedMip = 0;
                    viewDesc.Texture2DArray.MipLevels = desc.MipLevels;
                    viewDesc.Texture2DArray.FirstArraySlice = 0;
                    viewDesc.Texture2DArray.ArraySize = desc.DepthOrArraySize;
                    viewDesc.Texture2DArray.PlaneSlice = 0;
                    viewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
                }
            }
            else
            {
                if (desc.MipLevels > 1)
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                }
                else
                {
                    viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

                    viewDesc.Texture2D.MostDetailedMip = 0;
                    viewDesc.Texture2D.MipLevels = desc.MipLevels;
                    viewDesc.Texture2D.PlaneSlice = 0;
                    viewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
                }
            }
        }
    }
    break;

    case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
    {
        viewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;

        viewDesc.Texture3D.MostDetailedMip = 0;
        viewDesc.Texture3D.MipLevels = desc.MipLevels;
        viewDesc.Texture3D.ResourceMinLODClamp = 0.0f;
    }
    break;

    default:
    {
        // �z��O
        ThrowFailed("�z��O�̃e�N�X�`���ł�.");
    }
    break;
    }

    return viewDesc;
}
