#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif

const int COLS = 8, ROWS = 3, W = 22, H = 18;
int alive[ROWS][COLS], ex, ey, edir, px, score, bx, by, lives;
#ifdef MODE_UFO
int ufo, ufox;
#endif
#ifdef MODE_PLANE
int eyx[6], eyy[6], nplane;
#endif
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("SHOT ") + std::to_string(score) + " L" + std::to_string(lives));
#ifdef MODE_PLANE
    for (int i = 0; i < nplane; ++i)
        FillStr(eyx[i], eyy[i], "v");
    FillStr(px, 8, ">A<");
#else
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            if (alive[r][c])
                FillStr(ex + c * 2, ey + r, "W");
#ifdef MODE_UFO
    if (ufo)
        FillStr(ufox, 1, "<U>");
#endif
    FillStr(px, H - 1, "A");
#endif
    if (bx >= 0)
        FillStr(bx, by, "|");
}

void Over()
{
    SubmitScore(GAME_ID, score);
    FillStr(6, 8, "Game Over");
    Pause();
    Run();
}

int Remain()
{
#ifdef MODE_PLANE
    return nplane;
#else
    int n = 0;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            n += alive[r][c];
    return n;
#endif
}

void Wave()
{
#ifdef MODE_PLANE
    nplane = 6;
    for (int i = 0; i < 6; ++i)
    {
        eyx[i] = 2 + i * 3;
        eyy[i] = 1 + (i & 1);
    }
#else
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            alive[r][c] = 1;
    ex = 2;
    ey = 2;
    edir = 1;
#endif
}

void Step()
{
#ifdef MODE_PLANE
    for (int i = 0; i < nplane; ++i)
    {
        ++eyy[i];
        if ((rand() % 3) == 0)
            eyx[i] += (rand() & 1) ? 1 : -1;
        if (eyx[i] < 1)
            eyx[i] = 1;
        if (eyx[i] > W - 2)
            eyx[i] = W - 2;
        if (eyy[i] >= 8 && eyx[i] >= px && eyx[i] <= px + 2)
            Over();
        if (eyy[i] > 16)
        {
            eyy[i] = 1;
            eyx[i] = 1 + rand() % (W - 2);
        }
    }
#else
    ex += edir;
    if (ex < 1 || ex + COLS * 2 > W)
    {
        edir = -edir;
        ++ey;
    }
    if (ey + ROWS >= H - 1)
        Over();
#ifdef MODE_UFO
    if (ufo)
    {
        ++ufox;
        if (ufox > W)
            ufo = 0;
    }
    else if ((rand() % 20) == 0)
    {
        ufo = 1;
        ufox = 0;
    }
#endif
#endif
    if (bx >= 0)
    {
        --by;
        if (by < 1)
            bx = -1;
        else
        {
#ifdef MODE_PLANE
            for (int i = 0; i < nplane; ++i)
                if (bx == eyx[i] && by == eyy[i])
                {
                    eyx[i] = eyx[nplane - 1];
                    eyy[i] = eyy[nplane - 1];
                    --nplane;
                    ++score;
                    bx = -1;
                    break;
                }
#else
#ifdef MODE_UFO
            if (ufo && by == 1 && bx >= ufox && bx <= ufox + 2)
            {
                ufo = 0;
                score += 50;
                bx = -1;
            }
#endif
            for (int r = 0; r < ROWS; ++r)
                for (int c = 0; c < COLS; ++c)
                    if (alive[r][c] && bx == ex + c * 2 && by == ey + r)
                    {
                        alive[r][c] = 0;
                        score += 10;
                        bx = -1;
                    }
#endif
        }
    }
    if (Remain() <= 0)
    {
        score += 30;
        Wave();
    }
}

void Run()
{
    px = W / 2;
    score = 0;
    lives = 3;
    bx = -1;
#ifdef MODE_UFO
    ufo = 0;
#endif
    Wave();
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
            else if (ch == 'z' || ch == 'Z')
            {
                if (bx < 0)
                {
#ifdef MODE_PLANE
                    bx = px + 1;
                    by = 7;
#else
                    bx = px;
                    by = H - 2;
#endif
                }
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && px > 1)
                    --px;
                if (k == 77 && px < W - 2)
                    ++px;
#ifdef MODE_PLANE
                if (k == 72 && px)
                    ;
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
