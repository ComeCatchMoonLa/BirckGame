#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

const int BW = 15, BH = 30, RacingW = 3, RoadW = 5;
int PlayerX, Road[BH], FuelX[BH], Score, Fuel;

void Run();

void ShowPlayer()
{
    FillStr(PlayerX + 1, BH - 4, "■");
    FillStr(PlayerX, BH - 3, "■■■");
    FillStr(PlayerX + 1, BH - 2, "■");
    FillStr(PlayerX, BH - 1, "■  ■");
}

void ClearPlayer() { FillRec(PlayerX, BH - 4, 3, 4, "  "); }

void FillRoad(int y, const std::string& fill)
{
    FillRec(0, y, Road[y], 1, fill);
    FillRec(Road[y] + RoadW, y, BW - RoadW - Road[y], 1, fill);
    if (FuelX[y] >= 0)
        FillStr(FuelX[y], y, fill == "  " ? "  " : "●");
}

void Crash()
{
    FillStr(4, 14, "Game Over!");
    SubmitScore(56, Score);
    Pause();
    Run();
}

void Hud() { FillStr(0, BH, std::string("FUEL ") + std::to_string(Fuel) + "  "); }

void TickRoad()
{
    --Fuel;
    if (Fuel <= 0)
        Crash();
    for (int y = BH - 1; y > 0; --y)
    {
        FillRoad(y, "  ");
        Road[y] = Road[y - 1];
        FuelX[y] = FuelX[y - 1];
        FillRoad(y, "■");
    }
    FillRoad(0, "  ");
    Road[0] = Road[1];
    if ((rand() % 8) == 0)
    {
        Road[0] += (rand() & 1) ? 1 : -1;
        if (Road[0] < 1)
            Road[0] = 1;
        if (Road[0] + RoadW > BW - 2)
            Road[0] = BW - 2 - RoadW;
    }
    FuelX[0] = ((rand() % 10) == 0) ? (Road[0] + 1 + rand() % 3) : -1;
    FillRoad(0, "■");
    for (int i = 0; i < 4; ++i)
    {
        int y = BH - 1 - i;
        if (PlayerX < Road[y] || PlayerX + RacingW > Road[y] + RoadW)
            Crash();
        if (FuelX[y] >= PlayerX && FuelX[y] < PlayerX + RacingW)
        {
            Fuel = 80;
            FuelX[y] = -1;
        }
    }
    Hud();
}

void Run()
{
    ClearScreen();
    PlayerX = BW / 2;
    Score = 0;
    Fuel = 80;
    int root = (BW - RoadW) / 2;
    for (int i = 0; i < BH; ++i)
    {
        Road[i] = root;
        FuelX[i] = -1;
        FillRoad(i, "■");
    }
    ShowPlayer();
    Hud();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            ++Score;
            TickRoad();
            ShowPlayer();
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
                ClearPlayer();
                int k = getch();
                if (k == 75 && PlayerX > 0)
                    --PlayerX;
                if (k == 77 && PlayerX < BW - RacingW)
                    ++PlayerX;
                ShowPlayer();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(56);
    srand((int)time(0));
    Run();
}
