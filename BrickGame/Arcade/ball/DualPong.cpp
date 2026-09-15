#include "../../Engine/BrickEngine.h"
#include <string>

int y1p, y2p, bx, by, dx, dy, s1, s2;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, "P1方向 P2 WS");
    FillStr(8, 0, std::to_string(s1) + " : " + std::to_string(s2));
    FillRec(0, 1, 1, 16, "|");
    FillRec(24, 1, 1, 16, "|");
    FillStr(1, y1p, "]");
    FillStr(1, y1p + 1, "]");
    FillStr(1, y1p + 2, "]");
    FillStr(23, y2p, "[");
    FillStr(23, y2p + 1, "[");
    FillStr(23, y2p + 2, "[");
    FillStr(bx, by, "o");
}

void Goal(int who)
{
    if (who == 1)
        ++s1;
    else
        ++s2;
    if (s1 >= 5 || s2 >= 5)
    {
        SubmitScore(71, s1 > s2 ? s1 : s2);
        FillStr(8, 8, "Game Over");
        Pause();
        Run();
        return;
    }
    bx = 12;
    by = 8;
    dx = who == 1 ? 1 : -1;
}

void Run()
{
    y1p = y2p = 6;
    bx = 12;
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
            if (by <= 1 || by >= 16)
                dy = -dy;
            if (bx <= 1 && by >= y1p && by <= y1p + 2)
                dx = 1;
            else if (bx <= 1)
                Goal(2);
            if (bx >= 23 && by >= y2p && by <= y2p + 2)
                dx = -1;
            else if (bx >= 23)
                Goal(1);
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
                if (k == 72 && y1p > 1)
                    --y1p;
                if (k == 80 && y1p < 14)
                    ++y1p;
            }
            else if ((ch == 'w' || ch == 'W') && y2p > 1)
                --y2p;
            else if ((ch == 's' || ch == 'S') && y2p < 14)
                ++y2p;
        }
    }
}

int main()
{
    SetConsoleFromGameId(71);
    Run();
}
