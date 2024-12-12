#pragma once
#include <Windows.h>

HMODULE LoadSystemLibrary(const wchar_t *filename);
UINT ReadInterval(HMODULE module);
void Patch(HMODULE module);
