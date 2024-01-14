#include "FileUtil.h"

namespace
{
	//!@ brief �������u��
	//!@ param[in] input�@	�u������p�X
	//!@ param[in] pattern�@�u���������镶����
	//!@ param[in] result�@	�u�������镶����
	//!@ return[in]			�u�������p�X��Ԃ�
	std::string Replace
	(
		const std::string& input,
		std::string pattern,
		std::string replace
	)
	{
		std::string result = input;
		auto pos = result.find(pattern);

		while (pos != std::string::npos)
		{
			result.replace(pos, pattern.length(), replace);
			pos = result.find(pattern, pos + replace.length());
		}

		return result;
	}

	//!@ brief �������u��(���C�h������)
	std::wstring Replace
	(
		const std::wstring& input,
		std::wstring         pattern,
		std::wstring         replace)
	{
		std::wstring result = input;
		auto pos = result.find(pattern);

		while (pos != std::wstring::npos)
		{
			result.replace(pos, pattern.length(), replace);
			pos = result.find(pattern, pos + replace.length());
		}

		return result;
	}
}

/*!
@brief �t�@�C���p�X�̌���(���C�h������)
@param filename �t�@�C����
@param result �t�@�C���p�X
@return �t�@�C���p�X�����݂��邩
*/
bool SearchFilePathW(const wchar_t* filename, std::wstring& result)
{
	if (filename == nullptr)
		return false;

	if (wcscmp(filename, L" ") == 0 || wcscmp(filename, L"") == 0)
		return false;

	wchar_t exePath[520] = {};
	GetModuleFileName(nullptr, exePath, 520); //! ���s�t�@�C���̂���p�X��exePath�ɑ��
	exePath[519] = L'\0'; //! null�I�[��
	PathRemoveFileSpec(exePath); //! ���s�t�@�C���������폜

	wchar_t dstPath[520] = {};

	wcscpy_s(dstPath, filename); //! filename�̓��e��dstPath�ɃR�s�[

	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"..\\..\\res\\%s", filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}
	swprintf_s(dstPath, L"..\\..\\%s", filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"..\\..\\..\\%s", filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"..\\shader\\cso\\%s", filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"..\\..\\..\\res\\%s", filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"\\res\\%s", filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"%s\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"%s\\..\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"%s\\..\\..\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"%s\\res\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	swprintf_s(dstPath, L"%s\\..\\..\\res\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}
	swprintf_s(dstPath, L"%s\\..\\..\\res\\teapot\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}
	swprintf_s(dstPath, L"%s\\..\\..\\res\\matball\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}
	swprintf_s(dstPath, L"%s\\..\\..\\res\\cube\\%s", exePath, filename);
	if (PathFileExistsW(dstPath) == TRUE)
	{
		result = Replace(dstPath, L"\\", L"/");
		return true;
	}

	return false;
}

/*!
@brief �t�@�C���p�X�̌���
@param filename �t�@�C����
@param result �t�@�C���p�X
@return �t�@�C���p�X�����݂��邩
*/
bool SearchFilePathA(const char* filename, std::string& result)
{
	if (filename == nullptr)
		return false;

	if (strcmp(filename, " ") == 0 || strcmp(filename, "") == 0)
		return false;

	char exePath[520] = {0};
	memset(exePath, '\0', sizeof(exePath));
	GetModuleFileNameA(nullptr, exePath, 520); //! ���s�t�@�C���̂���p�X��exePath�ɑ��
	PathRemoveFileSpecA(exePath); //! ���s�t�@�C���������폜

	char dstPath[520] = {0};
	memset(dstPath, '\0', sizeof(dstPath));

	strcpy_s(dstPath, filename); //! filename�̓��e��dstPath�ɃR�s�[
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "..\\..\\%s", filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "..\\..\\..\\%s", filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "..\\shader\\cso\\%s", filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "..\\..\\res\\%s", filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "..\\..\\..\\res\\%s", filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "\\res\\%s", filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "%s\\%s", exePath, filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "%s\\..\\%s", exePath, filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "%s\\..\\..\\%s", exePath, filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	sprintf_s(dstPath, "%s\\res\\%s", exePath, filename);
	if (PathFileExistsA(dstPath) == TRUE)
	{
		result = Replace(dstPath, "\\", "/");
		return true;
	}

	return false;
}

/*!
@brief �f�B���N�g���p�X���폜���C�t�@�C������ԋp
@param path �f�B�X���N�g���p�X����菜���t�@�C���p�X
@return �t�@�C������ԋp
*/
std::string RemoveDirectoryPathA(const std::string& path)
{
	auto temp = Replace(path, "\\", "/");
	auto pos = temp.rfind('/');

	if (pos != std::string::npos)
		return temp.substr(pos + 1);

	return path;
}

/*!
@brief �f�B���N�g���p�X���폜���C�t�@�C������ԋp(���C�h������)
@param path �f�B�X���N�g���p�X����菜���t�@�C���p�X
@return �t�@�C������ԋp
*/
std::wstring RemoveDirectoryPathW(const std::wstring& path)
{
	auto temp = Replace(path, L"\\", L"/");
	auto pos = temp.rfind(L'/');

	if (pos != std::wstring::npos)
	{
		return temp.substr(pos + 1);
	}

	return path;
}

/*!
@brief �f�B���N�g�������擾
@param filePath �t�@�C���p�X
@return  �w�肳�ꂽ�t�@�C���p�X����f�B���N�g�����𔲂��o��
*/
std::string GetDirectoryPathA(const char* filePath)
{
	std::string path = Replace(filePath, "\\", "/");
	size_t idx = path.find_last_of("/");
	if (idx != std::string::npos)
		return path.substr(0, idx + 1);

	return std::string();
}

/*!
@brief �f�B���N�g�������擾(���C�h������)
@param filePath �t�@�C���p�X
@return  �w�肳�ꂽ�t�@�C���p�X����f�B���N�g�����𔲂��o��
*/
std::wstring GetDirectoryPathW(const wchar_t* filePath)
{
	std::wstring path = Replace(filePath, L"\\", L"/");
	size_t idx = path.find_last_of(L"/");
	if (idx != std::wstring::npos)
		return path.substr(0, idx + 1);

	return std::wstring();
}

