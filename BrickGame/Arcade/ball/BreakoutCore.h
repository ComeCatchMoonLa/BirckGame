#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif
#ifndef NBALL
#define NBALL 1
#endif

const int W = 16, H = 18, BW = 14, BH = 4;
int brick[BH][BW], px, lives, score, laser;
int bx[4], by[4], dx[4], dy[4], alive[4], nball;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("BRICK ") + std::to_string(score) + " L" + std::to_string(lives));
    for (int y = 0; y < BH; ++y)
        for (int x = 0; x < BW; ++x)
            if (brick[y][x])
                FillStr(1 + x, 2 + y, "#");
    FillStr(px, H - 1, "===");
    for (int i = 0; i < nball; ++i)
        if (alive[i])
            FillStr(bx[i], by[i], "o");
#ifdef MODE_LASER
    if (laser)
        FillRec(px + 1, 2, 1, H - 3, "|");
#endif
}

void Over(bool win)
{
    SubmitScore(GAME_ID, score);
    FillStr(4, 10, win ? "Clear" : "Game Over");
    Pause();
    Run();
}

int Remain()
{
    int c = 0;
    for (int y = 0; y < BH; ++y)
        for (int x = 0; x < BW; ++x)
            c += brick[y][x];
    return c;
}

void HitBrick(int x, int y)
{
    int gx = x - 1, gy = y - 2;
    if (gx >= 0 && gx < BW && gy >= 0 && gy < BH && brick[gy][gx])
    {
        brick[gy][gx] = 0;
        score += 10;
#ifdef MODE_MULTI
        if (nball < 3)
        {
            bx[nball] = x;
            by[nball] = y + 1;
            dx[nball] = -dx[0];
            dy[nball] = 1;
            alive[nball] = 1;
            ++nball;
        }
#endif
    }
}

void StepBall(int i)
{
    if (!alive[i])
        return;
    bx[i] += dx[i];
    by[i] += dy[i];
    if (bx[i] <= 1 || bx[i] >= W)
        dx[i] = -dx[i];
    if (by[i] <= 1)
        dy[i] = -dy[i];
    HitBrick(bx[i], by[i]);
    if (by[i] >= H - 1)
    {
        if (bx[i] >= px && bx[i] <= px + 2)
            dy[i] = -1;
        else
        {
            alive[i] = 0;
            int any = 0;
            for (int j = 0; j < nball; ++j)
                any |= alive[j];
            if (!any)
            {
                --lives;
                if (lives <= 0)
                    Over(false);
                alive[0] = 1;
                bx[0] = px + 1;
                by[0] = H - 3;
                dx[0] = 1;
                dy[0] = -1;
                nball = 1;
            }
        }
    }
}

void Run()
{
    std::memset(brick, 0, sizeof(brick));
    for (int y = 0; y < BH; ++y)
        for (int x = 0; x < BW; ++x)
            brick[y][x] = 1;
    px = 6;
    lives = 3;
    score = laser = 0;
    nball = NBALL;
    for (int i = 0; i < nball; ++i)
    {
        alive[i] = 1;
        bx[i] = 4 + i * 3;
        by[i] = 8;
        dx[i] = (i & 1) ? 1 : -1;
        dy[i] = -1;
    }
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
#ifdef MODE_LASER
            if (laser)
            {
                for (int y = 2; y < H - 1; ++y)
                    HitBrick(px + 1, y);
                laser = 0;
            }
#endif
            for (int i = 0; i < nball; ++i)
                StepBall(i);
            if (Remain() == 0)
                Over(true);
            Draw();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
#ifdef MODE_LASER
            else if (ch == 'z' || ch == 'Z')
                laser = 1;
#endif
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && px > 1)
                    --px;
                if (k == 77 && px < W - 3)
                    ++px;
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
