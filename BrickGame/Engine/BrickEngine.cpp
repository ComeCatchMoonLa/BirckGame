#include "BrickEngine.h"
#include <TlHelp32.h>

namespace
{
void EnsureUtf8Console()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void SetConsoleTitleUtf8(const std::string& name)
{
    const int n = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
    if (n <= 0)
        return;
    std::wstring wide(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, wide.data(), n);
    SetConsoleTitleW(wide.c_str());
}

bool AlreadyFixedHost()
{
    wchar_t buf[4];
    return GetEnvironmentVariableW(L"BRICK_FIXED_HOST", buf, 4) > 0;
}

bool HostIsClassicConsole()
{
    HWND hwnd = GetConsoleWindow();
    if (hwnd == nullptr || !IsWindow(hwnd))
        return false;
    wchar_t cls[128]{};
    if (GetClassNameW(hwnd, cls, 128) <= 0)
        return false;
    return lstrcmpiW(cls, L"ConsoleWindowClass") == 0;
}

bool ParentIsConhost()
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    const DWORD pid = GetCurrentProcessId();
    DWORD ppid = 0;
    wchar_t parentName[MAX_PATH]{};
    if (Process32FirstW(snap, &pe))
    {
        do
        {
            if (pe.th32ProcessID == pid)
                ppid = pe.th32ParentProcessID;
        } while (Process32NextW(snap, &pe));
    }
    pe.dwSize = sizeof(pe);
    if (ppid != 0 && Process32FirstW(snap, &pe))
    {
        do
        {
            if (pe.th32ProcessID == ppid)
            {
                lstrcpynW(parentName, pe.szExeFile, MAX_PATH);
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return lstrcmpiW(parentName, L"conhost.exe") == 0;
}

bool RelaunchInConhost()
{
    wchar_t exe[MAX_PATH];
    if (GetModuleFileNameW(nullptr, exe, MAX_PATH) == 0)
        return false;

    wchar_t sys[MAX_PATH];
    if (GetSystemDirectoryW(sys, MAX_PATH) == 0)
        return false;
    const std::wstring host = std::wstring(sys) + L"\\conhost.exe";
    if (GetFileAttributesW(host.c_str()) == INVALID_FILE_ATTRIBUTES)
        return false;

    std::wstring dir(exe);
    const size_t slash = dir.find_last_of(L"\\/");
    if (slash == std::wstring::npos)
        dir = L".";
    else
        dir.resize(slash);

    const std::wstring cmd = L"\"" + host + L"\" -- \"" + std::wstring(exe) + L"\"";
    std::vector<wchar_t> cmdBuf(cmd.begin(), cmd.end());
    cmdBuf.push_back(0);

    SetEnvironmentVariableW(L"BRICK_FIXED_HOST", L"1");

    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESHOWWINDOW;
    startup.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(host.c_str(), cmdBuf.data(), nullptr, nullptr, FALSE, 0, nullptr, dir.c_str(),
                        &startup, &process))
        return false;
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return true;
}
} // namespace

void SetPos(int i, short j)
{
    int step = CachedTerminal().cellColumnsPerBlock;
    if (step < 1)
        step = 1;
    COORD pos = {static_cast<SHORT>(i * step), j};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetConsole(std::string name, int width, int height, std::string color)
{
    EnsureUtf8Console();
    if (!AlreadyFixedHost() && !ParentIsConhost() && !HostIsClassicConsole())
    {
        if (RelaunchInConhost())
            ExitProcess(0);
    }
    ResetTerminalCache();
    const TerminalInfo info = CachedTerminal();
    SetConsoleTitleUtf8(name);
    ApplyConsoleSize(width, height, info);
    ApplyConsoleColor(color.c_str(), info);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    if (GetConsoleCursorInfo(handle, &cursorInfo))
    {
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(handle, &cursorInfo);
    }
}

void Pause()
{
    while (true)
        if (kbhit() && getch() == 32)
            break;
}

void FillStr(int x, int y, const std::string& fillstr)
{
    SetPos(x, y);
    std::cout << fillstr;
}

void FillRec(int x, int y, int width, int height, const std::string& fillstr)
{
    for (int i = 0; i < height; ++i)
    {
        SetPos(x, y + i);
        for (int j = width; j--;)
            std::cout << fillstr;
    }
}

void FillArea(int x, int y, const Sites& sites, const std::string& fillstr)
{
    for (const Vector2& point : sites)
    {
        SetPos(x + point.x, y + point.y);
        std::cout << fillstr;
    }
}
