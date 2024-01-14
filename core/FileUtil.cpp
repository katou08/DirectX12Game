#include "FileUtil.h"

namespace
{
	//!@ brief 文字列を置換
	//!@ param[in] input　	置換するパス
	//!@ param[in] pattern　置き換えられる文字列
	//!@ param[in] result　	置き換える文字列
	//!@ return[in]			置換したパスを返す
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

	//!@ brief 文字列を置換(ワイド文字列)
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
@brief ファイルパスの検索(ワイド文字列)
@param filename ファイル名
@param result ファイルパス
@return ファイルパスが存在するか
*/
bool SearchFilePathW(const wchar_t* filename, std::wstring& result)
{
	if (filename == nullptr)
		return false;

	if (wcscmp(filename, L" ") == 0 || wcscmp(filename, L"") == 0)
		return false;

	wchar_t exePath[520] = {};
	GetModuleFileName(nullptr, exePath, 520); //! 実行ファイルのあるパスをexePathに代入
	exePath[519] = L'\0'; //! null終端化
	PathRemoveFileSpec(exePath); //! 実行ファイル部分を削除

	wchar_t dstPath[520] = {};

	wcscpy_s(dstPath, filename); //! filenameの内容をdstPathにコピー

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
@brief ファイルパスの検索
@param filename ファイル名
@param result ファイルパス
@return ファイルパスが存在するか
*/
bool SearchFilePathA(const char* filename, std::string& result)
{
	if (filename == nullptr)
		return false;

	if (strcmp(filename, " ") == 0 || strcmp(filename, "") == 0)
		return false;

	char exePath[520] = {0};
	memset(exePath, '\0', sizeof(exePath));
	GetModuleFileNameA(nullptr, exePath, 520); //! 実行ファイルのあるパスをexePathに代入
	PathRemoveFileSpecA(exePath); //! 実行ファイル部分を削除

	char dstPath[520] = {0};
	memset(dstPath, '\0', sizeof(dstPath));

	strcpy_s(dstPath, filename); //! filenameの内容をdstPathにコピー
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
@brief ディレクトリパスを削除し，ファイル名を返却
@param path ディスレクトリパスを取り除くファイルパス
@return ファイル名を返却
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
@brief ディレクトリパスを削除し，ファイル名を返却(ワイド文字列)
@param path ディスレクトリパスを取り除くファイルパス
@return ファイル名を返却
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
@brief ディレクトリ名を取得
@param filePath ファイルパス
@return  指定されたファイルパスからディレクトリ名を抜き出す
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
@brief ディレクトリ名を取得(ワイド文字列)
@param filePath ファイルパス
@return  指定されたファイルパスからディレクトリ名を抜き出す
*/
std::wstring GetDirectoryPathW(const wchar_t* filePath)
{
	std::wstring path = Replace(filePath, L"\\", L"/");
	size_t idx = path.find_last_of(L"/");
	if (idx != std::wstring::npos)
		return path.substr(0, idx + 1);

	return std::wstring();
}

