#include "../Engine/BrickEngine.h"

// BW表示场景的宽度, BH表示场景的高度, PW表示玩家的宽度, PH表示玩家的高度, PS表示玩家的步长
const int BW(160), BH(120), PW(8), PH(8), PS(4);
// PlayerX表示玩家的横坐标, PlayerY表示玩家的纵坐标, ballnum表示子弹的数量
int PlayerX(BW >> 1), PlayerY(BH >> 1), ballnum(10);
struct Ball
{
    int BallX, BallY, BallD; // 弹球的横坐标、纵坐标、方向
    Ball(int BallX, int BallY, int BallD)
        : BallX(BallX), BallY(BallY), BallD(BallD)
    {
    }
};
// 玩家角色像素点集合 8×8
Sites Player = {
    {2, 0}, {3, 0}, {4, 0}, {5, 0}, 
    {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1},
    {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2},
    {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3},
    {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4},
    {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5},
    {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6},
    {2, 7}, {3, 7}, {4, 7}, {5, 7}, 
};
std::vector<Ball> ball;

// 添加弹球
void AddBall()
{
    int x(0), y(0);
    bool Ball_CD = false;
    while (true)
    {
        x = rand() % BW, y = rand() % BH;
        for (const Ball &b : ball)
            if (b.BallX == x && b.BallY == y)
            {
                Ball_CD = true;
                break;
            }
        if (!Ball_CD)
            break;
        Ball_CD = false;
    }
    ball.emplace_back(x, y, (rand() & 3) + 1);
}
// 初始化游戏
void Initialize(const int &BallNum)
{
    ball.reserve(BallNum);
    for (int i = BallNum; i--;)
        AddBall();
}
// 清除弹球
void ClearBall(const Ball &b)
{
    FillStr(b.BallX, b.BallY, "  ");
}
// 显示弹球
void ShowBall(const Ball &b)
{
    FillStr(b.BallX, b.BallY, "■");
}
// 清除玩家
void ClearPlayer(int x, int y)
{
    FillRec(x, y, PW, PH, "  "); // 比FillArea()函数性能更优
}
// 显示玩家
void AddPlayer(int x, int y)
{
    FillArea(x, y, Player, "■");
}
// 改变方向——辅助函数
void ChangeD_Helper(int &BallD, bool CD1, bool CD2, int a, int b, int c)
{
    BallD = CD1 ? (CD2 ? a : b) : (!CD2 ?: c);
}
// 改变方向
void Change_D(Ball &b)
{
    switch (b.BallD)
    {
    case 1:
        ChangeD_Helper(b.BallD, b.BallX == BW - 1, !b.BallY, 3, 4, 2);
        break;
    case 2:
        ChangeD_Helper(b.BallD, b.BallX == BW - 1, b.BallY == BH - 1, 4, 3, 1);
        break;
    case 3:
        ChangeD_Helper(b.BallD, !b.BallX, b.BallY == BH - 1, 1, 2, 4);
        break;
    case 4:
        ChangeD_Helper(b.BallD, !b.BallX, !b.BallY, 2, 1, 3);
        break;
    }
}
// 移动弹球——辅助函数
void MoveBall_Helper(Ball &b, bool CD1, bool CD2, int dx, int dy)
{
    if (CD1 && CD2)
        b.BallX += dx, b.BallY += dy;
    else
        Change_D(b);
}
// 移动弹球
void MoveBall()
{
    for (Ball &b : ball)
    {
        ClearBall(b);
        switch (b.BallD)
        {
        case 1:
            MoveBall_Helper(b, b.BallY > 0, b.BallX < BW - 1, 1, -1);
            break;
        case 2:
            MoveBall_Helper(b, b.BallX < BW - 1, b.BallY < BH - 1, 1, 1);
            break;
        case 3:
            MoveBall_Helper(b, b.BallX > 0, b.BallY < BH - 1, -1, 1);
            break;
        case 4:
            MoveBall_Helper(b, b.BallX > 0, b.BallY > 0, -1, -1);
            break;
        }
        ShowBall(b);
    }
}
// 玩家死亡
void PlayerDead()
{
    for (const Ball &b : ball)
        if ((b.BallX >= PlayerX && b.BallX <= PlayerX + PW) &&
            (b.BallY >= PlayerY && b.BallY <= PlayerY + PH))
        {
            FillArea(PlayerX, PlayerY, Player, "×");
            Pause();
            AddPlayer(PlayerX, PlayerY);
        }
}
// 运行游戏
void Run()
{
    Initialize(ballnum);
    AddPlayer(PlayerX, PlayerY);
    int t(0);
    while (true)
    {
        Sleep(1);
        ++t &= 511; // t的取值范围0~511, 当t等于511时, t加1值变为0
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                ClearPlayer(PlayerX, PlayerY);
                switch (getch())
                {
                case 72: // 按下键盘上键
                    PlayerY -= PlayerY >= PS ? PS : 0;
                    break;
                case 80: // 按下键盘下键
                    PlayerY += PlayerY <= BH - PW - PS ? PS : 0;
                    break;
                case 75: // 按下键盘左键
                    PlayerX -= PlayerX >= PS ? PS : 0;
                    break;
                case 77: // 按下键盘右键
                    PlayerX += PlayerX <= BW - PH - PS ? PS : 0;
                    break;
                }
                AddPlayer(PlayerX, PlayerY);
            }
        }
        if (!(t & 1)) // 控制弹球的移动速度
        {
            MoveBall();
            PlayerDead();
        }
        if (!(t & 511)) // 控制添加弹球的时间间隔
            AddBall();
    }
}

int main()
{
    SetConsole("寸步难行", 320, 120, "80");
    srand((int)time(0));
    Run();
}