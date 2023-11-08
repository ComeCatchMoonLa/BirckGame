#include "../Engine/BrickEngine.h"

void Run();
int row = 31, col = 51;
int block[29][49];                                   // 0�����, 1����ǽ
Vector2 dir[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // 0��, 1��, 2��, 3��

struct RoleInfo
{
    int x; // ������
    int y; // ������
    int d; // ����
};
std::vector<RoleInfo> roleInfos{{1, 1, 1}}; // ��һ������ҵ���Ϣ

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
    // �������
    block[roleInfos[0].y - 1][roleInfos[0].x - 1] = 2;
    // ��ǽ
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
    FillStr(roleInfos[n].x, roleInfos[n].y, n ? "��" : roleInfos[n].d != 3 ? (roleInfos[n].d != 2 ? (roleInfos[n].d != 1 ? (roleInfos[n].d ? NULL : "��") : "��") : "��") : "��");
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
    // �������
    srand((int)time(0));
    // ���õ�ͼ�߿�
    FillRec(0, 0, col, 1, "��");
    FillRec(0, 1, 1, row - 2, "��");
    FillRec(col - 1, 1, 1, row - 2, "��");
    FillRec(0, row - 1, col, 1, "��");
    // ��ʼ����ͼ
    for (int y = 0; y < row - 2; ++y)
        for (int x = 0; x < col - 2; ++x)
            block[y][x] = ((y & 1) || (x & 1)) ? 1 : 0;
    // �����Թ�(��ǽ)
    CreateMaze();
    // ��ʾ��ͼ
    for (int y = 0; y < row - 2; ++y)
        for (int x = 0; x < col - 2; ++x)
            FillStr(x + 1, y + 1, block[y][x] == 1 ? "��" : "  ");
    // ���ɲ���ʾ����
    int enemyNum = 10; // ��������
    for (int i = 0; i < enemyNum; ++i)
    {
        CreateEnemy();
        PrintRole(i + 1);
    }
    // ���ɲ���ʾ��ɫ
    roleInfos[0].y = roleInfos[0].x = 1;
    PrintRole(0);
}

void Update()
{
    while (true)
    {
        // ����յ�
        FillStr(49, 29, "��");
        // ��Ӧ�������
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // С�������
                    for (int i = 0; i < roleInfos.size(); ++i)
                        RoleTurnLeft(i); // ��ɫ��ת
                    break;
                case 77: // С�����Ҽ�
                    for (int i = 0; i < roleInfos.size(); ++i)
                        RoleTurnRight(i); // ��ɫ��ת
                    break;
                case 72: // С�����ϼ�
                    for (int i = 0; i < roleInfos.size(); ++i)
                        RoleGoForward(i); // ��ɫǰ��
                    break;
                }
            }
        }
        // �ж���Ϸ�Ƿ����
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
    // ���ÿ���̨����
    SetConsole("�Թ�", col << 1, row, "80");
    // ������Ϸ
    Run();
}