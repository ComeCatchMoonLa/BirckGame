#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <queue>
#include <string>
#include <utility>

#ifndef GAME_ID
#error
#endif

const int GW = 8, GH = 6;
int board[GH + 2][GW + 2];
int sx, sy, selx, sely, pairs, ticks, score;
void Run();

int In(int x, int y) { return x >= 0 && x < GW + 2 && y >= 0 && y < GH + 2; }

bool PathOK(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
        return false;
    int vis[GH + 2][GW + 2][4][3];
    std::memset(vis, 0, sizeof(vis));
    std::queue<int> q;
    for (int d = 0; d < 4; ++d)
    {
        q.push(x1);
        q.push(y1);
        q.push(d);
        q.push(0);
        vis[y1][x1][d][0] = 1;
    }
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};
    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        int y = q.front();
        q.pop();
        int d = q.front();
        q.pop();
        int t = q.front();
        q.pop();
        for (int nd = 0; nd < 4; ++nd)
        {
            int nt = t + (nd != d);
            if (nt > 2)
                continue;
            int nx = x + dx[nd], ny = y + dy[nd];
            if (!In(nx, ny))
                continue;
            if (nx == x2 && ny == y2)
                return true;
            if (board[ny][nx])
                continue;
            if (vis[ny][nx][nd][nt])
                continue;
            vis[ny][nx][nd][nt] = 1;
            q.push(nx);
            q.push(ny);
            q.push(nd);
            q.push(nt);
        }
    }
    return false;
}

void Draw()
{
    FillStr(0, 0, std::string("LLK ") + std::to_string(score)
#ifdef MODE_TIMED
                      + " TIME " + std::to_string(150 - ticks)
#endif
                      + "  ");
    for (int y = 1; y <= GH; ++y)
        for (int x = 1; x <= GW; ++x)
        {
            char c = board[y][x] ? char('A' + board[y][x] - 1) : ' ';
            std::string s(1, c);
            if (x == sx && y == sy)
                s = "[" + s + "]";
            else if (selx == x && sely == y)
                s = "(" + s + ")";
            else
                s = " " + s + " ";
            FillStr((x - 1) * 3, y + 1, s);
        }
}

void Win()
{
    SubmitScore(GAME_ID, score + (150 - ticks));
    FillStr(0, GH + 4, "Clear");
    Pause();
    Run();
}

void TimeUp()
{
    SubmitScore(GAME_ID, score);
    FillStr(0, GH + 4, "Time up");
    Pause();
    Run();
}

void Deal()
{
    std::memset(board, 0, sizeof(board));
    int bag[GW * GH];
    int n = GW * GH / 2;
    int p = 0;
    for (int i = 0; i < n; ++i)
    {
        bag[p++] = 1 + (i % 8);
        bag[p++] = 1 + (i % 8);
    }
    for (int i = p - 1; i > 0; --i)
        std::swap(bag[i], bag[rand() % (i + 1)]);
    p = 0;
    for (int y = 1; y <= GH; ++y)
        for (int x = 1; x <= GW; ++x)
            board[y][x] = bag[p++];
    pairs = n;
    selx = sely = -1;
    sx = sy = 1;
    ticks = score = 0;
}

void Run()
{
    ClearScreen();
    Deal();
    Draw();
    while (true)
    {
        PumpFrame();
#ifdef MODE_TIMED
        if (DueLogicTick())
        {
            ++ticks;
            Draw();
            if (ticks >= 150)
                TimeUp();
        }
#endif
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
            {
                if (!board[sy][sx])
                    ;
                else if (selx < 0)
                {
                    selx = sx;
                    sely = sy;
                    Draw();
                }
                else if (selx == sx && sely == sy)
                {
                    selx = sely = -1;
                    Draw();
                }
                else if (board[sy][sx] == board[sely][selx] && PathOK(selx, sely, sx, sy))
                {
                    board[sy][sx] = board[sely][selx] = 0;
                    selx = sely = -1;
                    --pairs;
                    score += 10;
                    Draw();
                    if (pairs <= 0)
                        Win();
                }
                else
                {
                    selx = sx;
                    sely = sy;
                    Draw();
                }
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && sx > 1)
                    --sx;
                if (k == 77 && sx < GW)
                    ++sx;
                if (k == 72 && sy > 1)
                    --sy;
                if (k == 80 && sy < GH)
                    ++sy;
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
