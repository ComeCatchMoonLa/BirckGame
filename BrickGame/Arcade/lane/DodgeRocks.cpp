#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

int px, py, score, rx[12], ry[12], n;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("DODGE ") + std::to_string(score));
    FillStr(px, py, "@");
    for (int i = 0; i < n; ++i)
        FillStr(rx[i], ry[i], "O");
}

void Over()
{
    SubmitScore(60, score);
    FillStr(6, 8, "Game Over");
    Pause();
    Run();
}

void Run()
{
    px = 10;
    py = 16;
    score = n = 0;
    Draw();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            ++score;
            if (n < 12 && (rand() % 2) == 0)
            {
                rx[n] = 1 + rand() % 18;
                ry[n] = 1;
                ++n;
            }
            for (int i = 0; i < n;)
            {
                ++ry[i];
                if (rx[i] == px && ry[i] == py)
                    Over();
                if (ry[i] > 18)
                {
                    rx[i] = rx[n - 1];
                    ry[i] = ry[n - 1];
                    --n;
                }
                else
                    ++i;
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
                if (k == 75 && px > 1)
                    --px;
                if (k == 77 && px < 18)
                    ++px;
                if (k == 72 && py > 2)
                    --py;
                if (k == 80 && py < 18)
                    ++py;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(60);
    srand((unsigned)time(0));
    Run();
}
