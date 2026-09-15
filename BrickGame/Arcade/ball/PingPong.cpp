#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

int py, bx, by, dx, dy, score;
void Run();

void Draw()
{
    ClearScreen();
    FillRec(0, 0, 24, 1, "-");
    FillRec(0, 16, 24, 1, "-");
    FillStr(1, py, "|");
    FillStr(1, py + 1, "|");
    FillStr(1, py + 2, "|");
    FillStr(bx, by, "o");
    FillStr(0, 17, std::string("PONG ") + std::to_string(score));
}

void Over()
{
    SubmitScore(29, score);
    FillStr(8, 8, "Game Over");
    Pause();
    Run();
}

void Run()
{
    py = 6;
    bx = 4;
    by = 8;
    dx = 1;
    dy = 1;
    score = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            bx += dx;
            by += dy;
            if (by <= 1 || by >= 15)
                dy = -dy;
            if (bx >= 23)
                dx = -dx;
            if (bx <= 1)
            {
                if (by >= py && by <= py + 2)
                {
                    dx = 1;
                    ++score;
                }
                else
                    Over();
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
                if (k == 72 && py > 1)
                    --py;
                if (k == 80 && py < 13)
                    ++py;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(29);
    srand((unsigned)time(0));
    Run();
}
