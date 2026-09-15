#pragma once
#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif
#ifndef NGHOST
#define NGHOST 1
#endif

const int W = 19, H = 11;
const char* kMap[11] = {
    "###################",
    "#........#........#",
    "#.##.###.#.###.##.#",
    "#.................#",
    "#.##.#.#####.#.##.#",
    "#....#...#...#....#",
    "#.##.###.#.###.##.#",
    "#.##.....#.....##.#",
    "#.##.###.#.###.##.#",
    "#.................#",
    "###################",
};
int wall[H][W], dot[H][W], px, py, gx[4], gy[4], dots, score;
void Run();

int Abs(int v) { return v < 0 ? -v : v; }

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("PAC ") + std::to_string(score));
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
        {
            const char* g = wall[y][x] ? "#" : (dot[y][x] ? "." : " ");
            FillStr(x, y + 1, g);
        }
    FillStr(px, py + 1, "C");
    for (int i = 0; i < NGHOST; ++i)
        FillStr(gx[i], gy[i] + 1, "M");
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(4, 6, "Game Over");
    Pause();
    Run();
}

void Win()
{
    SubmitScore(GAME_ID, score + 100);
    FillStr(4, 6, "Clear");
    Pause();
    Run();
}

void Chase(int i)
{
    int dx = (px > gx[i]) - (px < gx[i]);
    int dy = (py > gy[i]) - (py < gy[i]);
#ifdef MODE_FAST
    int tries[2][2] = {{dx, 0}, {0, dy}};
#else
    int tries[2][2] = {{dx, 0}, {0, dy}};
#endif
    if (Abs(px - gx[i]) < Abs(py - gy[i]))
    {
        tries[0][0] = 0;
        tries[0][1] = dy;
        tries[1][0] = dx;
        tries[1][1] = 0;
    }
    for (int t = 0; t < 2; ++t)
    {
        int nx = gx[i] + tries[t][0], ny = gy[i] + tries[t][1];
        if (nx >= 0 && ny >= 0 && nx < W && ny < H && !wall[ny][nx])
        {
            gx[i] = nx;
            gy[i] = ny;
            return;
        }
    }
}

void Run()
{
    dots = 0;
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
        {
            wall[y][x] = kMap[y][x] == '#';
            dot[y][x] = kMap[y][x] == '.';
            dots += dot[y][x];
        }
    px = 1;
    py = 1;
    score = 0;
    for (int i = 0; i < NGHOST; ++i)
    {
        gx[i] = W - 3 - i;
        gy[i] = H - 2;
    }
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            for (int i = 0; i < NGHOST; ++i)
            {
                Chase(i);
                if (gx[i] == px && gy[i] == py)
                    Over();
            }
            Draw();
        }
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
                int nx = px, ny = py;
                if (k == 75)
                    --nx;
                if (k == 77)
                    ++nx;
                if (k == 72)
                    --ny;
                if (k == 80)
                    ++ny;
                if (nx >= 0 && ny >= 0 && nx < W && ny < H && !wall[ny][nx])
                {
                    px = nx;
                    py = ny;
                    if (dot[py][px])
                    {
                        dot[py][px] = 0;
                        --dots;
                        score += 10;
                        if (dots <= 0)
                            Win();
                    }
                    for (int i = 0; i < NGHOST; ++i)
                        if (gx[i] == px && gy[i] == py)
                            Over();
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
