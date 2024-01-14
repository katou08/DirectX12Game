#pragma once
// プリコンパイル済みヘッダー
// 初回のみ解析されるためコンパイル時間の短縮になる
// 全てのcppからincludeされる必要がある

#define NOMINMAX
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <iostream>
#include <cassert>
#include <codecvt>
#include <exception>
#include <stdexcept>
#include <stdint.h>
#include <chrono>
#include <random>
#include <wrl.h>

#include <memory>

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>

// DX12関連
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include "d3dx12.h"

// DirectXTK関連
#include <DirectXTK12/SimpleMath.h>
#include <DirectXTK12/ResourceUploadBatch.h>
#include <DirectXTK12/DDSTextureLoader.h>
#include <DirectXTK12/CommonStates.h>
#include <DirectXTK12/Keyboard.h>
#include <DirectXTK12/Mouse.h>
#include <DirectXTK12/Audio.h>
#include <DirectXTK12/SpriteBatch.h>
#include <DirectXTK12/SpriteFont.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "DirectXTK12.lib")

// Assimp関連
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif

// imgui関連
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx12.h>

// FileUtil
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// 非同期処理用
#include <future>

#include "Utility.h"
#include "FileUtil.h"
#include "Type.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;