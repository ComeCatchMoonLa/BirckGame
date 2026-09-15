#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif

const int W = 20, H = 18;
int p1, p2, miss, score;
int fx[8], fy[8], fd[8], kind[8], nitem;
void Run();

void SpawnItem()
{
    if (nitem >= 8)
        return;
    int i = nitem++;
    fx[i] = 1 + rand() % (W - 2);
    fy[i] = 1;
#ifdef MODE_BOMB
    kind[i] = (rand() % 6 == 0) ? 1 : 0;
#else
    kind[i] = 0;
#endif
#ifdef MODE_DRIFT
    fd[i] = (rand() & 1) ? 1 : -1;
#else
    fd[i] = 0;
#endif
}

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("CATCH ") + std::to_string(score) + " MISS " + std::to_string(miss));
#ifdef MODE_DUAL
    FillStr(0, H + 1, "P1方向 P2 WASD");
#endif
    FillStr(p1, H - 1, "U");
#ifdef MODE_DUAL
    FillStr(p2, H - 1, "Y");
#endif
    for (int i = 0; i < nitem; ++i)
        FillStr(fx[i], fy[i], kind[i] ? "*" : "o");
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(6, 8, "Game Over");
    Pause();
    Run();
}

bool HitPaddle(int x)
{
#ifdef MODE_DUAL
    return x == p1 || x == p2;
#else
    return x == p1;
#endif
}

void Step()
{
    if ((rand() % 3) == 0)
        SpawnItem();
    for (int i = 0; i < nitem;)
    {
        fx[i] += fd[i];
        if (fx[i] < 1)
        {
            fx[i] = 1;
            fd[i] = 1;
        }
        if (fx[i] > W - 2)
        {
            fx[i] = W - 2;
            fd[i] = -1;
        }
        ++fy[i];
        if (fy[i] >= H - 1)
        {
            if (HitPaddle(fx[i]))
            {
                if (kind[i])
                    Over();
                else
                    ++score;
            }
            else if (!kind[i])
            {
                ++miss;
                if (miss >= 5)
                    Over();
            }
            fx[i] = fx[nitem - 1];
            fy[i] = fy[nitem - 1];
            fd[i] = fd[nitem - 1];
            kind[i] = kind[nitem - 1];
            --nitem;
        }
        else
            ++i;
    }
}

void Run()
{
    p1 = W / 2;
    p2 = W / 3;
    miss = score = nitem = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            Step();
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
                if (k == 75 && p1 > 1)
                    --p1;
                if (k == 77 && p1 < W - 2)
                    ++p1;
            }
#ifdef MODE_DUAL
            else if ((ch == 'a' || ch == 'A') && p2 > 1)
                --p2;
            else if ((ch == 'd' || ch == 'D') && p2 < W - 2)
                ++p2;
#endif
        }
    }
}

int main()
{
    SetConsoleFromGameId(GAME_ID);
    srand((unsigned)time(0));
    Run();
}
