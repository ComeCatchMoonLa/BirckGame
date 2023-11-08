#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <vector>
#include <list>
#include <queue>

// ��
struct Vector2
{
    int x, y;
};
// ��������
typedef std::vector<Vector2> Sites;

// ���ù��λ��
inline void
SetPos(int i, short j)
{
    COORD pos = {short(i << 1), j};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
// ���ÿ���̨����
void SetConsole(std::string name, int width, int height, std::string color)
{
    SetConsoleTitle(name.c_str());
    system(("mode con cols=" + std::to_string(width) + " lines=" + std::to_string(height)).c_str());
    system(("color " + color).c_str());
    // ���ؿ���̨���
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(handle, &CursorInfo);
}
// ��ͣ��Ϸ
void Pause()
{
    while (true)
        if (kbhit() && getch() == 32) // ���ո����¿�ʼ
            break;
}
// ��ӡһ���ַ�
void FillStr(int x, int y, const std::string &fillstr)
{
    SetPos(x, y);
    std::cout << fillstr;
}
// ����������
void FillRec(int x, int y, int width, int height, const std::string &fillstr)
{
    for (int i = 0; i < height; ++i)
    {
        SetPos(x, y + i);
        for (int j = width; j--;)
            std::cout << fillstr;
    }
}
// �����ص��������
void FillArea(int x, int y, const Sites &Sites, const std::string &fillstr)
{
    for (const Vector2 &Point : Sites)
    {
        SetPos(x + Point.x, y + Point.y);
        std::cout << fillstr;
    }
}