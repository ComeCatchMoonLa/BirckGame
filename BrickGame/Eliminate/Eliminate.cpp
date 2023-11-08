#include "../Engine/BrickEngine.h"

const int BW(25), BH(30); // BW表示场景的宽度, BH表示场景的高度
// PlayerX表示玩家的横坐标, EnemyBX、EnemyBY分别表示敌方子弹的横横坐标、纵坐标
int PlayerX, EnemyBX, EnemyBY;
// Wall表示该处是否有墙, Bullet表示该处是否有子弹
bool Wall[25 * 30], Bullet[25 * 30];

void Run();
// 显示玩家
void ShowPlayer()
{
    FillStr(PlayerX, BH - 2, "■");
    FillStr(PlayerX - 1, BH - 1, "■■■");
}
// 玩家死亡
void GameOver()
{
    // 先在中间留一块空白区域, 然后居中显示Game Over!
    FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
    FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
    // 改变玩家显示样式
    FillStr(PlayerX, BH - 2, "×");
    FillStr(PlayerX - 1, BH - 1, "×××");
    // 清除子弹
    FillStr(EnemyBX, EnemyBY, "  ");
    Pause();
    Run();
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
    int n = BW * (BH - 3) + BW - 1; // 倒数第3行最后一个坐标
    for (int idx = n - BW + 1; idx < n; ++idx)
        if (Wall[idx]) // 如果倒数第3行存在墙体, 则玩家死亡
            GameOver();
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
// 玩家子弹移动
void PlayerBulletMove()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            if (idx > BW) // 子弹未抵达边界
            {
                if (Wall[idx - BW]) // 子弹下一位置是墙,子弹和墙一同消除
                {
                    Wall[idx - BW] = false;
                    FillStr(idx % BW, idx / BW - 1, "  ");
                }
                else
                    Bullet[idx - BW] = true;
            }
        }
    // 显示移动后的子弹
    ShowBullet();
}
// 敌人子弹移动
void EnemyBulletMove()
{
    if (!EnemyBX) // 敌人子弹是否存在
        return;
    if ((EnemyBY == BH - 2 && (EnemyBX == PlayerX - 1 || EnemyBX == PlayerX + 1)) ||
        EnemyBY == BH - 3 && EnemyBX == PlayerX) // 子弹击中玩家
        GameOver();
    if (!EnemyBY)
    {
        int MaxY = 0; // 敌方子弹所处列中Wall的最大纵坐标
        for (int y = BH - 2; y >= 0; --y)
            if (Wall[y * BW + EnemyBX])
            {
                MaxY = y;
                break;
            }
        EnemyBY = MaxY + 1;
        FillStr(EnemyBX, EnemyBY, "■");
    }
    if (EnemyBY < BH - 1) // 子弹未抵达边界
    {
        // 敌方子弹和玩家子弹距离小于2格, 分别移动1格后两个子弹将会发生碰撞
        for (int i = 1; i < 3; ++i)
            if (Bullet[(EnemyBY + i) * BW + EnemyBX])
            {
                Bullet[(EnemyBY + i) * BW + EnemyBX] = false;
                FillStr(EnemyBX, EnemyBY, "  ");
                FillStr(EnemyBX, EnemyBY + i, "  ");
                EnemyBX = 0;
                return;
            }
        ++EnemyBY;
        FillStr(EnemyBX, EnemyBY - 1, "  ");
        FillStr(EnemyBX, EnemyBY, "■");
    }
    else // 子弹抵达边界
    {
        FillStr(EnemyBX, EnemyBY, "  ");
        EnemyBX = 0;
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
    EnemyBX = EnemyBY = 0;
    ShowPlayer();
    AddWall();

    int t = 0;
    while (true)
    {
        Sleep(1);
        ++t &= 511; // t的取值范围0~511, t每次加1, 等于511时值变为0
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
                ShowPlayer();
            }
            else if (ch == 122) // 玩家射击
            {
                Bullet[BW * (BH - 3) + PlayerX] = true;
                ShowBullet();
            }
            else if (ch == 32) // 暂停游戏
                Pause();
        }
        if (!(t & 1)) // 子弹移动
        {
            PlayerBulletMove();
            EnemyBulletMove();
        }
        if (!(t & 127)) // 添加新的一行
            AddWall();
        if (!(t & 511)) // 敌人射击
        {
            EnemyBX = rand() % (BW - 3) + 1; // 1到23
            EnemyBY = 0;
        }
    }
}

int main()
{
    SetConsole("消除射击", 50, 30, "80");
    srand((int)time(0));
    Run();
}