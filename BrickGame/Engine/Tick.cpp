#include "Tick.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace
{
int g_speed = -1;
bool g_tickStarted = false;
long long g_lastTick = 0;

int ReadEnvSpeed()
{
    wchar_t buf[16];
    const DWORD n = GetEnvironmentVariableW(L"BRICK_SPEED", buf, 16);
    if (n == 0 || n >= 16)
        return 5;
    return ClampMachineSpeed(_wtoi(buf));
}
} // namespace

int ClampMachineSpeed(int speed)
{
    if (speed < 1)
        return 1;
    if (speed > 10)
        return 10;
    return speed;
}

int GetMachineSpeed()
{
    if (g_speed < 0)
        g_speed = ReadEnvSpeed();
    return g_speed;
}

void SetMachineSpeed(int speed)
{
    g_speed = ClampMachineSpeed(speed);
    const std::wstring value = std::to_wstring(g_speed);
    SetEnvironmentVariableW(L"BRICK_SPEED", value.c_str());
}

int LogicIntervalMs()
{
    const int speed = GetMachineSpeed();
    return 700 - (speed - 1) * (700 - 60) / 9;
}

void PumpFrame()
{
    Sleep(8);
}

bool DueLogicTick()
{
    LARGE_INTEGER now{};
    LARGE_INTEGER freq{};
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&now);
    if (!g_tickStarted)
    {
        g_tickStarted = true;
        g_lastTick = now.QuadPart;
        return false;
    }
    const double ms = (now.QuadPart - g_lastTick) * 1000.0 / static_cast<double>(freq.QuadPart);
    if (ms >= LogicIntervalMs())
    {
        g_lastTick = now.QuadPart;
        return true;
    }
    return false;
}

void ResetTickState()
{
    g_speed = -1;
    g_tickStarted = false;
    g_lastTick = 0;
}
