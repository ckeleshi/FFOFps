#include "patch.h"
#include "byte_pattern.h"
#include "injector/hooking.hpp"
#include <ShlObj.h>
#include <chrono>
#include <filesystem>
#include <optional>

// 提高Sleep函数的精度
class time_period_guard
{
  public:
    time_period_guard()
    {
        std::memset(&_caps, 0, sizeof(_caps));
        timeGetDevCaps(&_caps, sizeof(_caps));
        timeBeginPeriod(_caps.wPeriodMin);
    }

    ~time_period_guard()
    {
        timeEndPeriod(_caps.wPeriodMin);
    }

  private:
    TIMECAPS _caps;
};

std::optional<time_period_guard> g_period_guard;

// 单个进程只进行一次patch
class inprocess_singleton
{
  public:
    inprocess_singleton()
    {
        wchar_t mutex_name_buffer[MAX_PATH];

        std::swprintf(mutex_name_buffer, L"FFOFPS_MUTEX_%u", GetCurrentProcessId());

        auto mutex = CreateMutexW(nullptr, FALSE, mutex_name_buffer);

        if (mutex != nullptr)
        {
            if (GetLastError() == ERROR_ALREADY_EXISTS)
            {
                CloseHandle(mutex);
            }
            else if (GetLastError() == ERROR_SUCCESS)
            {
                _mutex = mutex;
            }
        }
    }

    ~inprocess_singleton()
    {
        if (_mutex)
        {
            CloseHandle(_mutex);
        }
    }

    bool is_first_load() const
    {
        return _mutex != nullptr;
    }

  private:
    HANDLE _mutex;
} g_singleton;

static UINT ReadFPS(HMODULE module)
{
    wchar_t               cPath[MAX_PATH];
    std::filesystem::path cppPath;

    GetModuleFileNameW(module, cPath, MAX_PATH);
    cppPath = cPath;
    cppPath = cppPath.parent_path() / L"FFOFPS.ini";

    return std::clamp(GetPrivateProfileIntW(L"FFOFPS", L"FPS", 60u, cppPath.wstring().c_str()), 33u, 9999u);
}

static std::chrono::steady_clock::duration                  fps_interval;
static std::optional<std::chrono::steady_clock::time_point> next_process_time;

// 在还未到下一帧时进行等待
static void WINAPI FPSSleep(DWORD dwMilliseconds)
{
    auto now = std::chrono::steady_clock::now();

    if (!next_process_time.has_value())
    {
        next_process_time.emplace(now + fps_interval);
    }

    while (now < next_process_time.value())
    {
        if (next_process_time.value() - now > std::chrono::milliseconds(2))
        {
            Sleep(1);
        }
        else
        {
            Sleep(0);
        }

        now = std::chrono::steady_clock::now();
    }

    next_process_time.value() += fps_interval;
}

HMODULE LoadSystemLibrary(const wchar_t *filename)
{
    wchar_t *szSystemPath = nullptr;

    SHGetKnownFolderPath(FOLDERID_System, 0, NULL, &szSystemPath);

    std::filesystem::path path = szSystemPath;

    CoTaskMemFree(szSystemPath);

    // SHGetKnownFolderPath结果没有反斜杠的
    return LoadLibraryW((path / filename).c_str());
}

void Patch(HMODULE module)
{
    if (!g_singleton.is_first_load())
    {
        return;
    }

    g_period_guard.emplace();

    auto fps     = ReadFPS(module);
    // 计算每帧的steady_clock时间间隔
    fps_interval = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds(1)) / fps;

    byte_pattern patterner;

    patterner.find_pattern("83 65 08 00 8B 1D");
    if (patterner.has_size(1))
    {
        unsigned char asm_bytes[] = {0x90, 0xBB}; // nop; mov ebx, &FPSSleep;
        injector::WriteMemoryRaw(patterner.get(0).i(4), asm_bytes, 2, true);
        injector::WriteObject(patterner.get(0).i(6), &FPSSleep, true);
    }
}
