#pragma once

enum class TerminalKind
{
    ConHostLegacy = 0,
    ConHostModern,
    WindowsTerminal,
    UnknownConPTY,
    Redirected
};

struct TerminalInfo
{
    TerminalKind kind;
    unsigned osBuild;
    bool vtEnabled;
    bool canResize;
    unsigned inputCp;
    unsigned outputCp;
    int fontWidth;
    int fontHeight;
    int cellColumnsPerBlock;
};

TerminalInfo ProbeTerminal();
const TerminalInfo& CachedTerminal();
void ResetTerminalCache();
int LogicalToColumns(int logicalWidth, int cellColumnsPerBlock);
bool CanResizeHost(bool classicHost, TerminalKind kind);
bool ApplyConsoleSize(int columns, int rows, const TerminalInfo& info);
bool ApplyConsoleColor(const char* color, const TerminalInfo& info);
void SetCellColumnsPerBlock(int columns);
void SetConsoleFontForGrid(int columns, int rows);
