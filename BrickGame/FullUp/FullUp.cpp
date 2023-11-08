#include "../Engine/BrickEngine.h"

const int BW(15), BH(20); // BW表示场景的宽度, BH表示场景的高度
int PlayerX;              // PlayerX表示玩家的横坐标
// Wall表示该处是否有墙, Bullet表示该处是否有子弹
bool Wall[15 * 20], Bullet[15 * 20];

void Run();
// 显示角色
void ShowRole()
{
    FillStr(PlayerX, BH - 2, "■");
    FillStr(PlayerX - 1, BH - 1, "■■■");
}
// 玩家死亡
void PlayerDead()
{
    int n = BW * (BH - 3) + BW - 1; // 倒数第3行最后一个坐标
    for (int idx = n - BW + 1; idx < n; ++idx)
        if (Wall[idx]) // 如果倒数第3行存在墙体, 则玩家死亡
        {
            // 先在中间留一块空白区域, 然后居中显示Game Over!
            FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
            FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
            // 改变玩家显示样式
            FillStr(PlayerX, BH - 2, "×");
            FillStr(PlayerX - 1, BH - 1, "×××");
            Pause();
            Run();
        }
}
// 显示墙体
void ShowWall()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Wall[idx])
            FillStr(idx % BW, idx / BW, "■");
}
// 添加墙体
void AddWall()
{
    // 先判断玩家是否死亡
    PlayerDead();
    // 下移所有墙体
    for (int idx = BW * (BH - 1) - 1; idx >= 0; --idx)
        if (Wall[idx])
        {
            Wall[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            Wall[idx + BW] = true;
        }
    // 所有墙体向下移动一行, 此时最上方那行空出来了, 为空出来的那行添加墙体
    for (int x = 1; x < BW - 1; ++x)
        if (rand() & 1) // 有一半的概率生成墙体
            Wall[x] = true;
    // 显示移动后的墙体
    ShowWall();
}
// 显示子弹
void ShowBullet()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
            FillStr(idx % BW, idx / BW, "■");
}
// 移动子弹
void MoveBullet()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            if (idx > BW) // 子弹未抵达边界
            {
                if (Wall[idx - BW]) // 子弹下一位置是墙, 子弹变成墙
                    Wall[idx] = true;
                else // 子弹下一位置为空, 子弹移动
                {
                    Bullet[idx - BW] = true;
                    FillStr(idx % BW, idx / BW, "  ");
                }
            }
            else // 子弹抵达边界, 子弹变成墙
                Wall[idx] = true;
        }
    // 显示移动后的子弹
    ShowBullet();
}
// 整行消除
void LineClear()
{
    int count = 0; // 记录该行的墙的数量
    for (int idx = 0; idx < BW * BH; ++idx)
    {
        if (Wall[idx])
            ++count;
        if (!((idx + 1) % BW)) // 扫描到该行结尾
        {
            if (count == BW - 2) // 该行已满, 消除整行
            {
                for (int i = idx - BW; i < BW * BH; ++i)
                    if (Wall[i])
                    {
                        Wall[i] = false;
                        FillStr(i % BW, i / BW, "  ");
                        if (i > idx)
                            Wall[i - BW] = true;
                    }
                ShowWall();
                idx -= BW; // 回退坐标, 重新扫描这一行
            }
            count = 0;
        }
    }
}
// 运行游戏
void Run()
{
    // 初始化游戏
    system("cls");
    for (bool &w : Wall)
        w = false;
    for (bool &b : Bullet)
        b = false;
    PlayerX = (BW - 1 >> 1);
    ShowRole();
    AddWall();

    int t = 0;
    while (true)
    {
        Sleep(1);
        ++t &= 255; // t的取值范围0~255, 当t等于255时, t加1值变为0
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224) // 玩家移动
            {
                // 清除角色
                FillStr(PlayerX, BH - 2, "  ");
                FillRec(PlayerX - 1, BH - 1, 3, 1, "  ");

                switch (getch())
                {
                case 75: // 左移
                    PlayerX -= PlayerX != 1;
                    break;
                case 77: // 右移
                    PlayerX += PlayerX != BW - 2;
                    break;
                }
                ShowRole();
            }
            else if (ch == 122) // 玩家射击
            {
                Bullet[BW * (BH - 3) + PlayerX] = true;
                ShowBullet();
            }
            else if (ch == 32) // 暂停游戏
                Pause();
        }
        if (!(t & 1)) // 2的倍数
        {
            MoveBullet();
            LineClear();
        }
        if (!(t & 255)) // 255的倍数
            AddWall();
    }
}

int main()
{
    SetConsole("填补射击", 30, 20, "80");
    srand((int)time(0));
    Run();
}