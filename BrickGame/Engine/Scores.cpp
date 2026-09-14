#include "Scores.h"
#include "GameRegistry.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

namespace
{
constexpr int kMaxGameId = 88;
Scoreboard g_boards[kMaxGameId + 1];
bool g_loaded = false;
std::wstring g_overridePath;

void ClearBoards()
{
    for (int i = 0; i <= kMaxGameId; ++i)
    {
        g_boards[i].count = 0;
        for (int s = 0; s < kTopScoreSlots; ++s)
            g_boards[i].scores[s] = 0;
    }
}

std::wstring DefaultPath()
{
    wchar_t buf[MAX_PATH];
    const DWORD n = GetModuleFileNameW(nullptr, buf, MAX_PATH);
    if (n == 0 || n >= MAX_PATH)
        return L"brick_scores.txt";
    std::wstring path(buf, n);
    const size_t slash = path.find_last_of(L"\\/");
    if (slash == std::wstring::npos)
        return L"brick_scores.txt";
    return path.substr(0, slash + 1) + L"brick_scores.txt";
}

std::wstring Path()
{
    return g_overridePath.empty() ? DefaultPath() : g_overridePath;
}

bool ReadAll(const std::wstring& path, std::string* out)
{
    FILE* file = _wfopen(path.c_str(), L"rb");
    if (file == nullptr)
        return false;
    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return false;
    }
    const long size = ftell(file);
    if (size < 0)
    {
        fclose(file);
        return false;
    }
    rewind(file);
    if (size == 0)
    {
        fclose(file);
        out->clear();
        return true;
    }
    std::vector<char> buf(static_cast<size_t>(size));
    const size_t got = fread(buf.data(), 1, buf.size(), file);
    fclose(file);
    out->assign(buf.data(), got);
    return true;
}

bool WriteAtomic(const std::wstring& path, const std::string& data)
{
    const std::wstring tmp = path + L".tmp";
    FILE* file = _wfopen(tmp.c_str(), L"wb");
    if (file == nullptr)
        return false;
    const size_t wrote = fwrite(data.data(), 1, data.size(), file);
    const int closed = fclose(file);
    if (wrote != data.size() || closed != 0)
    {
        DeleteFileW(tmp.c_str());
        return false;
    }
    if (MoveFileExW(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING) != 0)
        return true;
    DeleteFileW(tmp.c_str());
    return false;
}

void ParseLine(const std::string& line)
{
    if (line.empty() || line[0] == '#')
        return;
    std::istringstream in(line);
    int id = 0;
    if (!(in >> id) || id < 1 || id > kMaxGameId)
        return;
    Scoreboard board{};
    int value = 0;
    while (board.count < kTopScoreSlots && (in >> value))
    {
        if (value < 0)
            value = 0;
        board.scores[board.count] = value;
        ++board.count;
    }
    for (int i = 1; i < board.count; ++i)
    {
        if (board.scores[i] > board.scores[i - 1])
            return;
    }
    g_boards[id] = board;
}

void Parse(const std::string& text)
{
    std::string line;
    for (char ch : text)
    {
        if (ch == '\n')
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            ParseLine(line);
            line.clear();
        }
        else
            line.push_back(ch);
    }
    if (!line.empty())
        ParseLine(line);
}

std::string Serialize()
{
    std::string out = "# brick-scores 1\n";
    for (int id = 1; id <= kMaxGameId; ++id)
    {
        const Scoreboard& board = g_boards[id];
        if (board.count <= 0)
            continue;
        out += std::to_string(id);
        for (int i = 0; i < board.count; ++i)
        {
            out.push_back(' ');
            out += std::to_string(board.scores[i]);
        }
        out.push_back('\n');
    }
    return out;
}

void EnsureLoaded()
{
    if (g_loaded)
        return;
    ClearBoards();
    std::string text;
    if (ReadAll(Path(), &text))
        Parse(text);
    g_loaded = true;
}

bool InsertDescending(Scoreboard& board, int score)
{
    if (board.count == 1 && score <= board.scores[0])
        return false;
    board.scores[0] = score;
    board.count = 1;
    return true;
}
} // namespace

Scoreboard LoadScoreboard(int gameId)
{
    // 游戏是另一进程写文件；启动器每次 h 必须重新读盘，不能沿用本进程缓存。
    g_loaded = false;
    EnsureLoaded();
    if (gameId < 1 || gameId > kMaxGameId)
        return {};
    return g_boards[gameId];
}

bool SubmitScore(int gameId, int score)
{
    if (!IsGameImplemented(gameId))
        return false;
    if (score < 0)
        score = 0;
    EnsureLoaded();
    if (!InsertDescending(g_boards[gameId], score))
        return false;
    WriteAtomic(Path(), Serialize());
    return true;
}

void SetScoreSavePathForTest(const wchar_t* path)
{
    g_overridePath = path != nullptr ? path : L"";
    g_loaded = false;
    ClearBoards();
}

void ResetScoreStateForTest()
{
    g_loaded = false;
    ClearBoards();
}
