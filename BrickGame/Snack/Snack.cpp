#include "../Engine/BrickEngine.h"

void run();
std::list<Vector2> snack;   // 贪吃蛇
int FoodX, FoodY, SnackD; // FoodX表示食物的横坐标, FoodY表示食物的纵坐标, SnackD表示贪吃蛇的方向
bool Dead;                // Dead表示贪吃蛇是否死亡

// 移动贪吃蛇
void MoveSnack(std::list<Vector2> &snack)
{
    // 在链表头部插入坐标对
    int dx = 0, dy = 0;
    SnackD & 1 ? dx = 2 - SnackD : dy = SnackD - 1;
    snack.insert(begin(snack), {snack.front().x + dx, snack.front().y + dy});
    // 显示贪吃蛇
    FillStr(snack.front().x, snack.front().y, "□");
    FillStr((*++snack.begin()).x, (*++snack.begin()).y, "■");
    // 没吃到食物，丢弃贪吃蛇尾部
    if (snack.front().x != FoodX || snack.front().y != FoodY)
    {
        FillStr(snack.back().x, snack.back().y, "  ");
        snack.pop_back();
    }
    else // 吃到食物
    {
        bool food_CD = false;
        while (true) // 生成食物坐标
        {
            FoodX = rand() % 40, FoodY = rand() % 25;
            for (Vector2 &s : snack)
                if (s.x == FoodX && s.y == FoodY) // 判断食物坐标是否在蛇上
                {
                    food_CD = true;
                    break;
                }
            if (!food_CD)
                break;
            food_CD = false;
        }
        FillStr(FoodX, FoodY, "●"); // 显示食物
    }
    // 判断死亡
    for (std::list<Vector2>::iterator it = ++snack.begin(); it != snack.end(); ++it)
        if ((*it).x == snack.front().x && (*it).y == snack.front().y)
            Dead = true;
    if (!Dead && (snack.front().x < 0 || snack.front().x > 39 || snack.front().y < 0 || snack.front().y > 24))
        Dead = true;
    if (Dead) // 游戏结束
    {
        FillStr(17, 12, "Game Over!");
        FillStr(13, 13, "press Spacebar to restart...");
        Pause();
        run();
    }
}

// 运行游戏
void run() 
{
    // 初始化游戏
    system("cls");
    FoodX = 20, FoodY = 16, SnackD = 1, Dead = false, snack = {{15, 17}, {14, 17}, {13, 17}};
    FillStr(FoodX, FoodY, "●");

    int t = 0;         // 控制贪吃蛇的速度
    bool move = false; // 使贪吃蛇：每移动一次至多改变一次方向
    while (true)
    {
        Sleep(30);
        if (t++ > 10)
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
            else if (ch == 32)  // 暂停游戏 | 重新开始
                Pause();
        }
    }
}

int main()
{
    SetConsole("贪吃蛇", 80, 25, "80");
    srand((int)time(0));
    run();
}