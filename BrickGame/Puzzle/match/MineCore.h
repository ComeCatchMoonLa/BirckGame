#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error
#endif
#ifndef MW
#define MW 9
#endif
#ifndef MH
#define MH 9
#endif
#ifndef MINES
#define MINES 10
#endif

int mine[MH][MW], adj[MH][MW], open[MH][MW], flag[MH][MW];
int cx, cy, opened, flags, ticks, started;
void Run();

int In(int x, int y) { return x >= 0 && x < MW && y >= 0 && y < MH; }

void PlaceMines(int sx, int sy)
{
    int n = 0;
    while (n < MINES)
    {
        int x = rand() % MW, y = rand() % MH;
        if (mine[y][x] || (x == sx && y == sy))
            continue;
        mine[y][x] = 1;
        ++n;
    }
    for (int y = 0; y < MH; ++y)
        for (int x = 0; x < MW; ++x)
        {
            int c = 0;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    if (In(x + dx, y + dy) && mine[y + dy][x + dx])
                        ++c;
            adj[y][x] = c;
        }
}

void Flood(int x, int y)
{
    if (!In(x, y) || open[y][x] || flag[y][x])
        return;
    open[y][x] = 1;
    ++opened;
    if (adj[y][x] == 0 && !mine[y][x])
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
                Flood(x + dx, y + dy);
}

char* Glyph(int x, int y, char* buf)
{
    if (flag[y][x])
    {
        buf[0] = 'F';
        buf[1] = 0;
        return buf;
    }
    if (!open[y][x])
    {
        buf[0] = '#';
        buf[1] = 0;
        return buf;
    }
    if (mine[y][x])
    {
        buf[0] = '*';
        buf[1] = 0;
        return buf;
    }
    if (adj[y][x] == 0)
    {
        buf[0] = '.';
        buf[1] = 0;
        return buf;
    }
    buf[0] = char('0' + adj[y][x]);
    buf[1] = 0;
    return buf;
}

void Draw()
{
    FillStr(0, 0, std::string("MINE ") + std::to_string(MINES - flags) + " T " + std::to_string(ticks) + "  ");
    char buf[4];
    for (int y = 0; y < MH; ++y)
        for (int x = 0; x < MW; ++x)
        {
            Glyph(x, y, buf);
            std::string s = buf;
            if (x == cx && y == cy)
                s = std::string("[") + buf + "]";
            else
                s = std::string(" ") + buf + " ";
            FillStr(x * 3, y + 2, s);
        }
}

void Win()
{
    int stored = 100000 - ticks;
    if (stored < 1)
        stored = 1;
    SubmitScore(GAME_ID, stored);
    FillStr(0, MH + 3, "Clear");
    Pause();
    Run();
}

void Boom()
{
    for (int y = 0; y < MH; ++y)
        for (int x = 0; x < MW; ++x)
            open[y][x] = 1;
    Draw();
    SubmitScore(GAME_ID, opened);
    FillStr(0, MH + 3, "Boom");
    Pause();
    Run();
}

void Run()
{
    ClearScreen();
    std::memset(mine, 0, sizeof(mine));
    std::memset(adj, 0, sizeof(adj));
    std::memset(open, 0, sizeof(open));
    std::memset(flag, 0, sizeof(flag));
    cx = cy = opened = flags = ticks = started = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (started && DueLogicTick())
        {
            ++ticks;
            Draw();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
            {
                if (!started)
                {
                    PlaceMines(cx, cy);
                    started = 1;
                }
                if (flag[cy][cx])
                    ;
                else if (mine[cy][cx])
                    Boom();
                else
                {
                    Flood(cx, cy);
                    Draw();
                    if (opened >= MW * MH - MINES)
                        Win();
                }
            }
            else if (ch == 'z' || ch == 'Z')
            {
                if (!open[cy][cx])
                {
                    flag[cy][cx] ^= 1;
                    flags += flag[cy][cx] ? 1 : -1;
                    Draw();
                }
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < MW - 1)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < MH - 1)
                    ++cy;
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
