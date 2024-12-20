﻿#include <patch.h>
#include <string>

HMODULE D3D9Module;
FARPROC fnDirect3DCreate9;

__declspec(naked) void *WINAPI Direct3DCreate9_stub(UINT SDKVersion)
{
    __asm { jmp fnDirect3DCreate9 }
}

void LoadD3D9()
{
    D3D9Module        = LoadSystemLibrary(L"d3d9.dll");
    fnDirect3DCreate9 = GetProcAddress(D3D9Module, "Direct3DCreate9");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        LoadD3D9();
        Patch(hModule);
        break;
    }

    default:
        break;
    }

    return TRUE;
}
