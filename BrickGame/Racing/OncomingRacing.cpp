#include "../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>

const int BW = 15, BH = 30, RacingW = 3, RoadW = 5;
int PlayerX, Road[BH], CarX[BH], Score;

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
    if (CarX[y] >= 0)
        FillStr(CarX[y], y, fill == "  " ? "  " : "▼");
}

void Crash()
{
    FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
    FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
    SubmitScore(55, Score);
    Pause();
    Run();
}

void TickRoad()
{
    for (int y = BH - 1; y > 0; --y)
    {
        FillRoad(y, "  ");
        Road[y] = Road[y - 1];
        CarX[y] = CarX[y - 1];
        FillRoad(y, "■");
    }
    FillRoad(0, "  ");
    if (Road[1] == Road[7])
    {
        Road[0] += ((rand() & 1) ? 1 : -1);
        if (Road[0] < 1)
            Road[0] = 1;
        if (Road[0] + RoadW > BW - 2)
            Road[0] = BW - 2 - RoadW;
    }
    else
        Road[0] = Road[1];
    CarX[0] = ((rand() % 6) == 0) ? (Road[0] + 1 + rand() % 3) : -1;
    FillRoad(0, "■");
    for (int i = 0; i < 4; ++i)
    {
        int y = BH - 1 - i;
        if (PlayerX < Road[y] || PlayerX + RacingW > Road[y] + RoadW)
            Crash();
        if (CarX[y] >= PlayerX && CarX[y] < PlayerX + RacingW)
            Crash();
    }
}

void Run()
{
    ClearScreen();
    PlayerX = BW / 2;
    Score = 0;
    int root = (BW - RoadW) / 2;
    for (int i = 0; i < BH; ++i)
    {
        Road[i] = root;
        CarX[i] = -1;
        FillRoad(i, "■");
    }
    ShowPlayer();
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
    SetConsoleFromGameId(55);
    srand((int)time(0));
    Run();
}
