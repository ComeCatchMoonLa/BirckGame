#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error GAME_ID
#endif
#ifndef GRID_N
#define GRID_N 4
#endif

int g[GRID_N][GRID_N];
int score;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(1, 0, std::string("2048  SCORE ") + std::to_string(score));
    for (int y = 0; y < GRID_N; ++y)
        for (int x = 0; x < GRID_N; ++x)
        {
            std::string s = g[y][x] ? std::to_string(g[y][x]) : ".";
            while (s.size() < 4)
                s = " " + s;
            FillStr(1 + x * 5, 2 + y * 2, s);
        }
}

void Spawn()
{
    int empty = 0;
    for (int y = 0; y < GRID_N; ++y)
        for (int x = 0; x < GRID_N; ++x)
            if (!g[y][x])
                ++empty;
    if (!empty)
        return;
    int k = rand() % empty;
    for (int y = 0; y < GRID_N; ++y)
        for (int x = 0; x < GRID_N; ++x)
            if (!g[y][x] && k-- == 0)
            {
                g[y][x] = (rand() % 10 == 0) ? 4 : 2;
                return;
            }
}

bool SlideRow(int* row)
{
    int tmp[GRID_N] = {}, w = 0;
    bool moved = false, merged[GRID_N] = {};
    for (int i = 0; i < GRID_N; ++i)
    {
        if (!row[i])
            continue;
        if (w > 0 && tmp[w - 1] == row[i] && !merged[w - 1])
        {
            tmp[w - 1] *= 2;
            score += tmp[w - 1];
            merged[w - 1] = true;
            moved = true;
        }
        else
        {
            if (w != i)
                moved = true;
            tmp[w++] = row[i];
        }
    }
    for (int i = 0; i < GRID_N; ++i)
        row[i] = tmp[i];
    return moved;
}

bool Move(int dir)
{
    bool moved = false;
    if (dir == 0 || dir == 2)
    {
        for (int x = 0; x < GRID_N; ++x)
        {
            int col[GRID_N];
            for (int y = 0; y < GRID_N; ++y)
                col[y] = (dir == 0) ? g[y][x] : g[GRID_N - 1 - y][x];
            if (SlideRow(col))
                moved = true;
            for (int y = 0; y < GRID_N; ++y)
                g[(dir == 0) ? y : GRID_N - 1 - y][x] = col[y];
        }
    }
    else
    {
        for (int y = 0; y < GRID_N; ++y)
        {
            int row[GRID_N];
            for (int x = 0; x < GRID_N; ++x)
                row[x] = (dir == 3) ? g[y][x] : g[y][GRID_N - 1 - x];
            if (SlideRow(row))
                moved = true;
            for (int x = 0; x < GRID_N; ++x)
                g[y][(dir == 3) ? x : GRID_N - 1 - x] = row[x];
        }
    }
    return moved;
}

bool CanMove()
{
    for (int y = 0; y < GRID_N; ++y)
        for (int x = 0; x < GRID_N; ++x)
        {
            if (!g[y][x])
                return true;
            if (x + 1 < GRID_N && g[y][x] == g[y][x + 1])
                return true;
            if (y + 1 < GRID_N && g[y][x] == g[y + 1][x])
                return true;
        }
    return false;
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(1, 2 + GRID_N * 2, "Game Over");
    Pause();
    Run();
}

void Run()
{
    for (int y = 0; y < GRID_N; ++y)
        for (int x = 0; x < GRID_N; ++x)
            g[y][x] = 0;
    score = 0;
    Spawn();
    Spawn();
    Draw();
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch == 224)
            {
                int k = getch();
                int dir = (k == 72) ? 0 : (k == 80) ? 2 : (k == 75) ? 3 : (k == 77) ? 1 : -1;
                if (dir >= 0 && Move(dir))
                {
                    Spawn();
                    Draw();
                    if (!CanMove())
                        Over();
                }
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
