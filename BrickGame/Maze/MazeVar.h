#pragma once
#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif

const int ROW = 21, COL = 31;
int block[ROW][COL], vis[ROW][COL], px, py, steps, ticks;
const int kDx[4] = {0, 1, 0, -1};
const int kDy[4] = {-1, 0, 1, 0};
void Run();

void Dfs(int x, int y)
{
    int d = rand() & 3;
    for (int i = 0; i < 4; ++i)
    {
        int j = (d + i) & 3;
        int nx = x + kDx[j] * 2, ny = y + kDy[j] * 2;
        if (nx > 0 && ny > 0 && nx < COL - 1 && ny < ROW - 1 && block[ny][nx] == 1)
        {
            block[y + kDy[j]][x + kDx[j]] = 0;
            block[ny][nx] = 0;
            Dfs(nx, ny);
        }
    }
}

void Draw()
{
#ifdef MODE_TIMED
    FillStr(0, 0, std::string("MAZE T ") + std::to_string(200 - ticks) + " S " + std::to_string(steps));
#else
    FillStr(0, 0, std::string("MAZE S ") + std::to_string(steps));
#endif
    for (int y = 0; y < ROW; ++y)
        for (int x = 0; x < COL; ++x)
        {
#ifdef MODE_FOG
            int show = vis[y][x] || (x >= px - 1 && x <= px + 1 && y >= py - 1 && y <= py + 1);
            if (!show)
            {
                FillStr(x, y + 1, " ");
                continue;
            }
#endif
            const char* g = block[y][x] ? "■" : "  ";
            if (x == px && y == py)
                g = "@";
            if (x == COL - 2 && y == ROW - 2)
                g = "X";
            FillStr(x, y + 1, g);
        }
}

void Win()
{
    int s = 100000 - steps;
    if (s < 1)
        s = 1;
#ifdef MODE_TIMED
    s += (200 - ticks);
#endif
    SubmitScore(GAME_ID, s);
    FillStr(4, 10, "Clear");
    Pause();
    Run();
}

void Fail()
{
    SubmitScore(GAME_ID, 1);
    FillStr(4, 10, "Time up");
    Pause();
    Run();
}

void Run()
{
    ClearScreen();
    for (int y = 0; y < ROW; ++y)
        for (int x = 0; x < COL; ++x)
            block[y][x] = 1;
    std::memset(vis, 0, sizeof(vis));
    block[1][1] = 0;
    Dfs(1, 1);
    block[ROW - 2][COL - 2] = 0;
    px = py = 1;
    vis[py][px] = 1;
    steps = ticks = 0;
    Draw();
    while (true)
    {
        PumpFrame();
#ifdef MODE_TIMED
        if (DueLogicTick())
        {
            ++ticks;
            Draw();
            if (ticks >= 200)
                Fail();
        }
#endif
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
                int d = (k == 72) ? 0 : (k == 77) ? 1 : (k == 80) ? 2 : (k == 75) ? 3 : -1;
                if (d >= 0)
                {
                    int nx = px + kDx[d], ny = py + kDy[d];
                    if (nx >= 0 && ny >= 0 && nx < COL && ny < ROW && block[ny][nx] == 0)
                    {
                        px = nx;
                        py = ny;
                        vis[py][px] = 1;
                        ++steps;
                        if (px == COL - 2 && py == ROW - 2)
                            Win();
                    }
                    Draw();
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
