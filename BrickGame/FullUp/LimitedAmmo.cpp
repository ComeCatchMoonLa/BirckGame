#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

const int BW = 15, BH = 20;
int PlayerX, Score, Ammo;
bool Wall[15 * 20], Bullet[15 * 20];
void Run();

void ShowRole()
{
    FillStr(PlayerX, BH - 2, "■");
    FillStr(PlayerX - 1, BH - 1, "■■■");
}

void GameOver()
{
    FillStr(4, 8, "Game Over!");
    SubmitScore(64, Score);
    Pause();
    Run();
}

void ShowWall()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Wall[idx])
            FillStr(idx % BW, idx / BW, "■");
}

void AddWall()
{
    for (int idx = BW * (BH - 3); idx < BW * (BH - 2); ++idx)
        if (Wall[idx])
        {
            GameOver();
            return;
        }
    for (int idx = BW * (BH - 1) - 1; idx >= 0; --idx)
        if (Wall[idx])
        {
            Wall[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            Wall[idx + BW] = true;
        }
    for (int x = 1; x < BW - 1; ++x)
        if (rand() & 1)
            Wall[x] = true;
    ShowWall();
}

void MoveBullet()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            if (idx > BW)
            {
                if (Wall[idx - BW])
                    Wall[idx] = true;
                else
                    Bullet[idx - BW] = true;
            }
            else
                Wall[idx] = true;
        }
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
            FillStr(idx % BW, idx / BW, "■");
}

void LineClear()
{
    for (int y = 0; y < BH; ++y)
    {
        int count = 0;
        for (int x = 1; x < BW - 1; ++x)
            if (Wall[y * BW + x])
                ++count;
        if (count == BW - 2)
        {
            for (int i = y * BW; i < BW * BH; ++i)
                if (Wall[i])
                {
                    Wall[i] = false;
                    FillStr(i % BW, i / BW, "  ");
                    if (i >= (y + 1) * BW)
                        Wall[i - BW] = true;
                }
            ShowWall();
            ++Score;
            --y;
        }
    }
}

void Hud() { FillStr(0, BH, std::string("AMMO ") + std::to_string(Ammo) + "  "); }

void Run()
{
    ClearScreen();
    for (bool& w : Wall)
        w = false;
    for (bool& b : Bullet)
        b = false;
    PlayerX = BW / 2;
    Score = 0;
    Ammo = 6;
    ShowRole();
    AddWall();
    Hud();
    int t = 0;
    while (true)
    {
        PumpFrame();
        TakeLogicSteps(64, [&] {
            ++t &= 255;
            if (!(t & 1))
            {
                MoveBullet();
                LineClear();
            }
            if (!(t & 31) && Ammo < 8)
            {
                ++Ammo;
                Hud();
            }
            if (!(t & 255))
                AddWall();
        });
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch == 122 && Ammo > 0)
            {
                --Ammo;
                Bullet[BW * (BH - 3) + PlayerX] = true;
                Hud();
            }
            else if (ch == 224)
            {
                FillStr(PlayerX, BH - 2, "  ");
                FillRec(PlayerX - 1, BH - 1, 3, 1, "  ");
                int k = getch();
                if (k == 75)
                    PlayerX -= PlayerX != 1;
                if (k == 77)
                    PlayerX += PlayerX != BW - 2;
                ShowRole();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(64);
    srand((int)time(0));
    Run();
}
