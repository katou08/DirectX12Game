#pragma once
// �v���R���p�C���ς݃w�b�_�[
// ����̂݉�͂���邽�߃R���p�C�����Ԃ̒Z�k�ɂȂ�
// �S�Ă�cpp����include�����K�v������

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

// DX12�֘A
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include "d3dx12.h"

// DirectXTK�֘A
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

// Assimp�֘A
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif

// imgui�֘A
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx12.h>

// FileUtil
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// �񓯊������p
#include <future>

#include "Utility.h"
#include "FileUtil.h"
#include "Type.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;