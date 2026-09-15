#include "../../Engine/BrickEngine.h"
#include <string>

int bar[2][3], bx, by, dx, dy, s1, s2;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("SOCCER ") + std::to_string(s1) + ":" + std::to_string(s2));
    FillStr(0, 1, "P1上下 P2 WS  槽位左右键");
    FillRec(0, 2, 1, 14, "|");
    FillRec(20, 2, 1, 14, "|");
    for (int s = 0; s < 2; ++s)
        for (int i = 0; i < 3; ++i)
            FillStr(s ? 16 : 4, bar[s][i], s ? "[" : "]");
    FillStr(bx, by, "o");
}

void Kick(int s)
{
    for (int i = 0; i < 3; ++i)
        if (by == bar[s][i] && (s ? bx >= 15 : bx <= 5))
            dx = s ? -1 : 1;
}

void Run()
{
    for (int i = 0; i < 3; ++i)
    {
        bar[0][i] = 4 + i * 4;
        bar[1][i] = 4 + i * 4;
    }
    bx = 10;
    by = 8;
    dx = 1;
    dy = 1;
    s1 = s2 = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            bx += dx;
            by += dy;
            if (by <= 2 || by >= 15)
                dy = -dy;
            Kick(0);
            Kick(1);
            if (bx <= 0)
            {
                ++s2;
                bx = 10;
            }
            if (bx >= 20)
            {
                ++s1;
                bx = 10;
            }
            if (s1 >= 5 || s2 >= 5)
            {
                SubmitScore(10, s1 > s2 ? s1 : s2);
                FillStr(6, 8, "Game Over");
                Pause();
                Run();
            }
            Draw();
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
                int d = (k == 72) ? -1 : (k == 80) ? 1 : 0;
                for (int i = 0; i < 3; ++i)
                {
                    bar[0][i] += d;
                    if (bar[0][i] < 2)
                        bar[0][i] = 2;
                    if (bar[0][i] > 15)
                        bar[0][i] = 15;
                }
            }
            else if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S')
            {
                int d = (ch == 'w' || ch == 'W') ? -1 : 1;
                for (int i = 0; i < 3; ++i)
                {
                    bar[1][i] += d;
                    if (bar[1][i] < 2)
                        bar[1][i] = 2;
                    if (bar[1][i] > 15)
                        bar[1][i] = 15;
                }
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(10);
    Run();
}
