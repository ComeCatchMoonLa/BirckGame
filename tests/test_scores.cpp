#include "Scores.h"
#include "doctest.h"
#include <Windows.h>
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
    SetScoreSavePathForTest(path.c_str());
}

void EndIsolated()
{
    const std::wstring path = TestPath();
    DeleteFileW(path.c_str());
    DeleteFileW((path + L".tmp").c_str());
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
