#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

int by, vy, px, gap, score, dead;
void Run();

void Draw()
{
    ClearScreen();
    for (int y = 0; y < 18; ++y)
    {
        if (y < gap || y > gap + 4)
            FillStr(px, y + 1, "█");
    }
    FillStr(6, by, dead ? "x" : ">");
    FillStr(0, 0, std::string("FLAPPY ") + std::to_string(score) + " 空格跳");
}

void Over()
{
    SubmitScore(22, score);
    FillStr(8, 9, "Game Over");
    Pause();
    Run();
}

void Run()
{
    ClearScreen();
    by = 8;
    vy = 0;
    px = 20;
    gap = 6;
    score = dead = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            by += (vy > 0 ? -1 : 1);
            if (vy > 0)
                --vy;
            --px;
            if (px < 2)
            {
                px = 22;
                gap = 3 + rand() % 8;
                ++score;
            }
            if (by < 1 || by > 18)
                Over();
            if (px == 6 && (by < gap + 1 || by > gap + 5))
                Over();
            Draw();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32 || ch == 224)
            {
                if (ch == 224 && getch() != 72)
                    ;
                else
                    vy = 2;
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(22);
    srand((unsigned)time(0));
    Run();
}
