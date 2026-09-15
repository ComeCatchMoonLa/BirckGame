#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

const int W = 18, H = 22;
int px, score, shield, lives;
int bee[40][2], nbee, bx, by, bdy;
void Run();

void DrawShip()
{
    FillStr(px, H - 2, shield > 0 ? "▲" : "A");
}

void Over()
{
    SubmitScore(65, score);
    FillStr(6, 10, "Game Over");
    Pause();
    Run();
}

void DrawBees()
{
    for (int i = 0; i < nbee; ++i)
        FillStr(bee[i][0], bee[i][1], "v");
}

void ClearBees()
{
    for (int i = 0; i < nbee; ++i)
        FillStr(bee[i][0], bee[i][1], "  ");
}

void SpawnWave()
{
    nbee = 8;
    for (int i = 0; i < nbee; ++i)
    {
        bee[i][0] = 2 + (i % 8) * 2;
        bee[i][1] = 1 + i / 8;
    }
}

void Run()
{
    ClearScreen();
    px = W / 2;
    score = 0;
    shield = 0;
    lives = 3;
    bx = -1;
    SpawnWave();
    FillStr(0, H, "Z护盾 方向移动");
    DrawShip();
    DrawBees();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            if (shield > 0)
                --shield;
            ClearBees();
            for (int i = 0; i < nbee; ++i)
            {
                if (rand() % 3 == 0)
                    bee[i][0] += (rand() & 1) ? 1 : -1;
                if (bee[i][0] < 1)
                    bee[i][0] = 1;
                if (bee[i][0] > W - 2)
                    bee[i][0] = W - 2;
                if ((rand() % 8) == 0)
                    ++bee[i][1];
                if (bee[i][1] >= H - 2)
                {
                    if (shield <= 0 && abs(bee[i][0] - px) <= 1)
                    {
                        --lives;
                        if (lives <= 0)
                            Over();
                    }
                    bee[i][1] = 1;
                }
            }
            DrawBees();
            if (bx >= 0)
            {
                FillStr(bx, by, "  ");
                by += bdy;
                if (by < 1)
                    bx = -1;
                else
                {
                    bool hit = false;
                    for (int i = 0; i < nbee; ++i)
                        if (bee[i][0] == bx && bee[i][1] == by)
                        {
                            FillStr(bee[i][0], bee[i][1], "  ");
                            bee[i][0] = bee[nbee - 1][0];
                            bee[i][1] = bee[nbee - 1][1];
                            --nbee;
                            ++score;
                            hit = true;
                            bx = -1;
                            break;
                        }
                    if (!hit && bx >= 0)
                        FillStr(bx, by, "|");
                }
            }
            if (nbee <= 0)
            {
                score += 20;
                SpawnWave();
            }
            FillStr(0, 0, std::string("S ") + std::to_string(score) + " L " + std::to_string(lives) +
                              " SH " + std::to_string(shield) + "  ");
            DrawShip();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch == 122)
                shield = 6;
            else if (ch == 'z')
                shield = 6;
            else if (ch == 224)
            {
                FillStr(px, H - 2, "  ");
                int k = getch();
                if (k == 75 && px > 1)
                    --px;
                if (k == 77 && px < W - 2)
                    ++px;
                if (k == 72 && bx < 0)
                {
                    bx = px;
                    by = H - 3;
                    bdy = -1;
                }
                DrawShip();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(65);
    srand((int)time(0));
    Run();
}
