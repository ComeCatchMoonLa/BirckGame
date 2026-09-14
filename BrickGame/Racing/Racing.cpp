#include "../Engine/BrickEngine.h"

const BW(15), BH(30);       // BW琛ㄧず鍦烘櫙鐨勫��, BH琛ㄧず鍦烘櫙鐨勯珮
const RacingW(3), RoadW(5); // RW琛ㄧず璧涜溅鐨勫��, RoadW琛ㄧず璺�鐨勫��
int PlayerX;                // PlayerX琛ㄧず璧涜溅鐨勬í鍧愭爣
int Road[BH];               // 璁板綍姣忎竴琛岃矾宸﹁竟鐨勪綅缃�
int Score;
void Run();

// 鏄剧ず鐜╁��
void ShowPlayer()
{
    FillStr(PlayerX + 1, BH - 4, "■");
    FillStr(PlayerX, BH - 3, "■■■");
    FillStr(PlayerX + 1, BH - 2, "■");
    FillStr(PlayerX, BH - 1, "■  ■");
}
// 娓呴櫎鐜╁��
void ClearPlayer()
{
    FillRec(PlayerX, BH - 4, 3, 4, "  ");
}
// 鏄剧ず\娓呴櫎椹�璺�
void FillRoad(int y, const std::string &fill)
{
    if (BH - 4 <= y && y <= BH - 1 && !(Road[y] <= PlayerX && PlayerX <= Road[y] + RacingW - 1))
    {
        FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
        FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
        SubmitScore(7, Score);
        Pause();
        Run();
    }
    FillRec(0, y, Road[y], 1, fill);
    FillRec(Road[y] + RoadW, y, BW - RoadW - Road[y], 1, fill);
}
// 鏇存柊椹�璺�
void UpdataRoad()
{
    for (int y = BH - 1; y > 0; --y)
        if (Road[y] != Road[y - 1])
        {
            FillRoad(y, "  ");
            Road[y] = Road[y - 1];
            FillRoad(y, "■");
        }
    if (Road[1] == Road[2] && Road[2] == Road[3] && Road[3] == Road[4] && Road[4] == Road[5])
    {
        FillRoad(0, "  ");
        while (true)
        {
            Road[0] += ((rand() & 1) ? 1 : -1) * ((rand() & 1) + 1);
            if (Road[0] > 0 && Road[0] + RoadW < BW - 1)
                break;
        }
        FillRoad(0, "■");
    }
}
// 鍒濆�嬪寲娓告垙
void Initialize()
{
    ClearScreen();
    // 鍒濆�嬪寲濉炶溅浣嶇疆
    PlayerX = BW - 2 >> 1;
    Score = 0;
    // 鏄剧ず濉炶溅
    ShowPlayer();
    // 鍒濆�嬪寲椹�璺�
    int RootPos = BW - RoadW >> 1;
    for (int &pos : Road)
        pos = RootPos;
    // 鐢熸垚椹�璺�
    for (int y = BH - 1; y >= 0; --y)
        FillRoad(y, "■");
}
// 杩愯�屾父鎴�
void Run()
{
    Initialize();
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            ++Score;
            UpdataRoad();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            if (ch == 224)
            {
                ClearPlayer();
                switch (getch())
                {
                case 75: // 鎸変笅灏忛敭鐩樺乏閿�
                {
                    bool CanMove = PlayerX != 0;
                    for (int i = 1; i <= 4; ++i)
                        CanMove &= Road[BH - i] < PlayerX;
                    if (CanMove)
                        --PlayerX;
                }
                break;
                case 77: // 鎸変笅灏忛敭鐩樺彸閿�
                {
                    bool CanMove = PlayerX != BW - RacingW;
                    for (int i = 1; i <= 4; ++i)
                        CanMove &= PlayerX < Road[BH - i] + RacingW - 1;
                    if (CanMove)
                        ++PlayerX;
                }
                break;
                }
                ShowPlayer();
            }
            else if (ch == 122)
                UpdataRoad();
            else if (ch == 32)
                Pause();
        }
    }
}

int main()
{
    SetConsoleFromGameId(7);
    srand((int)time(0));
    Run();
}
