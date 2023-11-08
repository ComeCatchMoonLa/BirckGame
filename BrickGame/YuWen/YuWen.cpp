#include <iostream>
#include <windows.h>

const int BW(60); // 方块矩阵的宽
const int BH(40); // 方块矩阵的高
const std::string color("80"); // 方块矩阵的配色
const std::string txt[] = 
{
    "xinlengdeshihou, zhouweideyiqiedoshilengde",
};

// 设置鼠标光标位置 (用于更新画面)
inline void
SetPos(int x, short y)
{
    COORD pos = {short(x << 1), y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 设置游戏窗口的属性(游戏名称, 窗口宽高, 游戏配色)
void SetGameWindow(const std::string &name, const int width, const int height, const std::string &color)
{
    SetConsoleTitle(name.c_str());
    system(("mode con cols=" + std::to_string(width << 1) + " lines=" + std::to_string(height)).c_str());
    system(("color " + color).c_str());
}

// 设置鼠标光标 (是否被隐藏)
void SetMouseCursor(const bool isHidden)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = !isHidden;
    SetConsoleCursorInfo(handle, &CursorInfo);
}

// 显示游戏开始界面
void ShowStartMenu()
{
    SetPos(BW - (txt[0].length() >> 1) >> 1, BH >> 1);
    std::cout << txt[0];
}

// 初始化游戏
void Initialize()
{
    // 初始化变量

    // 加载初始UI
    ShowStartMenu();
}

// 更新游戏
void FixedUpdated()
{
}

int main()
{
    SetGameWindow("余温", BW, BH, color);
    SetMouseCursor(true);
    Initialize();
    FixedUpdated();

    std::cin.get(); // 暂停屏幕
}