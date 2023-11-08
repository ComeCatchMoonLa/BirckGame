#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <vector>
#include <list>
#include <queue>

// 点
struct Vector2
{
    int x, y;
};
// 坐标序列
typedef std::vector<Vector2> Sites;

// 设置光标位置
inline void
SetPos(int i, short j)
{
    COORD pos = {short(i << 1), j};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// 设置控制台属性
void SetConsole(std::string name, int width, int height, std::string color)
{
    SetConsoleTitle(name.c_str());
    system(("mode con cols=" + std::to_string(width) + " lines=" + std::to_string(height)).c_str());
    system(("color " + color).c_str());
    // 隐藏控制台光标
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(handle, &CursorInfo);
}
// 暂停游戏
void Pause()
{
    while (true)
        if (kbhit() && getch() == 32) // 按空格重新开始
            break;
}
// 打印一串字符
void FillStr(int x, int y, const std::string &fillstr)
{
    SetPos(x, y);
    std::cout << fillstr;
}
// 填充矩形区域
void FillRec(int x, int y, int width, int height, const std::string &fillstr)
{
    for (int i = 0; i < height; ++i)
    {
        SetPos(x, y + i);
        for (int j = width; j--;)
            std::cout << fillstr;
    }
}
// 按像素点填充区域
void FillArea(int x, int y, const Sites &Sites, const std::string &fillstr)
{
    for (const Vector2 &Point : Sites)
    {
        SetPos(x + Point.x, y + Point.y);
        std::cout << fillstr;
    }
}