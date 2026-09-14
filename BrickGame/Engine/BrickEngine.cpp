#include "BrickEngine.h"
#include <TlHelp32.h>

namespace
{
void WriteFill(int x, int y, const std::string& fillstr)
{
    SetPos(x, y);
    if (fillstr.empty())
        return;
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    const int bytes = static_cast<int>(fillstr.size());
    const int n = MultiByteToWideChar(CP_UTF8, 0, fillstr.c_str(), bytes, nullptr, 0);
    if (n <= 0)
        return;
    std::wstring wide(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, fillstr.c_str(), bytes, wide.data(), n);
    DWORD written = 0;
    WriteConsoleW(out, wide.c_str(), static_cast<DWORD>(n), &written, nullptr);
}

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

void SetConsoleFromGameId(int id)
{
    const GameEntry* entry = FindGameById(id);
    if (entry == nullptr || entry->name == nullptr)
        return;
    SetConsole(entry->name, entry->windowColumns, entry->windowRows, "80");
}

void ClearScreen()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == nullptr || out == INVALID_HANDLE_VALUE)
        return;
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (!GetConsoleScreenBufferInfo(out, &info))
        return;
    const DWORD cells = static_cast<DWORD>(info.dwSize.X) * static_cast<DWORD>(info.dwSize.Y);
    COORD origin{0, 0};
    DWORD written = 0;
    FillConsoleOutputCharacterW(out, L' ', cells, origin, &written);
    FillConsoleOutputAttribute(out, info.wAttributes, cells, origin, &written);
    SetConsoleCursorPosition(out, origin);
}

void QuitToLauncher()
{
    ExitProcess(0);
}

bool SpawnGameInClassicHost(const wchar_t* exePath, const wchar_t* workDir, int columns, int rows,
                             PROCESS_INFORMATION* out)
{
    if (out == nullptr || exePath == nullptr)
        return false;
    ZeroMemory(out, sizeof(*out));

    wchar_t sys[MAX_PATH];
    const UINT n = GetSystemDirectoryW(sys, MAX_PATH);
    std::wstring host;
    if (n > 0 && n < MAX_PATH)
        host = std::wstring(sys) + L"\\conhost.exe";
    const bool useConhost =
        !host.empty() && GetFileAttributesW(host.c_str()) != INVALID_FILE_ATTRIBUTES;

    std::wstring app = exePath;
    std::wstring cmd = L"\"" + std::wstring(exePath) + L"\"";
    DWORD flags = CREATE_NEW_CONSOLE;
    if (useConhost)
    {
        app = host;
        cmd = L"\"" + host + L"\" -- \"" + std::wstring(exePath) + L"\"";
        flags = 0;
    }
    std::vector<wchar_t> cmdBuf(cmd.begin(), cmd.end());
    cmdBuf.push_back(0);

    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESHOWWINDOW;
    startup.wShowWindow = SW_SHOW;
    if (columns > 0 && rows > 0)
    {
        startup.dwFlags |= STARTF_USECOUNTCHARS;
        startup.dwXCountChars = static_cast<DWORD>(columns);
        startup.dwYCountChars = static_cast<DWORD>(rows);
    }

    const wchar_t* dir = workDir != nullptr && workDir[0] != 0 ? workDir : nullptr;
    if (useConhost)
        SetEnvironmentVariableW(L"BRICK_FIXED_HOST", L"1");
    if (!CreateProcessW(app.c_str(), cmdBuf.data(), nullptr, nullptr, FALSE, flags, nullptr, dir,
                        &startup, out))
    {
        if (useConhost)
            SetEnvironmentVariableW(L"BRICK_FIXED_HOST", nullptr);
        return false;
    }
    if (useConhost)
        SetEnvironmentVariableW(L"BRICK_FIXED_HOST", nullptr);
    return true;
}

void Pause()
{
    for (;;)
    {
        if (!kbhit())
        {
            PumpFrame();
            continue;
        }
        const int ch = getch();
        if (ch == 27)
            QuitToLauncher();
        if (ch == 32)
            break;
    }
}

void FillStr(int x, int y, const std::string& fillstr)
{
    WriteFill(x, y, fillstr);
}

void FillRec(int x, int y, int width, int height, const std::string& fillstr)
{
    if (width < 1 || height < 1)
        return;
    std::string row;
    row.reserve(fillstr.size() * static_cast<size_t>(width));
    for (int j = 0; j < width; ++j)
        row += fillstr;
    for (int i = 0; i < height; ++i)
        WriteFill(x, y + i, row);
}

void FillArea(int x, int y, const Sites& sites, const std::string& fillstr)
{
    for (const Vector2& point : sites)
        WriteFill(x + point.x, y + point.y, fillstr);
}
