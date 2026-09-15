#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstring>
#include <string>

#ifndef GAME_ID
#error
#endif

const int W = 4, H = 5;
int cell[H][W], sel, moves;
struct Piece
{
    int id, x, y, w, h;
};
Piece p[10];
int np;
void Run();

void Paint()
{
    std::memset(cell, 0, sizeof(cell));
    for (int i = 0; i < np; ++i)
        for (int y = 0; y < p[i].h; ++y)
            for (int x = 0; x < p[i].w; ++x)
                cell[p[i].y + y][p[i].x + x] = p[i].id;
}

int CanMove(int i, int dx, int dy)
{
    int nx = p[i].x + dx, ny = p[i].y + dy;
    if (nx < 0 || ny < 0 || nx + p[i].w > W || ny + p[i].h > H)
        return 0;
    for (int y = 0; y < p[i].h; ++y)
        for (int x = 0; x < p[i].w; ++x)
        {
            int v = cell[ny + y][nx + x];
            if (v && v != p[i].id)
                return 0;
        }
    return 1;
}

void Draw()
{
    Paint();
    ClearScreen();
    FillStr(0, 0, std::string("KL ") + std::to_string(moves) + " Z选块");
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
        {
            char c = cell[y][x] ? char('A' + cell[y][x] - 1) : '.';
            std::string s(1, c);
            if (cell[y][x] == p[sel].id)
                s = "[" + s + "]";
            else
                s = " " + s + " ";
            FillStr(x * 4, y * 2 + 2, s);
        }
}

void Win()
{
    int s = 100000 - moves;
    if (s < 1)
        s = 1;
    SubmitScore(GAME_ID, s);
    FillStr(0, 14, "Clear");
    Pause();
    Run();
}

void Layout()
{
#ifdef MODE_HENG
    p[0] = {1, 1, 0, 2, 2};
    p[1] = {2, 0, 0, 1, 2};
    p[2] = {3, 3, 0, 1, 2};
    p[3] = {4, 0, 2, 1, 2};
    p[4] = {5, 1, 2, 2, 1};
    p[5] = {6, 3, 2, 1, 2};
    p[6] = {7, 1, 3, 1, 1};
    p[7] = {8, 2, 3, 1, 1};
    p[8] = {9, 0, 4, 1, 1};
    p[9] = {10, 3, 4, 1, 1};
    np = 10;
#else
    p[0] = {1, 1, 0, 2, 2};
    p[1] = {2, 0, 0, 1, 2};
    p[2] = {3, 3, 0, 1, 2};
    p[3] = {4, 0, 2, 1, 2};
    p[4] = {5, 3, 2, 1, 2};
    p[5] = {6, 1, 2, 2, 1};
    p[6] = {7, 1, 3, 1, 1};
    p[7] = {8, 2, 3, 1, 1};
    p[8] = {9, 0, 4, 1, 1};
    p[9] = {10, 3, 4, 1, 1};
    np = 10;
#endif
}

void Run()
{
    Layout();
    sel = moves = 0;
    Paint();
    Draw();
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch == 'z' || ch == 'Z')
            {
                sel = (sel + 1) % np;
                Draw();
            }
            else if (ch == 224)
            {
                int k = getch();
                int dx = (k == 77) - (k == 75);
                int dy = (k == 80) - (k == 72);
                if (CanMove(sel, dx, dy))
                {
                    p[sel].x += dx;
                    p[sel].y += dy;
                    ++moves;
                    Paint();
                    if (p[0].x == 1 && p[0].y == 3)
                        Win();
                }
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
