#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

int hole, miss, score, ticks;
void Run();

void Draw()
{
    FillStr(0, 0, std::string("MOLE ") + std::to_string(score) + " MISS " + std::to_string(miss));
    for (int i = 0; i < 9; ++i)
    {
        int x = (i % 3) * 6, y = 2 + (i / 3) * 3;
        FillStr(x, y, i == hole ? "[O]" : "[ ]");
        FillStr(x, y + 1, std::to_string(i + 1));
    }
}

void Over()
{
    SubmitScore(31, score);
    FillStr(0, 12, "Game Over");
    Pause();
    Run();
}

void Run()
{
    ClearScreen();
    hole = rand() % 9;
    miss = score = ticks = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        TakeLogicSteps(2, [&] {
            ++ticks;
            if (ticks % 4 == 0)
            {
                ++miss;
                hole = rand() % 9;
                Draw();
                if (miss >= 8)
                    Over();
            }
        });
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch >= '1' && ch <= '9')
            {
                if (ch - '1' == hole)
                {
                    ++score;
                    miss = 0;
                    hole = rand() % 9;
                    Draw();
                }
                else
                {
                    ++miss;
                    Draw();
                    if (miss >= 8)
                        Over();
                }
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(31);
    srand((unsigned)time(0));
    Run();
}
