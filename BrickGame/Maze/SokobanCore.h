#pragma once
#include "../Engine/BrickEngine.h"
#include <cstring>
#include <string>

#ifndef GAME_ID
#error
#endif

const int W = 9, H = 7;
const char* kLvl =
    "#########"
    "#   .   #"
    "# $ $ $ #"
    "#   @   #"
    "# $ $ $ #"
    "#   . . #"
    "#########";
int wall[H][W], box[H][W], goal[H][W], px, py, moves;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("BOX ") + std::to_string(moves));
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
        {
            const char* g = " ";
            if (wall[y][x])
                g = "#";
            else if (box[y][x] && goal[y][x])
                g = "*";
            else if (box[y][x])
                g = "$";
            else if (goal[y][x])
                g = ".";
            if (x == px && y == py)
                g = "@";
            FillStr(x, y + 1, g);
        }
}

void Win()
{
    SubmitScore(GAME_ID, 100000 - moves > 1 ? 100000 - moves : 1);
    FillStr(0, H + 2, "Clear");
    Pause();
    Run();
}

int Done()
{
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            if (goal[y][x] && !box[y][x])
                return 0;
    return 1;
}

void Slide(int& x, int& y, int dx, int dy)
{
#ifdef MODE_ICE
    while (true)
    {
        int nx = x + dx, ny = y + dy;
        if (nx < 0 || ny < 0 || nx >= W || ny >= H || wall[ny][nx] || box[ny][nx])
            break;
        x = nx;
        y = ny;
    }
#else
    (void)dx;
    (void)dy;
#endif
}

void Try(int dx, int dy)
{
    int nx = px + dx, ny = py + dy;
    if (nx < 0 || ny < 0 || nx >= W || ny >= H || wall[ny][nx])
        return;
    if (box[ny][nx])
    {
        int tx = nx + dx, ty = ny + dy;
#ifdef MODE_ICE
        int sx = nx, sy = ny;
        Slide(tx, ty, dx, dy);
        if (tx == nx && ty == ny)
            return;
        box[ny][nx] = 0;
        box[ty][tx] = 1;
        px = nx;
        py = ny;
        (void)sx;
        (void)sy;
#else
        if (tx < 0 || ty < 0 || tx >= W || ty >= H || wall[ty][tx] || box[ty][tx])
            return;
        box[ny][nx] = 0;
        box[ty][tx] = 1;
        px = nx;
        py = ny;
#endif
    }
    else
    {
        px = nx;
        py = ny;
#ifdef MODE_ICE
        Slide(px, py, dx, dy);
#endif
    }
    ++moves;
    if (Done())
        Win();
}

void Run()
{
    std::memset(wall, 0, sizeof(wall));
    std::memset(box, 0, sizeof(box));
    std::memset(goal, 0, sizeof(goal));
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
        {
            char c = kLvl[y * W + x];
            if (c == '#')
                wall[y][x] = 1;
            if (c == '$')
                box[y][x] = 1;
            if (c == '.')
                goal[y][x] = 1;
            if (c == '@')
            {
                px = x;
                py = y;
            }
        }
    moves = 0;
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
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75)
                    Try(-1, 0);
                if (k == 77)
                    Try(1, 0);
                if (k == 72)
                    Try(0, -1);
                if (k == 80)
                    Try(0, 1);
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
