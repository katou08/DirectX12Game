#pragma once
#include "pch.h"

#define STRINGFY(s)  #s
#define TO_STRING(x) STRINGFY(x)
#define FILE_PREFIX __FILE__ "(" TO_STRING(__LINE__) "): " 
#define ThrowIfFailed(hr, msg) Utility::CheckResultCodeD3D12(hr, FILE_PREFIX msg)
#define ThrowFailed(msg) Utility::CheckCodeD3D12(msg FILE_PREFIX)


namespace Utility
{
    inline std::wstring StringToWString(const std::string& input)
    {
        size_t i;
        wchar_t* buffer = new wchar_t[input.size() + 1];
        mbstowcs_s(&i, buffer, input.size() + 1, input.c_str(), _TRUNCATE);
        std::wstring result = buffer;
        delete[] buffer;
        return result;
    }

    inline std::wstring aiStringToWString(const aiString& input)
    {
        wchar_t temp[256] = {};
        size_t size;
        mbstowcs_s(&size, temp, input.C_Str(), 256);
        return std::wstring(temp);
    }

    inline std::string WStringToString(const std::wstring& input)
    {
        size_t i;
        char* buffer = new char[input.size() * MB_CUR_MAX + 1];
        wcstombs_s(&i, buffer, input.size() * MB_CUR_MAX + 1, input.c_str(), _TRUNCATE);
        std::string result = buffer;
        delete[] buffer;
        return result;
    }

    class DX12Exception : public std::runtime_error
    {
    public:
        DX12Exception(const std::string& msg) : std::runtime_error(msg.c_str()) {}
    };

    inline void CheckResultCodeD3D12(HRESULT hr, const std::string errorMsg)
    {
        if (FAILED(hr))
            throw DX12Exception(errorMsg);
    }

    inline void CheckCodeD3D12(const std::string errorMsg)
    {
        throw DX12Exception(errorMsg);
    }
}
