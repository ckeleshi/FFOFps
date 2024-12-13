#include <ShlObj.h>
#include <patch.h>
#include <string>

HMODULE DDRAWModule;
FARPROC fnDirectDrawCreate;

__declspec(naked) void *WINAPI DirectDrawCreate_stub(UINT SDKVersion)
{
    __asm { jmp fnDirectDrawCreate }
}

void LoadDDRAW()
{
    DDRAWModule        = LoadSystemLibrary(L"ddraw.dll");
    fnDirectDrawCreate = GetProcAddress(DDRAWModule, "DirectDrawCreate");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        LoadDDRAW();
        Patch(hModule);
        break;
    }

    default:
        break;
    }

    return TRUE;
}
