#include "pch.h"
#include "Window.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! @brief �E�B���h�E�v���V�[�W���[�֐�
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
	case WM_CREATE: // �E�B���h�E���쐬���ꂽ�Ƃ��ɃC�x���g������
	{
		break;
	}
	case WM_SIZE: // �E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��ɃC�x���g������
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onSize(window->getClientSize());
		break;
	}
	case WM_SETFOCUS: // �t�H�[�J�X���擾�����Ƃ��C�x���g������
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (window) window->onFocus();
		break;
	}
	case WM_KILLFOCUS: // �t�H�[�J�X�̏������C�x���g������
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY: // �E�B���h�E���j�����ꂽ�Ƃ��ɃC�x���g������
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
		break;
	}
	case WM_CLOSE: // �E�B���h�E�̕���{�^���������ꂽ��
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

//! @brief �R���X�g���N�^ (�E�B���h�E�̏�����)
Window::Window()
{
	// �E�B���h�E�̐ݒ�
	WNDCLASSEX wc = {};
	wc.cbSize			= sizeof(WNDCLASSEX);					//!< �\���̂̃T�C�Y
	wc.style			= CS_HREDRAW | CS_VREDRAW;				//!< �E�B���h�E�N���X�̃X�^�C��
	wc.lpfnWndProc		= &WndProc;								//!< �E�B���h�E�v���V�[�W���[���w��
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);			//!< �w�i�u���V�̃n���h��
	wc.lpszClassName	= L"MyWindowClass";						//!< �E�B���h�E�N���X�̖��O

	// �E�B���h�E�̓o�^
	auto classId = RegisterClassEx(&wc);
	if (!classId) ThrowFailed("�E�B���h�E�̍쐬�Ɏ��s���܂���.");

	RECT rc = { 0, 0, m_size.width, m_size.height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�n���h���̍쐬
	m_hwnd = ::CreateWindowEx(NULL, MAKEINTATOM(classId), L"DirectX Game",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, NULL, NULL);

	if (!m_hwnd) ThrowFailed("�E�B���h�E�쐬�Ɏ��s���܂���.");

	auto hwnd = static_cast<HWND>(m_hwnd);

	if (hwnd != nullptr)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

		// �E�B���h�E�̕\��
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);
	}
	else
		ThrowFailed("�E�B���h�E�쐬�Ɏ��s���܂���.");
}

//! @brief �f�X�g���N�^
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
