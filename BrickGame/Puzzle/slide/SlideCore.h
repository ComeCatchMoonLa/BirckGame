#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif
#ifndef SLIDE_N
#define SLIDE_N 3
#endif

int tile[SLIDE_N][SLIDE_N], rot[SLIDE_N][SLIDE_N], ex, ey, cx, cy, moves;
void Run();

int Solved()
{
    int k = 1;
    for (int y = 0; y < SLIDE_N; ++y)
        for (int x = 0; x < SLIDE_N; ++x)
        {
            if (x == SLIDE_N - 1 && y == SLIDE_N - 1)
                return tile[y][x] == 0;
            if (tile[y][x] != k)
                return 0;
#ifdef MODE_ROT
            if (rot[y][x] != 0)
                return 0;
#endif
            ++k;
        }
    return 1;
}

void Draw()
{
    FillStr(0, 0, std::string("SLIDE ") + std::to_string(moves));
    for (int y = 0; y < SLIDE_N; ++y)
        for (int x = 0; x < SLIDE_N; ++x)
        {
            std::string s = tile[y][x] ? std::to_string(tile[y][x]) : ".";
#ifdef MODE_ROT
            if (tile[y][x])
                s += std::string(1, char(" ^>v<"[rot[y][x] & 3]));
#endif
            if (x == cx && y == cy)
                s = "[" + s + "]";
            else
                s = " " + s + " ";
            FillStr(x * 5, y * 2 + 2, s + "  ");
        }
}

void Win()
{
    int s = 100000 - moves;
    if (s < 1)
        s = 1;
    SubmitScore(GAME_ID, s);
    FillStr(0, SLIDE_N * 2 + 3, "Clear");
    Pause();
    Run();
}

void SwapEmpty(int x, int y)
{
    if (x < 0 || y < 0 || x >= SLIDE_N || y >= SLIDE_N)
        return;
    if (abs(x - ex) + abs(y - ey) != 1)
        return;
    tile[ey][ex] = tile[y][x];
    rot[ey][ex] = rot[y][x];
    tile[y][x] = 0;
    rot[y][x] = 0;
    ex = x;
    ey = y;
    ++moves;
}

void Shuffle()
{
    int k = 1;
    for (int y = 0; y < SLIDE_N; ++y)
        for (int x = 0; x < SLIDE_N; ++x)
        {
            tile[y][x] = k++;
            rot[y][x] = 0;
        }
    tile[SLIDE_N - 1][SLIDE_N - 1] = 0;
    ex = ey = SLIDE_N - 1;
    for (int i = 0; i < SLIDE_N * SLIDE_N * 20; ++i)
    {
        int d = rand() % 4;
        int nx = ex + (d == 1) - (d == 3);
        int ny = ey + (d == 2) - (d == 0);
        int ox = ex, oy = ey;
        SwapEmpty(nx, ny);
        moves = 0;
        (void)ox;
        (void)oy;
    }
#ifdef MODE_ROT
    for (int y = 0; y < SLIDE_N; ++y)
        for (int x = 0; x < SLIDE_N; ++x)
            if (tile[y][x])
                rot[y][x] = rand() & 3;
#endif
}

void Run()
{
    ClearScreen();
    cx = cy = 0;
    moves = 0;
    Shuffle();
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
            {
#ifdef MODE_ROT
                if (tile[cy][cx])
                {
                    rot[cy][cx] = (rot[cy][cx] + 1) & 3;
                    ++moves;
                    Draw();
                    if (Solved())
                        Win();
                }
#else
                Pause();
#endif
            }
            else if (ch == 'z' || ch == 'Z')
            {
                SwapEmpty(cx, cy);
                Draw();
                if (Solved())
                    Win();
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < SLIDE_N - 1)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < SLIDE_N - 1)
                    ++cy;
#ifndef MODE_ROT
                SwapEmpty(cx, cy);
                cx = ex;
                cy = ey;
                if (Solved())
                    Win();
#endif
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
