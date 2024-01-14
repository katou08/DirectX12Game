#include "pch.h"
#include "Window.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! @brief ウィンドウプロシージャー関数
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
	case WM_CREATE: // ウィンドウが作成されたときにイベントが発生
	{
		break;
	}
	case WM_SIZE: // ウィンドウのサイズが変更されたときにイベントが発生
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onSize(window->getClientSize());
		break;
	}
	case WM_SETFOCUS: // フォーカスを取得したときイベントが発生
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onFocus();
		break;
	}
	case WM_KILLFOCUS: // フォーカスの消失時イベントが発生
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY: // ウィンドウが破棄されたときにイベントが発生
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		break;
	}
	case WM_CLOSE: // ウィンドウの閉じるボタンが押されたら
	{
		PostQuitMessage(0);
		break;
	}
	case WM_ACTIVATEAPP:
	{
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		break;
	}
	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		break;
	case WM_SYSKEYDOWN:
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
		{
		}
		break;
	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

//! @brief コンストラクタ (ウィンドウの初期化)
Window::Window()
{
	// ウィンドウの設定
	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof(WNDCLASSEX);					//!< 構造体のサイズ
	wc.style			= CS_HREDRAW | CS_VREDRAW;				//!< ウィンドウクラスのスタイル
	wc.lpfnWndProc		= &WndProc;								//!< ウィンドウプロシージャーを指定
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);			//!< 背景ブラシのハンドル
	wc.lpszClassName	= L"MyWindowClass";						//!< ウィンドウクラスの名前

	// ウィンドウの登録
	auto classId = RegisterClassEx(&wc);
	if (!classId) ThrowFailed("ウィンドウの作成に失敗しました.");

	RECT rc = { 0, 0, m_size.width, m_size.height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウハンドルの作成
	m_hwnd = ::CreateWindowEx(NULL, MAKEINTATOM(classId), L"DirectX Game",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd) ThrowFailed("ウィンドウ作成に失敗しました.");

	auto hwnd = static_cast<HWND>(m_hwnd);

	if (hwnd != nullptr)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

		// ウィンドウの表示
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
	}
	else
		ThrowFailed("ウィンドウ作成に失敗しました.");
}

//! @brief デストラクタ
Window::~Window()
{
	DestroyWindow(static_cast<HWND>(m_hwnd));
}

Rect Window::getClientSize()
{
	RECT rc = {};
	auto hwnd = static_cast<HWND>(m_hwnd);
	GetClientRect(hwnd, &rc);
	ClientToScreen(hwnd, (LPPOINT)&rc.left);
	ClientToScreen(hwnd, (LPPOINT)&rc.right);

	return { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

Rect Window::getScreenSize()
{
	RECT rc = {};

	rc.right = GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = GetSystemMetrics(SM_CYSCREEN);

	return { 0, 0, rc.right - rc.left, rc.bottom - rc.top };
}

void Window::onCreate()
{
}

void Window::onUpdate()
{
}

void Window::onDestroy()
{
}

void Window::onFocus()
{
}

void Window::onKillFocus()
{
}

void Window::onSize(const Rect& size)
{
}
