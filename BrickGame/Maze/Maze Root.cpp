#include "../Engine/BrickEngine.h"

void Run();
int row = 31, col = 51;
int px = 1, py = 1;
int block[29][49];                                   // 0�����, 1����ǽ
Vector2 dir[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // 0��, 1��, 2��, 3��

void Initialize()
{
    // �������
    srand((int)time(0));
    // ���ñ߿�
    FillRec(0, 0, col, 1, "��");
    FillRec(0, 1, 1, row - 2, "��");
    FillRec(col - 1, 1, 1, row - 2, "��");
    FillRec(0, row - 1, col, 1, "��");
}

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
    block[py - 1][px - 1] = 2;
    // ��ǽ
    Dfs(px, py);
}

bool CanMove(int px, int py, int d)
{
    int x = px + dir[d].x;
    int y = py + dir[d].y;
    return CheckBlockType(x, y, 1, false);
}

void HandleWin()
{
    if (px == 49 && py == 29)
        Run();
}

void PlayerControl()
{
    // ���ɽ�ɫ
    py = px = 1;
    FillStr(px, py, "��");
    // ���ƽ�ɫ�ƶ�
    while (true)
    {
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                FillStr(px, py, "  ");
                switch (getch())
                {
                case 75: // С�������
                    if (CanMove(px, py, 0))
                        --px;
                    break;
                case 77: // С�����Ҽ�
                    if (CanMove(px, py, 2))
                        ++px;
                    break;
                case 72: // С�����ϼ�
                    if (CanMove(px, py, 1))
                        --py;
                    break;
                case 80: // С�����¼�
                    if (CanMove(px, py, 3))
                        ++py;
                    break;
                }
                FillStr(px, py, "��");
            }
        }
        HandleWin();
    }
}

void Run()
{
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
    FillStr(49, 29, "��");
    PlayerControl();
}

int main()
{
    // ���ÿ���̨����
    SetConsole("�Թ�", col << 1, row, "80");
    // ��ʼ����Ϸ
    Initialize();
    // ������Ϸ
    Run();
}