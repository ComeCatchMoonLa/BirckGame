#include "BrickEngine.h"
#include <iostream>
#include <string>
#include <vector>

namespace
{
std::wstring ExeDir()
{
    wchar_t buf[MAX_PATH];
    DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH);
    if (n == 0)
        return L".";
    std::wstring path(buf, n);
    const size_t pos = path.find_last_of(L"\\/");
    if (pos == std::wstring::npos)
        return L".";
    return path.substr(0, pos);
}

std::wstring AsciiToWide(const char* text)
{
    std::wstring out;
    if (text == nullptr)
        return out;
    for (const char* p = text; *p != '\0'; ++p)
        out.push_back(static_cast<unsigned char>(*p));
    return out;
}

int Utf8DisplayWidth(const char* text)
{
    int width = 0;
    if (text == nullptr)
        return 0;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(text); *p != '\0';)
    {
        if (*p < 0x80)
        {
            ++width;
            ++p;
        }
        else if ((*p & 0xE0) == 0xC0)
        {
            ++width;
            p += 2;
        }
        else if ((*p & 0xF0) == 0xE0)
        {
            width += 2;
            p += 3;
        }
        else if ((*p & 0xF8) == 0xF0)
        {
            width += 2;
            p += 4;
        }
        else
            ++p;
    }
    return width;
}

void PrintMenuCell(const GameEntry& game, int cellWidth)
{
    std::string cell;
    if (game.id < 10)
        cell.push_back(' ');
    cell += std::to_string(game.id);
    cell += '.';
    cell += game.name;
    if (game.status == GameStatus::Unimplemented)
        cell.push_back('*');
    std::cout << cell;
    for (int i = Utf8DisplayWidth(cell.c_str()); i < cellWidth; ++i)
        std::cout << ' ';
}

void SetupConsole()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    constexpr int kMenuCols = 3;
    const int gameRows = (GetGameCount() + kMenuCols - 1) / kMenuCols;
    SetConsole("Brick Game", 80, gameRows + 6, "80");
}

void WaitEnter();

void DrawMenu()
{
    constexpr int kMenuCols = 3;
    constexpr int kCellWidth = 26;
    ClearScreen();
    SetupConsole();
    std::cout << "Brick Game  SPEED " << GetMachineSpeed() << "\n";
    std::cout << "输入编号回车，s 改速度，h 排行榜，0 退出；* 未实现\n";
    const int n = GetGameCount();
    const int gameRows = (n + kMenuCols - 1) / kMenuCols;
    for (int r = 0; r < gameRows; ++r)
    {
        for (int c = 0; c < kMenuCols; ++c)
        {
            const int index = r + c * gameRows;
            if (index < n)
                PrintMenuCell(GetGameAt(index), kCellWidth);
            else
            {
                for (int i = 0; i < kCellWidth; ++i)
                    std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << "> " << std::flush;
}

void DrawRanking()
{
    ClearScreen();
    SetConsole("Brick Game", 80, 16, "80");
    std::cout << "Brick Game  HIGH SCORES\n";
    std::cout << "每游戏一条最高分，只显示积分\n";
    const int n = GetGameCount();
    for (int i = 0; i < n; ++i)
    {
        const GameEntry& game = GetGameAt(i);
        if (game.status != GameStatus::Implemented)
            continue;
        std::string left;
        if (game.id < 10)
            left.push_back(' ');
        left += std::to_string(game.id);
        left += '.';
        left += game.name;
        std::cout << left;
        for (int pad = Utf8DisplayWidth(left.c_str()); pad < 20; ++pad)
            std::cout << ' ';
        const Scoreboard board = LoadScoreboard(game.id);
        if (board.count == 0)
            std::cout << "--";
        else
            std::cout << board.scores[0];
        std::cout << '\n';
    }
    std::cout << "按回车返回\n";
    WaitEnter();
}

void WaitEnter()
{
    std::string dummy;
    std::getline(std::cin, dummy);
}

void TrimLine(std::string& line)
{
    while (!line.empty() && (line.back() == ' ' || line.back() == '\t' || line.back() == '\r'))
        line.pop_back();
    size_t start = 0;
    while (start < line.size() && (line[start] == ' ' || line[start] == '\t'))
        ++start;
    line.erase(0, start);
}

bool LaunchGame(const GameEntry& game)
{
    const std::wstring dir = ExeDir();
    const std::wstring exe = dir + L"\\" + AsciiToWide(game.exeFileName) + L".exe";
    SetMachineSpeed(GetMachineSpeed());

    PROCESS_INFORMATION process{};
    if (!SpawnGameInClassicHost(exe.c_str(), dir.c_str(), game.windowColumns, game.windowRows,
                               &process))
    {
        const DWORD err = GetLastError();
        std::cout << "无法启动 " << game.exeFileName << ".exe ("
                  << err << ")\n";
        std::cout << "按回车继续\n";
        WaitEnter();
        return false;
    }
    CloseHandle(process.hThread);
    WaitForSingleObject(process.hProcess, INFINITE);
    CloseHandle(process.hProcess);
    return true;
}
} // namespace

int main()
{
    SetMachineSpeed(5);
    SetupConsole();
    for (;;)
    {
        DrawMenu();
        std::string line;
        if (!std::getline(std::cin, line))
            break;
        TrimLine(line);
        if (line == "s" || line == "S")
        {
            SetMachineSpeed(GetMachineSpeed() % 10 + 1);
            continue;
        }
        if (line == "h" || line == "H")
        {
            DrawRanking();
            continue;
        }
        int id = 0;
        try
        {
            id = std::stoi(line);
        }
        catch (...)
        {
            std::cout << "请输入 1-" << GetGameCount() << "\n";
            WaitEnter();
            continue;
        }
        if (id == 0)
            break;
        const GameEntry* game = FindGameById(id);
        if (game == nullptr)
        {
            std::cout << "请输入 1-" << GetGameCount() << "\n";
            WaitEnter();
            continue;
        }
        if (!IsGameImplemented(id))
        {
            std::cout << "未实现\n"
                      << "按回车继续\n";
            WaitEnter();
            continue;
        }
        LaunchGame(*game);
    }
    return 0;
}
