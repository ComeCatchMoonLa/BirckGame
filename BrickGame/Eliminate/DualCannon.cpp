#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>

const int BW = 25, BH = 30;
int PlayerX, EnemyBX, EnemyBY, Score;
bool Wall[25 * 30], Bullet[25 * 30];
int which = 0;
void Run();

void ShowPlayer()
{
    FillStr(PlayerX, BH - 2, "■");
    FillStr(PlayerX - 1, BH - 1, "■■■");
}

void GameOver()
{
    FillStr(BW / 2 - 4, BH / 2, "Game Over!");
    SubmitScore(63, Score);
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

void PlayerBulletMove()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            if (idx > BW)
            {
                if (Wall[idx - BW])
                {
                    Wall[idx - BW] = false;
                    ++Score;
                    FillStr(idx % BW, idx / BW - 1, "  ");
                }
                else
                    Bullet[idx - BW] = true;
            }
        }
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
            FillStr(idx % BW, idx / BW, "■");
}

void EnemyBulletMove()
{
    if (!EnemyBX)
        return;
    if (EnemyBY >= BH - 3 && EnemyBX >= PlayerX - 1 && EnemyBX <= PlayerX + 1)
    {
        GameOver();
        return;
    }
    FillStr(EnemyBX, EnemyBY, "  ");
    ++EnemyBY;
    if (EnemyBY >= BH)
        EnemyBX = 0;
    else
        FillStr(EnemyBX, EnemyBY, "▼");
}

void Fire()
{
    int x = which ? PlayerX + 1 : PlayerX - 1;
    which ^= 1;
    if (x < 1)
        x = 1;
    if (x > BW - 2)
        x = BW - 2;
    Bullet[BW * (BH - 3) + x] = true;
}

void Run()
{
    ClearScreen();
    for (bool& w : Wall)
        w = false;
    for (bool& b : Bullet)
        b = false;
    PlayerX = BW / 2;
    EnemyBX = EnemyBY = Score = which = 0;
    ShowPlayer();
    AddWall();
    int t = 0;
    while (true)
    {
        PumpFrame();
        TakeLogicSteps(16, [&] {
            ++t &= 511;
            if (!(t & 1))
            {
                PlayerBulletMove();
                EnemyBulletMove();
            }
            if (!(t & 127))
                AddWall();
            if (!(t & 511))
            {
                EnemyBX = rand() % (BW - 3) + 1;
                EnemyBY = 1;
            }
        });
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
            else if (ch == 122)
                Fire();
            else if (ch == 224)
            {
                FillStr(PlayerX, BH - 2, "  ");
                FillRec(PlayerX - 1, BH - 1, 3, 1, "  ");
                int k = getch();
                if (k == 75)
                    PlayerX -= PlayerX != 1;
                if (k == 77)
                    PlayerX += PlayerX != BW - 2;
                ShowPlayer();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(63);
    srand((int)time(0));
    Run();
}
