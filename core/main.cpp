#include "pch.h"
#include "Game/Game.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //!< ƒƒ‚ƒŠƒŠ[ƒN‚ð’m‚ç‚¹‚é

	try
	{
		Game game;
		bool success = game.Initialize();
		if (success)
		{
			game.RunLoop();
		}
		game.Shutdown();
	}
	catch (const std::runtime_error e)
	{
		OutputDebugStringA((std::string(e.what()) + "\n").c_str());
		return -1;
	}
	catch (...)
	{
		OutputDebugStringA("Unknown Error \n");
		return -1;
	}

	return 0;
}