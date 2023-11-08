#include "../Engine/BrickEngine.h"

const int BW(40), BH(30); // BW表示场景的宽度, BH表示场景的高度
enum Sharp
{
    Sharp1 = 1,
    Sharp2,
    Sharp3,
    Sharp4,
};
Sharp TS1, TS2, TS3, YS1, YS2, YS3; // TS表示目标图形, YS表示你的图形
int TSy(5), score(0);               // TSy表示目标图形的纵坐标, score表示获得的分数

void Run();
// 清除图形
void ClearGraph(int x, int y)
{
    FillRec(x, y, 2, 2, "  ");
}
// 清除该行所有图形
void ClearAllGraph(int x1, int x2, int x3, int y)
{
    FillRec(x1, y, 2, 2, "  ");
    FillRec(x2, y, 2, 2, "  ");
    FillRec(x3, y, 2, 2, "  ");
}
// 显示图形
void ShowGraph(int x, int y, const Sharp &s)
{
    switch (s)
    {
    case 1:
        FillRec(x, y + 1, 1, 1, "■");
        break;
    case 2:
        FillRec(x, y, 1, 2, "■");
        break;
    case 3:
        FillArea(x, y, {{0, 0}, {0, 1}, {1, 1}}, "■");
        break;
    case 4:
        FillRec(x, y, 2, 2, "■");
        break;
    }
}
// 显示该行所有图形
void ShowAllGraph(int x1, int x2, int x3, int y, const Sharp &s1, const Sharp &s2, const Sharp &s3)
{
    ShowGraph(x1, y, s1);
    ShowGraph(x2, y, s2);
    ShowGraph(x3, y, s3);
}
// 添加新的目标图形
void AddGarph(int y, Sharp &s1, Sharp &s2, Sharp &s3)
{
    s1 = Sharp((rand() & 3) + 1);
    s2 = Sharp((rand() & 3) + 1);
    s3 = Sharp((rand() & 3) + 1);
    ShowAllGraph(10, 20, 30, y, s1, s2, s3);
}
// 目标图形下降
void DropGraph(int &y, Sharp &s1, Sharp &s2, Sharp &s3)
{
    if (y < BH - 4) // 目标图形和你的图形还未接触, 目标图形直接下降
    {
        ClearAllGraph(10, 20, 30, y);
        ++y;
        ShowAllGraph(10, 20, 30, y, s1, s2, s3);
    }
    else if (s1 == YS1 && s2 == YS2 && s3 == YS3) // 目标图形接触到了你的图形, 判断形状是否一样, 若形状一样则生成新的图形
    {
        ClearAllGraph(10, 20, 30, y);
        ++score;
        y = 5;
        AddGarph(y, s1, s2, s3);
    }
    else // 若形状不一样, 则提示游戏结束
    {
        FillStr((BW >> 1) - 2, (BH >> 1), "Game Over!");
        Pause();
        Run();
    }
}
// 改变你的图形的形状
void ChangeSharp(int x, Sharp &s)
{
    ClearGraph(x, BH - 2);
    Sharp TmpSharp;
    do
    {
        TmpSharp = Sharp((rand() & 3) + 1); // Sharp类型的值只能是1、2、3、4
    } while (s == TmpSharp);
    s = TmpSharp;
    ShowGraph(x, BH - 2, s);
}
// 运行游戏
void Run()
{
    // 初始化
    system("cls");
    TSy = 5;
    AddGarph(BH - 2, YS1, YS2, YS3);
    AddGarph(TSy, TS1, TS2, TS3);

    int time(0);
    while (true)
    {
        Sleep(1);
        if (++time > 20 - score * 0.2)
        {
            DropGraph(TSy, TS1, TS2, TS3);
            time = 0;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // 按下键盘左键
                    ChangeSharp(10, YS1);
                    break;
                case 72: // 按下键盘上键
                    ChangeSharp(20, YS2);
                    break;
                case 77: // 按下键盘右键
                    ChangeSharp(30, YS3);
                    break;
                }
            }
            else if (ch == 122) // 加速
                DropGraph(TSy, TS1, TS2, TS3);
            else if (ch == 32) // 暂停
                Pause();
        }
    }
}

int main()
{
    SetConsole("淘气方块", 80, 30, "80");
    srand((int)time(0));
    Run();
}