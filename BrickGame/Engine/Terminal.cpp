#include "Terminal.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

namespace
{
TerminalInfo g_cache{};
bool g_ready = false;

unsigned QueryOsBuild()
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (!ntdll)
        return 0;
    FARPROC raw = GetProcAddress(ntdll, "RtlGetVersion");
    if (!raw)
        return 0;
    RtlGetVersionPtr fn = nullptr;
    std::memcpy(&fn, &raw, sizeof(fn));
    RTL_OSVERSIONINFOW info{};
    info.dwOSVersionInfoSize = sizeof(info);
    if (fn(&info) != 0)
        return 0;
    return info.dwBuildNumber;
}

bool TryEnableVt(HANDLE out)
{
    DWORD mode = 0;
    if (!GetConsoleMode(out, &mode))
        return false;
    if (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        return true;
    return SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
}

int Nibble(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

bool ParseColorPair(const char* color, int& bg, int& fg)
{
    if (!color || !color[0] || !color[1] || color[2] != '\0')
        return false;
    bg = Nibble(color[0]);
    fg = Nibble(color[1]);
    return bg >= 0 && fg >= 0;
}

bool ResizeWithApi(HANDLE out, int columns, int rows)
{
    if (columns < 1)
        columns = 1;
    if (rows < 1)
        rows = 1;
    SMALL_RECT tiny{0, 0, 0, 0};
    SetConsoleWindowInfo(out, TRUE, &tiny);

    COORD size{static_cast<SHORT>(columns), static_cast<SHORT>(rows)};
    if (!SetConsoleScreenBufferSize(out, size))
        return false;

    SMALL_RECT rect{0, 0, static_cast<SHORT>(columns - 1), static_cast<SHORT>(rows - 1)};
    SetConsoleWindowInfo(out, TRUE, &rect);

    CONSOLE_SCREEN_BUFFER_INFO buffer{};
    if (!GetConsoleScreenBufferInfo(out, &buffer))
        return false;
    return buffer.dwSize.X == columns;
}

void TryVtWindowResize(HANDLE out, int columns, int rows)
{
    char buf[48];
    const int n = std::snprintf(buf, sizeof(buf), "\x1b[8;%d;%dt", rows, columns);
    if (n <= 0)
        return;
    DWORD written = 0;
    WriteConsoleA(out, buf, static_cast<DWORD>(n), &written, nullptr);
}

int QueryBufferColumns(HANDLE out)
{
    CONSOLE_SCREEN_BUFFER_INFO buffer{};
    if (!GetConsoleScreenBufferInfo(out, &buffer))
        return 0;
    return buffer.dwSize.X;
}

HWND ConsoleHwnd()
{
    HWND hwnd = GetConsoleWindow();
    if (hwnd == nullptr || !IsWindow(hwnd))
        return nullptr;
    return hwnd;
}

void ApplyFixedWindowStyle(HWND hwnd)
{
    LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    style &= ~(static_cast<LONG_PTR>(WS_THICKFRAME) | static_cast<LONG_PTR>(WS_MAXIMIZEBOX) |
               static_cast<LONG_PTR>(WS_VSCROLL) | static_cast<LONG_PTR>(WS_HSCROLL));
    style |= static_cast<LONG_PTR>(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER);
    SetWindowLongPtrW(hwnd, GWL_STYLE, style);

    HMENU menu = GetSystemMenu(hwnd, FALSE);
    if (menu != nullptr)
        EnableMenuItem(menu, SC_MOVE, MF_BYCOMMAND | MF_ENABLED);

    ShowScrollBar(hwnd, SB_BOTH, FALSE);
}

bool ForceBufferEqualsWindow(HANDLE out, int columns, int rows)
{
    SMALL_RECT tiny{0, 0, 0, 0};
    SetConsoleWindowInfo(out, TRUE, &tiny);
    COORD size{static_cast<SHORT>(columns), static_cast<SHORT>(rows)};
    if (!SetConsoleScreenBufferSize(out, size))
        return false;
    SMALL_RECT rect{0, 0, static_cast<SHORT>(columns - 1), static_cast<SHORT>(rows - 1)};
    return SetConsoleWindowInfo(out, TRUE, &rect) != 0;
}

COORD MeasureCellPixels(HWND hwnd, HANDLE out)
{
    COORD cell{8, 16};
    CONSOLE_FONT_INFO font{};
    if (GetCurrentConsoleFont(out, FALSE, &font))
    {
        const COORD size = GetConsoleFontSize(out, font.nFont);
        if (size.X > 0)
            cell.X = size.X;
        if (size.Y > 0)
            cell.Y = size.Y;
    }

    CONSOLE_SCREEN_BUFFER_INFO info{};
    RECT client{};
    if (!GetConsoleScreenBufferInfo(out, &info) || !GetClientRect(hwnd, &client))
        return cell;
    const int visCols = info.srWindow.Right - info.srWindow.Left + 1;
    const int visRows = info.srWindow.Bottom - info.srWindow.Top + 1;
    if (visCols > 0 && visRows > 0 && client.right > 0 && client.bottom > 0)
    {
        const int cw = client.right / visCols;
        const int ch = client.bottom / visRows;
        if (cw > 0)
            cell.X = static_cast<SHORT>(cw);
        if (ch > 0)
            cell.Y = static_cast<SHORT>(ch);
    }
    return cell;
}

void FitWindowToCells(HWND hwnd, HANDLE out, int columns, int rows)
{
    const COORD cell = MeasureCellPixels(hwnd, out);
    RECT rc{0, 0, columns * cell.X, rows * cell.Y};
    const DWORD style = static_cast<DWORD>(GetWindowLongPtrW(hwnd, GWL_STYLE));
    const DWORD ex = static_cast<DWORD>(GetWindowLongPtrW(hwnd, GWL_EXSTYLE));
    AdjustWindowRectEx(&rc, style, FALSE, ex);
    SetWindowPos(hwnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void LockConsoleHostWindow(int columns, int rows)
{
    HWND hwnd = ConsoleHwnd();
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hwnd == nullptr || out == nullptr || out == INVALID_HANDLE_VALUE)
        return;

    ApplyFixedWindowStyle(hwnd);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    ForceBufferEqualsWindow(out, columns, rows);
    FitWindowToCells(hwnd, out, columns, rows);
    ForceBufferEqualsWindow(out, columns, rows);
    ApplyFixedWindowStyle(hwnd);
}

int AnsiFg(int c)
{
    static const int map[16] = {30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97};
    return map[c & 15];
}

int AnsiBg(int c)
{
    static const int map[16] = {40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107};
    return map[c & 15];
}
} // namespace

TerminalInfo ProbeTerminal()
{
    TerminalInfo info{};
    info.cellColumnsPerBlock = 2;
    info.osBuild = QueryOsBuild();
    info.inputCp = GetConsoleCP();
    info.outputCp = GetConsoleOutputCP();

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == nullptr || out == INVALID_HANDLE_VALUE)
    {
        info.kind = TerminalKind::Redirected;
        return info;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(out, &mode))
    {
        info.kind = TerminalKind::Redirected;
        return info;
    }

    info.vtEnabled = TryEnableVt(out);
    CONSOLE_FONT_INFOEX font{};
    font.cbSize = sizeof(font);
    if (GetCurrentConsoleFontEx(out, FALSE, &font))
    {
        info.fontWidth = font.dwFontSize.X;
        info.fontHeight = font.dwFontSize.Y;
    }

    HWND hwnd = ConsoleHwnd();
    wchar_t cls[128]{};
    if (hwnd != nullptr)
        GetClassNameW(hwnd, cls, 128);
    const bool classicHost = hwnd != nullptr && lstrcmpiW(cls, L"ConsoleWindowClass") == 0;

    const char* wt = std::getenv("WT_SESSION");
    if (!classicHost && wt != nullptr && wt[0] != '\0')
    {
        info.kind = TerminalKind::WindowsTerminal;
        info.canResize = false;
        return info;
    }

    if (info.vtEnabled)
    {
        info.kind = TerminalKind::ConHostModern;
        info.canResize = false;
        return info;
    }

    if (info.osBuild >= 18362)
    {
        info.kind = TerminalKind::UnknownConPTY;
        info.canResize = false;
        return info;
    }

    info.kind = TerminalKind::ConHostLegacy;
    info.canResize = false;
    return info;
}

const TerminalInfo& CachedTerminal()
{
    if (!g_ready)
    {
        g_cache = ProbeTerminal();
        g_ready = true;
    }
    return g_cache;
}

void ResetTerminalCache()
{
    g_ready = false;
}

int LogicalToColumns(int logicalWidth, int cellColumnsPerBlock)
{
    if (logicalWidth < 0)
        logicalWidth = 0;
    if (cellColumnsPerBlock < 1)
        cellColumnsPerBlock = 1;
    return logicalWidth * cellColumnsPerBlock;
}

bool ApplyConsoleSize(int columns, int rows, const TerminalInfo& info)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (info.kind == TerminalKind::Redirected || out == nullptr || out == INVALID_HANDLE_VALUE)
        return false;
    if (columns < 1)
        columns = 1;
    if (rows < 1)
        rows = 1;

    // Snake maps wrap at one row of 40 fullwidth cells = width columns.
    if (info.vtEnabled)
        TryVtWindowResize(out, columns, rows);
    ResizeWithApi(out, columns, rows);
    if (QueryBufferColumns(out) != columns)
    {
        std::string cmd = "mode con cols=" + std::to_string(columns) + " lines=" + std::to_string(rows);
        std::system(cmd.c_str());
        ResizeWithApi(out, columns, rows);
    }
    LockConsoleHostWindow(columns, rows);
    return QueryBufferColumns(out) == columns;
}

bool ApplyConsoleColor(const char* color, const TerminalInfo& info)
{
    int bg = 8;
    int fg = 0;
    if (!ParseColorPair(color, bg, fg))
    {
        bg = 8;
        fg = 0;
    }

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    const WORD attr = static_cast<WORD>((bg << 4) | fg);

    if (info.vtEnabled && info.kind != TerminalKind::Redirected && out != nullptr &&
        out != INVALID_HANDLE_VALUE)
    {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "\x1b[%d;%dm", AnsiFg(fg), AnsiBg(bg));
        DWORD written = 0;
        WriteConsoleA(out, buf, static_cast<DWORD>(std::strlen(buf)), &written, nullptr);
        SetConsoleTextAttribute(out, attr);
        return true;
    }

    if (out != nullptr && out != INVALID_HANDLE_VALUE && SetConsoleTextAttribute(out, attr))
        return true;

    std::string cmd = std::string("color ") + (color != nullptr ? color : "80");
    return std::system(cmd.c_str()) == 0;
}
