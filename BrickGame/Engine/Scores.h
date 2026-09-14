#pragma once

constexpr int kTopScoreSlots = 1;

struct Scoreboard
{
    int scores[kTopScoreSlots];
    int count;
};

Scoreboard LoadScoreboard(int gameId);
bool SubmitScore(int gameId, int score);
void SetScoreSavePathForTest(const wchar_t* path);
void ResetScoreStateForTest();
