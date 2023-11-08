#include "../Engine/BrickEngine.h"

void Run();
int row = 31, col = 51;
int block[29][49];                                   // 0代表空, 1代表墙
Vector2 dir[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // 0左, 1上, 2右, 3下

struct RoleInfo
{
    int x; // 横坐标
    int y; // 纵坐标
    int d; // 方向
};
std::vector<RoleInfo> roleInfos{{1, 1, 1}}; // 第一个存玩家的信息

bool CheckBlockType(int x, int y, int type, bool judgeSame = true)
{
    bool ans = 0 < x && x < col - 1 && 0 < y && y < row - 1;
    bool isSame = block[y - 1][x - 1] == type;
    ans = ans && (judgeSame ? isSame : !isSame);
    return ans;
}

void Dfs(int px, int py)
{
    int d = (rand() & 3);
    for (int i = 4; i--;)
    {
        int j = (d + i & 3);
        if (CheckBlockType(px + (dir[j].x << 1), py + (dir[j].y << 1), 0))
        {
            block[py + (dir[j].y << 1) - 1][px + (dir[j].x << 1) - 1] = block[py + dir[j].y - 1][px + dir[j].x - 1] = 2;
            Dfs(px + (dir[j].x << 1), py + (dir[j].y << 1));
        }
    }
}

void CreateMaze()
{
    // 设置起点
    block[roleInfos[0].y - 1][roleInfos[0].x - 1] = 2;
    // 挖墙
    Dfs(roleInfos[0].x, roleInfos[0].y);
}

void HandleWin()
{
    if (roleInfos[0].x == col - 2 && roleInfos[0].y == row - 2)
        Run();
}

void EraseRole(int n)
{
    FillStr(roleInfos[n].x, roleInfos[n].y, "  ");
}

void PrintRole(int n)
{
    FillStr(roleInfos[n].x, roleInfos[n].y, n ? "●" : roleInfos[n].d != 3 ? (roleInfos[n].d != 2 ? (roleInfos[n].d != 1 ? (roleInfos[n].d ? NULL : "←") : "↑") : "→") : "↓");
}

void RoleTurnLeft(int n)
{
    roleInfos[n].d = roleInfos[n].d - 1 & 3;
    PrintRole(n);
}

void RoleTurnRight(int n)
{
    roleInfos[n].d = roleInfos[n].d + 1 & 3;
    PrintRole(n);
}

bool canMoveTo(int targetX, int targetY)
{
    return 0 <= targetX && targetX <= col - 2 && 0 <= targetY && targetY <= row - 2 && CheckBlockType(targetX, targetY, 1, false);
}

void RoleGoForward(int n)
{
    if (canMoveTo(roleInfos[n].x + dir[roleInfos[n].d].x, roleInfos[n].y + dir[roleInfos[n].d].y))
    {
        EraseRole(n);
        roleInfos[n].x += dir[roleInfos[n].d].x;
        roleInfos[n].y += dir[roleInfos[n].d].y;
        PrintRole(n);
    }
}

void RoleJump(int n)
{
    if (canMoveTo(roleInfos[n].x + (dir[roleInfos[n].d].x << 1), roleInfos[n].y + (dir[roleInfos[n].d].y << 1)))
    {
        EraseRole(n);
        roleInfos[n].x += (dir[roleInfos[n].d].x << 1);
        roleInfos[n].y += (dir[roleInfos[n].d].y << 1);
        PrintRole(n);
    }
}

void CreateEnemy()
{
    int enemyX;
    int enemyY;
    bool canCreate = false;
    while (!canCreate)
    {
        enemyX = rand() % col - 3;
        enemyY = rand() % row - 3;
        canCreate = CheckBlockType(enemyX + 1, enemyY + 1, 1, true);
    }
    RoleInfo roleInfo = {enemyX, enemyY, rand() & 3};
    roleInfos.push_back(roleInfo);
}

void Initialize()
{
    // 随机种子
    srand((int)time(0));
    // 设置地图边框
    FillRec(0, 0, col, 1, "■");
    FillRec(0, 1, 1, row - 2, "■");
    FillRec(col - 1, 1, 1, row - 2, "■");
    FillRec(0, row - 1, col, 1, "■");
    // 初始化地图
    for (int y = 0; y < row - 2; ++y)
        for (int x = 0; x < col - 2; ++x)
            block[y][x] = ((y & 1) || (x & 1)) ? 1 : 0;
    // 生成迷宫(挖墙)
    CreateMaze();
    // 显示地图
    for (int y = 0; y < row - 2; ++y)
        for (int x = 0; x < col - 2; ++x)
            FillStr(x + 1, y + 1, block[y][x] == 1 ? "■" : "  ");
    // 生成并显示敌人
    int enemyNum = 10; // 敌人数量
    for (int i = 0; i < enemyNum; ++i)
    {
        CreateEnemy();
        PrintRole(i + 1);
    }
    // 生成并显示角色
    roleInfos[0].y = roleInfos[0].x = 1;
    PrintRole(0);
}

void Update()
{
    while (true)
    {
        // 标记终点
        FillStr(49, 29, "×");
        // 响应玩家输入
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // 小键盘左键
                    for (int i = 0; i < roleInfos.size(); ++i)
                        RoleTurnLeft(i); // 角色左转
                    break;
                case 77: // 小键盘右键
                    for (int i = 0; i < roleInfos.size(); ++i)
                        RoleTurnRight(i); // 角色右转
                    break;
                case 72: // 小键盘上键
                    for (int i = 0; i < roleInfos.size(); ++i)
                        RoleGoForward(i); // 角色前进
                    break;
                }
            }
        }
        // 判断游戏是否结束
        HandleWin();
    }
}

void Run()
{
    Initialize();
    Update();
}

int main()
{
    // 设置控制台属性
    SetConsole("迷宫", col << 1, row, "80");
    // 运行游戏
    Run();
}