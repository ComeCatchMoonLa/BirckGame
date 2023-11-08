#include "../Engine/BrickEngine.h"

const BW(15), BH(30);       // BW表示场景的宽, BH表示场景的高
const RacingW(3), RoadW(5); // RW表示赛车的宽, RoadW表示路的宽
int PlayerX;                // PlayerX表示赛车的横坐标
int Road[BH];               // 记录每一行路左边的位置
void Run();

// 显示玩家
void ShowPlayer()
{
    FillStr(PlayerX + 1, BH - 4, "■");
    FillStr(PlayerX, BH - 3, "■■■");
    FillStr(PlayerX + 1, BH - 2, "■");
    FillStr(PlayerX, BH - 1, "■  ■");
}
// 清除玩家
void ClearPlayer()
{
    FillRec(PlayerX, BH - 4, 3, 4, "  ");
}
// 显示\清除马路
void FillRoad(int y, const std::string &fill)
{
    if (BH - 4 <= y && y <= BH - 1 && !(Road[y] <= PlayerX && PlayerX <= Road[y] + RacingW - 1))
    {
        FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
        FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
        Pause();
        Run();
    }
    FillRec(0, y, Road[y], 1, fill);
    FillRec(Road[y] + RoadW, y, BW - RoadW - Road[y], 1, fill);
}
// 更新马路
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
// 初始化游戏
void Initialize()
{
    system("cls");
    // 初始化塞车位置
    PlayerX = BW - 2 >> 1;
    // 显示塞车
    ShowPlayer();
    // 初始化马路
    int RootPos = BW - RoadW >> 1;
    for (int &pos : Road)
        pos = RootPos;
    // 生成马路
    for (int y = BH - 1; y >= 0; --y)
        FillRoad(y, "■");
}
// 运行游戏
void Run()
{
    Initialize();
    int t = 0;
    while (true)
    {
        Sleep(1);
        ++t &= 31;
        if (!(t & 31))
            UpdataRoad();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                ClearPlayer();
                switch (getch())
                {
                case 75: // 按下小键盘左键
                {
                    bool CanMove = PlayerX != 0;
                    for (int i = 1; i <= 4; ++i)
                        CanMove &= Road[BH - i] < PlayerX;
                    if (CanMove)
                        --PlayerX;
                }
                break;
                case 77: // 按下小键盘右键
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
    SetConsole("方块赛车", 30, 30, "80");
    srand((int)time(0));
    Run();
}