#pragma once
#include "../../Engine/BrickEngine.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <vector>

#ifndef GAME_ID
#error
#endif

const int N = 4;
int face[N * N], up[N * N], done[N * N];
int c1, c2, first, moves, ticks, matched;
void Run();

void Draw()
{
    FillStr(0, 0, std::string("MOVES ") + std::to_string(moves) + "  ");
#ifdef MODE_TIMED
    FillStr(14, 0, std::string("TIME ") + std::to_string(120 - ticks) + "  ");
#endif
    for (int i = 0; i < N * N; ++i)
    {
        int x = (i % N) * 4, y = 2 + (i / N) * 2;
        std::string s = done[i] || up[i] ? std::string(1, char('A' + face[i])) : "?";
        if (i == c1)
            s = "[" + s + "]";
        else
            s = " " + s + " ";
        FillStr(x, y, s);
    }
}

void Win()
{
    int stored = 100000 - moves;
    if (stored < 1)
        stored = 1;
    SubmitScore(GAME_ID, stored);
    FillStr(0, 12, "Clear");
    Pause();
    Run();
}

void FailTime()
{
    SubmitScore(GAME_ID, matched);
    FillStr(0, 12, "Time up");
    Pause();
    Run();
}

void Run()
{
    ClearScreen();
    std::vector<int> bag;
    for (int i = 0; i < N * N / 2; ++i)
    {
        bag.push_back(i);
        bag.push_back(i);
    }
    std::mt19937 rng((unsigned)time(0));
    std::shuffle(bag.begin(), bag.end(), rng);
    for (int i = 0; i < N * N; ++i)
    {
        face[i] = bag[i];
        up[i] = done[i] = 0;
    }
    c1 = 0;
    c2 = -1;
    first = -1;
    moves = ticks = matched = 0;
    Draw();
    while (true)
    {
        PumpFrame();
#ifdef MODE_TIMED
        if (DueLogicTick())
        {
            ++ticks;
            Draw();
            if (ticks >= 120)
                FailTime();
        }
#endif
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
            {
                if (done[c1] || up[c1])
                    ;
                else if (first < 0)
                {
                    first = c1;
                    up[c1] = 1;
                    Draw();
                }
                else if (c1 != first)
                {
                    up[c1] = 1;
                    ++moves;
                    Draw();
                    if (face[c1] == face[first])
                    {
                        done[c1] = done[first] = 1;
                        ++matched;
                        first = -1;
                        if (matched == N * N / 2)
                            Win();
                    }
                    else
                    {
                        Pause();
                        up[c1] = up[first] = 0;
                        first = -1;
                        Draw();
                    }
                }
            }
            else if (ch == 224)
            {
                int k = getch();
                int x = c1 % N, y = c1 / N;
                if (k == 75 && x)
                    --x;
                if (k == 77 && x < N - 1)
                    ++x;
                if (k == 72 && y)
                    --y;
                if (k == 80 && y < N - 1)
                    ++y;
                c1 = y * N + x;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(GAME_ID);
    srand((unsigned)time(0));
    Run();
}
