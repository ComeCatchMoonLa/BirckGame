#include "Scores.h"
#include "doctest.h"
#include <Windows.h>
#include <cstdio>
#include <string>

namespace
{
std::wstring TestPath()
{
    wchar_t dir[MAX_PATH];
    GetTempPathW(MAX_PATH, dir);
    return std::wstring(dir) + L"brick_scores_test.txt";
}

void BeginIsolated()
{
    const std::wstring path = TestPath();
    DeleteFileW(path.c_str());
    DeleteFileW((path + L".tmp").c_str());
    RemoveDirectoryW((path + L".tmp").c_str());
    SetScoreSavePathForTest(path.c_str());
}

void EndIsolated()
{
    const std::wstring path = TestPath();
    DeleteFileW(path.c_str());
    DeleteFileW((path + L".tmp").c_str());
    RemoveDirectoryW((path + L".tmp").c_str());
    SetScoreSavePathForTest(nullptr);
    ResetScoreStateForTest();
}
} // namespace

TEST_CASE("empty board before any submit")
{
    BeginIsolated();
    const Scoreboard board = LoadScoreboard(1);
    CHECK(board.count == 0);
    CHECK(LoadScoreboard(0).count == 0);
    EndIsolated();
}

TEST_CASE("submit keeps a single high score")
{
    BeginIsolated();
    CHECK(SubmitScore(1, 30));
    CHECK(SubmitScore(1, 100));
    CHECK_FALSE(SubmitScore(1, 50));
    CHECK_FALSE(SubmitScore(1, 100));
    const Scoreboard board = LoadScoreboard(1);
    REQUIRE(board.count == 1);
    CHECK(board.scores[0] == 100);
    EndIsolated();
}

TEST_CASE("unimplement or invalid id is rejected")
{
    BeginIsolated();
    CHECK_FALSE(SubmitScore(8, 99));
    CHECK_FALSE(SubmitScore(0, 1));
    CHECK_FALSE(SubmitScore(99, 1));
    CHECK(LoadScoreboard(8).count == 0);
    EndIsolated();
}

TEST_CASE("negative score stores as zero and persists")
{
    BeginIsolated();
    CHECK(SubmitScore(7, -4));
    ResetScoreStateForTest();
    const Scoreboard board = LoadScoreboard(7);
    REQUIRE(board.count == 1);
    CHECK(board.scores[0] == 0);
    CHECK(LoadScoreboard(1).count == 0);
    EndIsolated();
}

TEST_CASE("load rereads file written by another process")
{
    BeginIsolated();
    CHECK(SubmitScore(2, 10));
    REQUIRE(LoadScoreboard(2).scores[0] == 10);
    const std::wstring path = TestPath();
    FILE* file = _wfopen(path.c_str(), L"wb");
    REQUIRE(file != nullptr);
    const char* text = "# brick-scores 1\n2 30\n";
    const size_t n = std::char_traits<char>::length(text);
    CHECK(fwrite(text, 1, n, file) == n);
    CHECK(fclose(file) == 0);
    const Scoreboard board = LoadScoreboard(2);
    REQUIRE(board.count == 1);
    CHECK(board.scores[0] == 30);
    EndIsolated();
}

TEST_CASE("submit rolls back memory when write fails")
{
    BeginIsolated();
    CHECK(SubmitScore(1, 10));
    const std::wstring tmp = TestPath() + L".tmp";
    REQUIRE(CreateDirectoryW(tmp.c_str(), nullptr) != 0);
    CHECK_FALSE(SubmitScore(1, 99));
    RemoveDirectoryW(tmp.c_str());
    CHECK(SubmitScore(1, 50));
    const Scoreboard board = LoadScoreboard(1);
    REQUIRE(board.count == 1);
    CHECK(board.scores[0] == 50);
    EndIsolated();
}
