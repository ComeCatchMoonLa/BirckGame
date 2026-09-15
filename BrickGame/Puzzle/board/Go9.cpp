#include "../../Engine/BrickEngine.h"
#include <cstring>
#include <string>
#include <vector>

const int N = 9;
int b[N][N], turn, cx, cy, vis[N][N];
void Run();

int In(int x, int y) { return x >= 0 && y >= 0 && x < N && y < N; }

void Flood(int x, int y, int col, std::vector<int>& stones, int& lib)
{
    if (!In(x, y) || vis[y][x])
        return;
    if (b[y][x] == 0)
    {
        ++lib;
        return;
    }
    if (b[y][x] != col)
        return;
    vis[y][x] = 1;
    stones.push_back(y * N + x);
    Flood(x + 1, y, col, stones, lib);
    Flood(x - 1, y, col, stones, lib);
    Flood(x, y + 1, col, stones, lib);
    Flood(x, y - 1, col, stones, lib);
}

void Capture(int col)
{
    std::memset(vis, 0, sizeof(vis));
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
            if (b[y][x] == col && !vis[y][x])
            {
                std::vector<int> st;
                int lib = 0;
                Flood(x, y, col, st, lib);
                if (lib == 0)
                    for (int id : st)
                        b[id / N][id % N] = 0;
            }
}

void Draw()
{
    ClearScreen();
    FillStr(0, 0, turn == 1 ? "黑 9x9" : "白 9x9");
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
        {
            char c = b[y][x] == 1 ? 'X' : (b[y][x] == 2 ? 'O' : '+');
            std::string s(1, c);
            if (x == cx && y == cy)
                s = "[" + s + "]";
            else
                s = " " + s + " ";
            FillStr(x * 2, y + 1, s);
        }
}

void PassEnd()
{
    int a = 0, c = 0;
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
            if (b[y][x] == 1)
                ++a;
            else if (b[y][x] == 2)
                ++c;
    SubmitScore(27, a > c ? a : c);
    FillStr(0, N + 2, std::string("End ") + std::to_string(a) + ":" + std::to_string(c));
    Pause();
    Run();
}

int passes;

void Run()
{
    std::memset(b, 0, sizeof(b));
    turn = 1;
    cx = cy = N / 2;
    passes = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 'p' || ch == 'P')
            {
                ++passes;
                turn = 3 - turn;
                if (passes >= 2)
                    PassEnd();
                Draw();
            }
            else if (ch == 32 && !b[cy][cx])
            {
                b[cy][cx] = turn;
                Capture(3 - turn);
                std::vector<int> st;
                int lib = 0;
                std::memset(vis, 0, sizeof(vis));
                Flood(cx, cy, turn, st, lib);
                if (lib == 0)
                    b[cy][cx] = 0;
                else
                {
                    passes = 0;
                    turn = 3 - turn;
                }
                Draw();
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < N - 1)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < N - 1)
                    ++cy;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(27);
    Run();
}
