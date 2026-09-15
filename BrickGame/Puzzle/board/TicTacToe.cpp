#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>

int b[9], cx, cy, score;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, "井字 空格落子");
    for (int i = 0; i < 9; ++i)
    {
        char c = b[i] == 1 ? 'X' : (b[i] == 2 ? 'O' : '.');
        std::string s(1, c);
        if (i % 3 == cx && i / 3 == cy)
            s = "[" + s + "]";
        else
            s = " " + s + " ";
        FillStr((i % 3) * 4, 2 + (i / 3) * 2, s);
    }
    FillStr(0, 9, std::string("WIN ") + std::to_string(score));
}

int Line(int p)
{
    const int w[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
    for (int i = 0; i < 8; ++i)
        if (b[w[i][0]] == p && b[w[i][1]] == p && b[w[i][2]] == p)
            return 1;
    return 0;
}

int Full()
{
    for (int i = 0; i < 9; ++i)
        if (!b[i])
            return 0;
    return 1;
}

void Cpu()
{
    for (int p = 2; p >= 1; --p)
        for (int i = 0; i < 9; ++i)
            if (!b[i])
            {
                b[i] = p == 2 ? 2 : 1;
                int ok = Line(p == 2 ? 2 : 1);
                b[i] = 0;
                if (ok && p == 2)
                {
                    b[i] = 2;
                    return;
                }
                if (ok && p == 1)
                {
                    b[i] = 2;
                    return;
                }
            }
    int k = rand() % 9;
    while (b[k])
        k = rand() % 9;
    b[k] = 2;
}

void EndRound(const char* msg)
{
    Draw();
    FillStr(0, 10, msg);
    Pause();
    Run();
}

void Run()
{
    for (int i = 0; i < 9; ++i)
        b[i] = 0;
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
            else if (ch == 32)
            {
                int i = cy * 3 + cx;
                if (!b[i])
                {
                    b[i] = 1;
                    if (Line(1))
                    {
                        ++score;
                        SubmitScore(24, score);
                        EndRound("You win");
                    }
                    else if (Full())
                        EndRound("Draw");
                    else
                    {
                        Cpu();
                        Draw();
                        if (Line(2))
                            EndRound("CPU win");
                        else if (Full())
                            EndRound("Draw");
                    }
                }
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < 2)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < 2)
                    ++cy;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(24);
    srand((unsigned)time(0));
    score = 0;
    Run();
}
