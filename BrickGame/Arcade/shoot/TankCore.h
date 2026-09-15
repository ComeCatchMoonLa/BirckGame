#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif

const int W = 17, H = 15;
int map[H][W], px, py, pd, ex, ey, ed, score, bx, by, bd, alive;
void Run();

int Walk(int t) { return t == 0 || t == 2; }

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("TANK ") + std::to_string(score) + " Z射击");
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
        {
            const char* g = "  ";
            if (map[y][x] == 1)
                g = "##";
            else if (map[y][x] == 2)
                g = "[]";
            FillStr(x, y + 1, g);
        }
    const char* dg[] = {"^", ">", "v", "<"};
    FillStr(px, py + 1, dg[pd]);
    if (alive)
        FillStr(ex, ey + 1, "E");
    if (bx >= 0)
        FillStr(bx, by + 1, "*");
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(4, 8, "Game Over");
    Pause();
    Run();
}

void Build()
{
    std::memset(map, 0, sizeof(map));
    for (int x = 0; x < W; ++x)
        map[0][x] = map[H - 1][x] = 2;
    for (int y = 0; y < H; ++y)
        map[y][0] = map[y][W - 1] = 2;
    for (int y = 2; y < H - 2; y += 2)
        for (int x = 2; x < W - 2; x += 2)
            map[y][x] = (rand() & 1) ? 1 : 2;
#ifdef MODE_MAZE
    for (int y = 1; y < H - 1; ++y)
        for (int x = 1; x < W - 1; ++x)
            if ((x & 1) == 0 && (y & 1) == 0)
                map[y][x] = 2;
            else if ((x + y) % 5 == 0)
                map[y][x] = 2;
            else
                map[y][x] = 0;
    map[1][1] = map[H - 2][W - 2] = 0;
#endif
}

int Can(int x, int y) { return x >= 0 && y >= 0 && x < W && y < H && Walk(map[y][x]); }

void Fire(int x, int y, int d)
{
    if (bx >= 0)
        return;
    bx = x;
    by = y;
    bd = d;
}

void StepShot()
{
    if (bx < 0)
        return;
    const int kDx[4] = {0, 1, 0, -1};
    const int kDy[4] = {-1, 0, 1, 0};
    bx += kDx[bd];
    by += kDy[bd];
    if (bx < 0 || by < 0 || bx >= W || by >= H)
    {
        bx = -1;
        return;
    }
    if (map[by][bx] == 1)
    {
        map[by][bx] = 0;
        bx = -1;
        return;
    }
    if (map[by][bx] == 2)
    {
        bx = -1;
        return;
    }
    if (alive && bx == ex && by == ey)
    {
        alive = 0;
        score += 20;
        bx = -1;
        ex = W - 3;
        ey = 2;
        alive = 1;
    }
    if (bx == px && by == py)
        Over();
}

void Run()
{
    Build();
    px = py = 1;
    pd = 1;
    ex = W - 3;
    ey = H - 3;
    ed = 3;
    score = 0;
    bx = -1;
    alive = 1;
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            int nd = rand() % 4;
            const int kDx[4] = {0, 1, 0, -1};
            const int kDy[4] = {-1, 0, 1, 0};
            if (Can(ex + kDx[nd], ey + kDy[nd]))
            {
                ex += kDx[nd];
                ey += kDy[nd];
                ed = nd;
            }
            if ((rand() % 6) == 0)
                Fire(ex, ey, ed);
            StepShot();
            Draw();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch == 'z' || ch == 'Z')
                Fire(px, py, pd);
            else if (ch == 224)
            {
                int k = getch();
                int nd = (k == 72) ? 0 : (k == 77) ? 1 : (k == 80) ? 2 : (k == 75) ? 3 : pd;
                pd = nd;
                const int kDx[4] = {0, 1, 0, -1};
                const int kDy[4] = {-1, 0, 1, 0};
                if (Can(px + kDx[pd], py + kDy[pd]))
                {
                    px += kDx[pd];
                    py += kDy[pd];
                }
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
