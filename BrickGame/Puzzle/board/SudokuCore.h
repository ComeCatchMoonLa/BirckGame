#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstring>
#include <string>

#ifndef GAME_ID
#error
#endif
#ifndef SN
#define SN 9
#endif

#if SN == 6
const int BW = 3, BH = 2;
const char* kPuz = "530070000600195000098000060800060003400803001700020006060000280000419005000080079";
#else
const int BW = 3, BH = 3;
const char* kPuz = "530070000600195000098000060800060003400803001700020006060000280000419005000080079";
#endif

int g[SN][SN], given[SN][SN], cx, cy;
void Run();

int Conflict(int x, int y, int v)
{
    for (int i = 0; i < SN; ++i)
        if (i != x && g[y][i] == v)
            return 1;
    for (int i = 0; i < SN; ++i)
        if (i != y && g[i][x] == v)
            return 1;
    int bx = x / BW * BW, by = y / BH * BH;
    for (int yy = 0; yy < BH; ++yy)
        for (int xx = 0; xx < BW; ++xx)
            if (!(bx + xx == x && by + yy == y) && g[by + yy][bx + xx] == v)
                return 1;
    return 0;
}

int FullOk()
{
    for (int y = 0; y < SN; ++y)
        for (int x = 0; x < SN; ++x)
            if (!g[y][x] || Conflict(x, y, g[y][x]))
                return 0;
    return 1;
}

void Draw()
{
    ClearScreen();
    FillStr(0, 0, "SUDOKU 1-9填 Z清");
    for (int y = 0; y < SN; ++y)
        for (int x = 0; x < SN; ++x)
        {
            char c = g[y][x] ? char('0' + g[y][x]) : '.';
            std::string s(1, c);
            if (x == cx && y == cy)
                s = "[" + s + "]";
            else if (g[y][x] && Conflict(x, y, g[y][x]))
                s = "!" + s + "!";
            else
                s = " " + s + " ";
            FillStr(x * 3, y + 2, s);
        }
}

void Win()
{
    SubmitScore(GAME_ID, 1);
    FillStr(0, SN + 3, "Clear");
    Pause();
    Run();
}

void Run()
{
    std::memset(g, 0, sizeof(g));
    std::memset(given, 0, sizeof(given));
    for (int i = 0; i < SN * SN && kPuz[i]; ++i)
    {
        int v = kPuz[i] - '0';
        if (v >= 0 && v <= 9)
        {
            g[i / SN][i % SN] = v;
            given[i / SN][i % SN] = v != 0;
        }
    }
#if SN == 6
    {
        const char* p6 = "123000000456456000000123231000000564";
        for (int i = 0; i < 36; ++i)
        {
            int v = p6[i] - '0';
            g[i / 6][i % 6] = v;
            given[i / 6][i % 6] = v != 0;
        }
    }
#endif
    cx = cy = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 'z' || ch == 'Z')
            {
                if (!given[cy][cx])
                    g[cy][cx] = 0;
                Draw();
            }
            else if (ch >= '1' && ch <= '0' + SN)
            {
                if (!given[cy][cx])
                    g[cy][cx] = ch - '0';
                Draw();
                if (FullOk())
                    Win();
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < SN - 1)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < SN - 1)
                    ++cy;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(GAME_ID);
    Run();
}
