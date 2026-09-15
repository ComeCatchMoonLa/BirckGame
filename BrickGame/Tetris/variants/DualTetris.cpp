#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

const int W = 10, H = 20, BX1 = 1, BX2 = 14, BY = 0;
const int kShape[7][4][4][2] = {
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}, {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 1}}, {{2, 0}, {2, 1}, {2, 2}, {2, 3}}},
    {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}, {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {2, 0}, {1, 1}, {2, 1}}, {{1, 0}, {2, 0}, {1, 1}, {2, 1}}},
    {{{0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
     {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
     {{1, 1}, {2, 1}, {0, 2}, {1, 2}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{2, 0}, {1, 1}, {2, 1}, {1, 2}},
     {{0, 1}, {1, 1}, {1, 2}, {2, 2}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}}},
    {{{0, 0}, {0, 1}, {0, 2}, {1, 2}}, {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {0, 1}, {1, 1}, {2, 1}}},
    {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}, {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {0, 2}}, {{0, 0}, {1, 0}, {2, 0}, {2, 1}}},
};

struct P
{
    int b[H][W];
    int x, y, rot, kind, score;
    bool over;
    int ox;
};

P a, b;
void Run();

void Cell(const P& p, int x, int y, const char* g)
{
    if (y < 0)
        return;
    FillStr(x + p.ox, y + BY, g);
}

bool Hits(const P& p, int nx, int ny, int nr)
{
    for (int i = 0; i < 4; ++i)
    {
        int x = nx + kShape[p.kind][nr][i][0];
        int y = ny + kShape[p.kind][nr][i][1];
        if (x < 0 || x >= W || y >= H)
            return true;
        if (y >= 0 && p.b[y][x])
            return true;
    }
    return false;
}

void DrawP(P& p, bool show)
{
    for (int i = 0; i < 4; ++i)
        Cell(p, p.x + kShape[p.kind][p.rot][i][0], p.y + kShape[p.kind][p.rot][i][1], show ? "■" : "  ");
}

void Spawn(P& p)
{
    p.kind = rand() % 7;
    p.rot = 0;
    p.x = W / 2 - 1;
    p.y = 0;
    if (Hits(p, p.x, p.y, p.rot))
        p.over = true;
}

void ClearLine(P& p)
{
    for (int y = H - 1; y >= 0; --y)
    {
        int c = 0;
        for (int x = 0; x < W; ++x)
            c += p.b[y][x] != 0;
        if (c == W)
        {
            for (int yy = y; yy > 0; --yy)
                for (int x = 0; x < W; ++x)
                    p.b[yy][x] = p.b[yy - 1][x];
            for (int x = 0; x < W; ++x)
                p.b[0][x] = 0;
            ++p.score;
            ++y;
        }
    }
}

void Redraw(P& p)
{
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            Cell(p, x, y, p.b[y][x] ? "■" : "  ");
    if (!p.over)
        DrawP(p, true);
}

void Lock(P& p)
{
    DrawP(p, false);
    for (int i = 0; i < 4; ++i)
    {
        int x = p.x + kShape[p.kind][p.rot][i][0];
        int y = p.y + kShape[p.kind][p.rot][i][1];
        if (y >= 0)
            p.b[y][x] = 1;
    }
    ClearLine(p);
    Spawn(p);
    Redraw(p);
}

void Try(P& p, int dx, int dy, int dr)
{
    if (p.over)
        return;
    int nr = (p.rot + dr) & 3;
    if (!Hits(p, p.x + dx, p.y + dy, nr))
    {
        DrawP(p, false);
        p.x += dx;
        p.y += dy;
        p.rot = nr;
        DrawP(p, true);
    }
    else if (dy == 1 && dx == 0 && dr == 0)
        Lock(p);
}

void EndIfNeed()
{
    if (a.over && b.over)
    {
        SubmitScore(41, a.score + b.score);
        FillStr(10, 10, "Game Over");
        Pause();
        Run();
    }
}

void InitP(P& p, int ox)
{
    std::memset(p.b, 0, sizeof(p.b));
    p.ox = ox;
    p.score = 0;
    p.over = false;
    FillRec(ox - 1, BY, 1, H, "│");
    FillRec(ox + W, BY, 1, H, "│");
    Spawn(p);
    Redraw(p);
}

void Run()
{
    ClearScreen();
    FillStr(1, H + 1, "P1方向键 P2 WASD");
    InitP(a, BX1);
    InitP(b, BX2);
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            Try(a, 0, 1, 0);
            Try(b, 0, 1, 0);
            EndIfNeed();
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
                if (k == 72)
                    Try(a, 0, 0, 1);
                else if (k == 80)
                    Try(a, 0, 1, 0);
                else if (k == 75)
                    Try(a, -1, 0, 0);
                else if (k == 77)
                    Try(a, 1, 0, 0);
            }
            else if (ch == 'w' || ch == 'W')
                Try(b, 0, 0, 1);
            else if (ch == 's' || ch == 'S')
                Try(b, 0, 1, 0);
            else if (ch == 'a' || ch == 'A')
                Try(b, -1, 0, 0);
            else if (ch == 'd' || ch == 'D')
                Try(b, 1, 0, 0);
        }
    }
}

int main()
{
    SetConsoleFromGameId(41);
    srand((unsigned)time(0));
    Run();
}
