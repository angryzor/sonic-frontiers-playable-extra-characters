#include <bootstrap.h>

HOOK(uint64_t, __fastcall, GameModeBootInit, 0x1401AADF0, app::game::GameMode* self) {
	auto res = originalGameModeBootInit(self);
	Bootstrap();
	return res;
}

BOOL WINAPI DllMain(_In_ HINSTANCE hInstance, _In_ DWORD reason, _In_ LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		INSTALL_HOOK(GameModeBootInit);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
