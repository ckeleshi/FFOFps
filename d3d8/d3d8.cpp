#include <ShlObj.h>
#include <patch.h>
#include <string>

HMODULE D3D8Module;
FARPROC fnDirect3DCreate8;

__declspec(naked) void *WINAPI Direct3DCreate8_stub(UINT SDKVersion)
{
    __asm { jmp fnDirect3DCreate8 }
}

void LoadD3D8()
{
    D3D8Module        = LoadSystemLibrary(L"d3d8.dll");
    fnDirect3DCreate8 = GetProcAddress(D3D8Module, "Direct3DCreate8");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        LoadD3D8();
        Patch(hModule);
        break;
    }

    default:
        break;
    }

    return TRUE;
}
