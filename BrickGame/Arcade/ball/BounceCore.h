#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif
#ifndef NBALL
#define NBALL 3
#endif

int px, py, score;
int bx[8], by[8], dx[8], dy[8], n;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("BALL ") + std::to_string(score));
    FillStr(px, py, "@");
    for (int i = 0; i < n; ++i)
        FillStr(bx[i], by[i], "o");
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(6, 8, "Game Over");
    Pause();
    Run();
}

void Run()
{
    px = 10;
    py = 10;
    score = 0;
    n = NBALL;
    for (int i = 0; i < n; ++i)
    {
        bx[i] = 2 + i * 3;
        by[i] = 2 + (i & 3);
        dx[i] = (i & 1) ? 1 : -1;
        dy[i] = (i & 2) ? 1 : -1;
    }
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            ++score;
            for (int i = 0; i < n; ++i)
            {
                bx[i] += dx[i];
                by[i] += dy[i];
                if (bx[i] <= 1 || bx[i] >= 22)
                    dx[i] = -dx[i];
                if (by[i] <= 1 || by[i] >= 16)
                    dy[i] = -dy[i];
                for (int j = i + 1; j < n; ++j)
                    if (bx[i] == bx[j] && by[i] == by[j])
                    {
                        dx[i] = -dx[i];
                        dx[j] = -dx[j];
                    }
                if (bx[i] == px && by[i] == py)
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
                if (k == 75 && px > 1)
                    --px;
                if (k == 77 && px < 22)
                    ++px;
                if (k == 72 && py > 1)
                    --py;
                if (k == 80 && py < 16)
                    ++py;
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
