#include "../Engine/BrickEngine.h"
#include "Map.h"

void run();
std::list<Vector2> snack; // 贪吃蛇
// 食物位置、贪吃蛇方向、贪吃蛇速度、关卡数、玩家得分
int FoodX, FoodY, SnackD, Speed(1), MapNum(1), Score;
bool Dead;       // 贪吃蛇是否死亡
std::string Map; // 关卡

void LogMap(std::string Map) // 设置蛇的属性并打印地图
{
    if (MapNum == 5 || MapNum == 6)
        SnackD = 0, snack = {{36, 24}, {36, 25}, {36, 26}};
    else
        SnackD = 1, snack = {{16, 17}, {15, 17}, {14, 17}};
    SetPos(0, 4);
    for (int i = 0; i < 40 * 26; ++i)
        std::cout << (Map[i] == '0' ? "■" : "  ");
}
void LogSelectMap() // 打印预览地图
{
    Map = Maps[MapNum - 1];
    LogMap(Map);
    FillRec(14, 13, 12, 8, "  ");
    FillStr(18, 15, " Map: " + std::to_string(MapNum));
    FillStr(18, 17, "Speed: " + std::to_string(Speed));
}
void AddFood() // 生成食物
{
    bool food_CD = false;
    while (true)
    {
        FoodX = rand() % 40, FoodY = rand() % 24 + 5;
        if (Map[FoodX + (FoodY - 4) * 40] == '0')
            food_CD = true;
        else
            for (const Vector2 &s : snack)
                if (s.x == FoodX && s.y == FoodY)
                {
                    food_CD = true;
                    break;
                }
        if (!food_CD)
            break;
        food_CD = false;
    }
    FillStr(FoodX, FoodY, "●");
}
void SelectMap() // 选择地图
{
    while (true)
    {
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // 小键盘左键
                    MapNum -= MapNum != 1;
                    break;
                case 77: // 小键盘右键
                    MapNum += MapNum != 6;
                    break;
                case 72: // 小键盘上键
                    Speed += Speed != 5;
                    break;
                case 80: // 小键盘下键
                    Speed -= Speed != 1;
                    break;
                }
                LogSelectMap();
                FillStr(9, 2, ' ' + std::to_string(MapNum));
                FillStr(32, 2, std::to_string(Speed));
            }
            else if (ch == 32)
                break;
        }
    }
}
void MoveSnack(std::list<Vector2> &snack) // 移动贪吃蛇
{
    // 在链表头部插入坐标对
    int dx = 0, dy = 0;
    SnackD & 1 ? dx = 2 - SnackD : dy = SnackD - 1;
    snack.insert(begin(snack), {snack.front().x + dx, snack.front().y + dy});
    // 判断是否吃到食物
    if (snack.front().x != FoodX || snack.front().y != FoodY) // 没吃到食物，丢弃贪吃蛇尾部
    {
        FillStr(snack.back().x, snack.back().y, "  ");
        snack.pop_back();
    }
    else // 吃到食物
    {
        SetPos(20, 2);
        std::cout << std::setw(3) << ++Score;
        AddFood();
    }
    // 判断死亡
    if (Map[snack.front().x + (snack.front().y - 4) * 40] == '0')
        Dead = true;
    else
        for (std::list<Vector2>::iterator it = ++snack.begin(); it != snack.end(); ++it)
            if ((*it).x == snack.front().x && (*it).y == snack.front().y)
                Dead = true;
    // 显示贪吃蛇
    if (Dead)
        for (const Vector2 &s : snack)
            FillStr(s.x, s.y, "x");
    else
    {
        FillStr(snack.front().x, snack.front().y, "□");
        FillStr((*++snack.begin()).x, (*++snack.begin()).y, "■");
    }
    if (Dead) // 游戏结束
    {
        FillStr(17, 16, "Game Over!");
        FillStr(13, 17, "press Spacebar to restart...");
        Pause();
        run();
    }
}
void Initialize() // 初始化游戏
{
    system("cls");
    Dead = false, Score = 0;
    FillStr(6, 1, "==========\t\t==============\t\t===========\n");
    FillStr(6, 2, "| Map: " + std::to_string(MapNum) + " |\t\t| SCORE:   " + std::to_string(Score) + " |\t\t| SPEED:" + std::to_string(Speed) + " |");
    FillStr(6, 3, "==========\t\t==============\t\t===========\n");
    LogSelectMap(); // 打印预选地图
    SelectMap();    // 选择地图
    LogMap(Map);    // 打印已选地图
    AddFood();      // 添加食物
}
void run() // 运行游戏
{
    Initialize();
    int t = 0;         // 控制贪吃蛇的速度
    bool move = false; // 使贪吃蛇：每移动一次至多改变一次方向
    while (true)
    {
        Sleep(1);
        if (t++ > 30 - Speed * 5)
        {
            MoveSnack(snack);
            move = true, t = 0;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224 && move) // 改变贪吃蛇的方向
            {
                ch = getch();
                SnackD = SnackD & 1 ? (ch == 72 ? 0 : (ch == 80 ? 2 : SnackD)) : (ch == 75 ? 3 : (ch == 77 ? 1 : SnackD));
                move = false;
            }
            else if (ch == 122) // 加速移动
                MoveSnack(snack);
            else if (ch == 32) // 暂停游戏
                Pause();
        }
    }
}

int main()
{
    SetConsole("贪吃蛇", 80, 30, "80");
    srand((int)time(0));
    run();
}