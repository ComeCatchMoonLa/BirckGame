#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif

const int W = 18, H = 16, LANES = 10;
int px, py, score, cx[LANES][6], cv[LANES], clen[LANES];
#ifdef MODE_RIVER
int isRiver[LANES];
#endif
#ifdef MODE_RETURN
int goingUp;
#endif
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("FROG ") + std::to_string(score));
    for (int y = 0; y < H; ++y)
        FillRec(0, y + 1, W, 1, " ");
    for (int L = 0; L < LANES; ++L)
    {
        int y = 2 + L;
#ifdef MODE_RIVER
        if (isRiver[L])
            FillRec(0, y, W, 1, "~");
#endif
        for (int i = 0; i < 6; ++i)
        {
            int x = cx[L][i];
            if (x < 0 || x >= W)
                continue;
#ifdef MODE_RIVER
            FillStr(x, y, isRiver[L] ? "=" : ">");
#else
            FillStr(x, y, ">");
#endif
        }
    }
    FillStr(px, py, "@");
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(6, 8, "Game Over");
    Pause();
    Run();
}

int OnLog()
{
#ifdef MODE_RIVER
    int L = py - 2;
    if (L < 0 || L >= LANES || !isRiver[L])
        return 1;
    for (int i = 0; i < 6; ++i)
        if (cx[L][i] == px)
            return 1;
    return 0;
#else
    return 1;
#endif
}

int HitCar()
{
    int L = py - 2;
    if (L < 0 || L >= LANES)
        return 0;
#ifdef MODE_RIVER
    if (isRiver[L])
        return 0;
#endif
    for (int i = 0; i < 6; ++i)
        if (cx[L][i] == px)
            return 1;
    return 0;
}

void StepCars()
{
    for (int L = 0; L < LANES; ++L)
        for (int i = 0; i < 6; ++i)
        {
            cx[L][i] += cv[L];
            if (cv[L] > 0 && cx[L][i] >= W)
                cx[L][i] = -1 - rand() % 4;
            if (cv[L] < 0 && cx[L][i] < -1)
                cx[L][i] = W + rand() % 4;
        }
#ifdef MODE_RIVER
    int L = py - 2;
    if (L >= 0 && L < LANES && isRiver[L])
        px += cv[L];
#endif
    if (px < 0 || px >= W || HitCar() || !OnLog())
        Over();
}

void WinTrip()
{
#ifdef MODE_RETURN
    if (goingUp && py == 1)
        goingUp = 0;
    else if (!goingUp && py == H)
    {
        goingUp = 1;
        ++score;
        px = W / 2;
    }
#else
    ++score;
    py = H;
    px = W / 2;
#endif
}

void Run()
{
    px = W / 2;
    py = H;
    score = 0;
#ifdef MODE_RETURN
    goingUp = 1;
#endif
    for (int L = 0; L < LANES; ++L)
    {
#ifdef MODE_TWOWAY
        cv[L] = (L & 1) ? 1 : -1;
#else
        cv[L] = 1;
#endif
#ifdef MODE_RIVER
        isRiver[L] = (L >= 6);
#endif
        for (int i = 0; i < 6; ++i)
            cx[L][i] = i * 4 + (L & 3);
    }
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            StepCars();
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
                if (k == 75 && px)
                    --px;
                if (k == 77 && px < W - 1)
                    ++px;
                if (k == 72 && py > 1)
                    --py;
                if (k == 80 && py < H)
                    ++py;
                if (py == 1 || py == H)
                    WinTrip();
                if (HitCar() || !OnLog())
                    Over();
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
