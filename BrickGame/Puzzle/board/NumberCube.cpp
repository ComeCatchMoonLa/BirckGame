#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

int g[3][3], cx, cy, moves;
void Run();

int Solved()
{
    int k = 1;
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            if (g[y][x] != k++)
                return 0;
    return 1;
}

void Draw()
{
    FillStr(0, 0, std::string("CUBE ") + std::to_string(moves) + " 空格旋行 Z旋列");
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
        {
            std::string s = std::to_string(g[y][x]);
            if (x == cx && y == cy)
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
    SubmitScore(16, s);
    FillStr(0, 10, "Clear");
    Pause();
    Run();
}

void RotRow(int y)
{
    int t = g[y][2];
    g[y][2] = g[y][1];
    g[y][1] = g[y][0];
    g[y][0] = t;
}

void RotCol(int x)
{
    int t = g[2][x];
    g[2][x] = g[1][x];
    g[1][x] = g[0][x];
    g[0][x] = t;
}

void Run()
{
    ClearScreen();
    int k = 1;
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            g[y][x] = k++;
    for (int i = 0; i < 12; ++i)
        (rand() & 1) ? RotRow(rand() % 3) : RotCol(rand() % 3);
    cx = cy = moves = 0;
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
            {
                RotRow(cy);
                ++moves;
                Draw();
                if (Solved())
                    Win();
            }
            else if (ch == 'z' || ch == 'Z')
            {
                RotCol(cx);
                ++moves;
                Draw();
                if (Solved())
                    Win();
            }
            else if (ch == 224)
            {
                int kch = getch();
                if (kch == 75 && cx)
                    --cx;
                if (kch == 77 && cx < 2)
                    ++cx;
                if (kch == 72 && cy)
                    --cy;
                if (kch == 80 && cy < 2)
                    ++cy;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(16);
    srand((unsigned)time(0));
    Run();
}
