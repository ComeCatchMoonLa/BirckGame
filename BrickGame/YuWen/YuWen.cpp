#include <iostream>
#include <windows.h>

const int BW(60); // �������Ŀ�
const int BH(40); // �������ĸ�
const std::string color("80"); // ����������ɫ
const std::string txt[] = 
{
    "xinlengdeshihou, zhouweideyiqiedoshilengde",
};

// ���������λ�� (���ڸ��»���)
inline void
SetPos(int x, short y)
{
    COORD pos = {short(x << 1), y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// ������Ϸ���ڵ�����(��Ϸ����, ���ڿ��, ��Ϸ��ɫ)
void SetGameWindow(const std::string &name, const int width, const int height, const std::string &color)
{
    SetConsoleTitle(name.c_str());
    system(("mode con cols=" + std::to_string(width << 1) + " lines=" + std::to_string(height)).c_str());
    system(("color " + color).c_str());
}

// ��������� (�Ƿ�����)
void SetMouseCursor(const bool isHidden)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = !isHidden;
    SetConsoleCursorInfo(handle, &CursorInfo);
}

// ��ʾ��Ϸ��ʼ����
void ShowStartMenu()
{
    SetPos(BW - (txt[0].length() >> 1) >> 1, BH >> 1);
    std::cout << txt[0];
}

// ��ʼ����Ϸ
void Initialize()
{
    // ��ʼ������

    // ���س�ʼUI
    ShowStartMenu();
}

// ������Ϸ
void FixedUpdated()
{
}

int main()
{
    SetGameWindow("����", BW, BH, color);
    SetMouseCursor(true);
    Initialize();
    FixedUpdated();

    std::cin.get(); // ��ͣ��Ļ
}