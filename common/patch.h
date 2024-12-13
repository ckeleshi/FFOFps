#pragma once
#include <Windows.h>

HMODULE LoadSystemLibrary(const wchar_t *filename);
UINT ReadFPS(HMODULE module);
void Patch(HMODULE module);
