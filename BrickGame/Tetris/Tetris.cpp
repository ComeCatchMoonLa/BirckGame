#include "../Engine/BrickEngine.h"

const int BW(20);             // BW表示场景宽度
const int BH(27);             // BH表示场景高度
const int HomeX(BW - 1 >> 1); // HomeX表示图形初始横坐标
const int HomeY(1);           // HomeY表示图形初始纵坐标
bool Board[BW * BH];          // Board[x + y * BW]表示(x, y)处是否为小方块
int sharp;                    // 图形的形状(形状随机产生)
int x, y;                     // 图形的横纵坐标

// 声明这19种图形
// 图形4个像素点的声明顺序无关紧要
// 但每种图形必须按旋转变换的顺序依次声明
Vector2 From[19][4] = {
    // 条形
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    // 方形
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
    // L形
    {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
    {{2, 0}, {1, 0}, {0, 0}, {0, 1}},
    {{2, 2}, {2, 1}, {2, 0}, {1, 0}},
    {{0, 2}, {1, 2}, {2, 2}, {2, 1}},
    // 镜像L形
    {{2, 0}, {2, 1}, {2, 2}, {1, 2}},
    {{2, 2}, {1, 2}, {0, 2}, {0, 1}},
    {{0, 2}, {0, 1}, {0, 0}, {1, 0}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
    // T型
    {{1, 1}, {0, 2}, {1, 2}, {2, 2}},
    {{1, 1}, {0, 0}, {0, 1}, {0, 2}},
    {{1, 1}, {2, 0}, {1, 0}, {0, 0}},
    {{1, 1}, {2, 0}, {2, 1}, {2, 2}},
    // 闪电形
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    {{2, 0}, {1, 0}, {1, 1}, {0, 1}},
    // 镜像闪电形
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
    {{1, 0}, {1, 1}, {0, 1}, {0, 2}}};

void run();
// 相对坐标转绝对坐标
Vector2 SiteChange(Vector2 p)
{
    return {x + p.x, y + p.y};
}
// 显示、清除图形的辅助函数
void ShowOrClear(bool exist, const std::string &fill)
{
    for (int i = 0; i < 4; ++i)
    {
        Vector2 Pixel = SiteChange(From[sharp][i]);
        if (Board[Pixel.x + Pixel.y * BW] == exist) // 该处已存在图形, 游戏结束
        {
            SetPos(BW - 3 >> 1, BH >> 1);
            std::cout << "Game Over!";
            Pause();
            run();
        }
        Board[Pixel.x + Pixel.y * BW] = exist;
        FillStr(Pixel.x, Pixel.y, fill);
    }
}
// 显示图形
void ShowGraph() { ShowOrClear(true, "■"); }
// 清除图形
void ClearGraph() { ShowOrClear(false, "  "); }
// 添加图形
void AddGraph()
{
    sharp = rand() % 19;
    x = HomeX;
    y = HomeY;
    ShowGraph();
}
// 整行消除
void LineRemove()
{
    int JudgeY = BH - 1; // 当前判断的行数
    while (JudgeY >= 0)
    {
        int cnt = 0;
        for (int i = 0; i < BW; ++i)
            if (Board[BW * JudgeY + i])
                ++cnt;
        if (cnt != BW) // 未满行, 判断上一行
            --JudgeY;
        else // 满行消除
        {
            // 移除JudgeY这一行
            for (int i = 0; i < BW; ++i)
            {
                Board[i + JudgeY * BW] = false;
                FillStr(i, JudgeY, "  ");
            }
            // 将JudgeY这行上方的所有方块下移一行
            for (int i = BW * JudgeY - 1; i >= 0; --i)
                if (Board[i])
                {
                    Board[i] = false;
                    FillStr(i % BW, i / BW, "  ");
                    if (!Board[i + BW])
                    {
                        Board[i + BW] = true;
                        FillStr(i % BW, i / BW + 1, "■");
                    }
                }
        }
    }
}
// 碰撞检测：图形下降
bool Drop_CD()
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // 检测该点是否为该行下边的像素点
        Vector2 Pixel0 = SiteChange(From[sharp][i]);

        // 是否越界
        if (Pixel0.y > BH - 2)
            return true;
        // 是否碰到障碍物
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.x == Pixel0.x && Pixel1.y > Pixel0.y)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x + (Pixel0.y + 1) * BW])
            return true;
    }
    return false;
}
// 图形下降
void Drop()
{
    if (!Drop_CD())
    {
        ClearGraph();
        ++y;
        ShowGraph();
    }
    else
    {
        LineRemove();
        AddGraph();
    }
}
// 碰撞检测：图形左移
bool Left_CD()
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // 检测该点是否为该行最左边的像素点
        Vector2 Pixel0 = SiteChange(From[sharp][i]);

        // 是否越界
        if (Pixel0.x < 1)
            return true;
        // 是否碰到障碍物
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.y == Pixel0.y && Pixel1.x < Pixel0.x)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x - 1 + Pixel0.y * BW])
            return true;
    }
    return false;
}
// 图形左移
void Left()
{
    if (!Left_CD())
    {
        ClearGraph();
        --x;
        ShowGraph();
    }
}
// 碰撞检测：图形右移
bool Right_CD()
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // 检测该点是否为该行最右边的像素点
        Vector2 Pixel0 = SiteChange(From[sharp][i]);

        // 是否越界
        if (Pixel0.x > BW - 2)
            return true;
        // 是否碰到障碍物
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.y == Pixel0.y && Pixel1.x > Pixel0.x)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x + 1 + Pixel0.y * BW])
            return true;
    }
    return false;
}
// 图形右移
void Right()
{
    if (!Right_CD())
    {
        ClearGraph();
        ++x;
        ShowGraph();
    }
}
// 碰撞检测：图形旋转
bool Ratote_CD(int NextSharp)
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // 检测该点下一位置是否无其他像素点
        Vector2 Pixel0 = SiteChange(From[NextSharp][i]);

        // 是否越界
        if (Pixel0.x < 0 || BW - 1 < Pixel0.x || Pixel0.y > BH - 1)
            return true;
        // 是否碰到障碍物
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.x == Pixel0.x && Pixel1.y == Pixel0.y)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x + Pixel0.y * BW])
            return true;
    }
    return false;
}
// 图形旋转
void Ratote()
{
    int NextSharp = sharp;
    if (sharp == 6 || sharp == 10 || sharp == 14)
        NextSharp -= 3;
    else if (sharp == 1 || sharp == 16 || sharp == 18)
        --NextSharp;
    else
        ++NextSharp;
    if (!Ratote_CD(NextSharp))
    {
        ClearGraph();
        sharp = NextSharp;
        ShowGraph();
    }
}
// 初始化游戏
void Initialize()
{
    system("cls");
    AddGraph();
    for (bool b : Board)
        b = false;
}
// 运行游戏
void run()
{
    Initialize();

    int t = 0;
    while (true)
    {
        Sleep(1);
        if (t++ > 50)
        {
            Drop();
            t = 0;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 72:      // 按下键盘上键
                    Ratote(); // 旋转图形
                    break;
                case 80:    // 按下键盘下键
                    Drop(); // 下降图形
                    break;
                case 75:    // 按下键盘左键
                    Left(); // 左移图形
                    break;
                case 77:     // 按下键盘右键
                    Right(); // 右移图形
                    break;
                }
            }
            else if (ch == 32)
                Pause();
        }
    }
}

int main()
{
    SetConsole("俄罗斯方块", 40, 27, "80");
    srand((int)time(0));
    run();
}