#include "../Engine/BrickEngine.h"

const BW(15), BH(30);       // BW��ʾ�����Ŀ�, BH��ʾ�����ĸ�
const RacingW(3), RoadW(5); // RW��ʾ�����Ŀ�, RoadW��ʾ·�Ŀ�
int PlayerX;                // PlayerX��ʾ�����ĺ�����
int Road[BH];               // ��¼ÿһ��·��ߵ�λ��
void Run();

// ��ʾ���
void ShowPlayer()
{
    FillStr(PlayerX + 1, BH - 4, "��");
    FillStr(PlayerX, BH - 3, "������");
    FillStr(PlayerX + 1, BH - 2, "��");
    FillStr(PlayerX, BH - 1, "��  ��");
}
// ������
void ClearPlayer()
{
    FillRec(PlayerX, BH - 4, 3, 4, "  ");
}
// ��ʾ\�����·
void FillRoad(int y, const std::string &fill)
{
    if (BH - 4 <= y && y <= BH - 1 && !(Road[y] <= PlayerX && PlayerX <= Road[y] + RacingW - 1))
    {
        FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
        FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
        Pause();
        Run();
    }
    FillRec(0, y, Road[y], 1, fill);
    FillRec(Road[y] + RoadW, y, BW - RoadW - Road[y], 1, fill);
}
// ������·
void UpdataRoad()
{
    for (int y = BH - 1; y > 0; --y)
        if (Road[y] != Road[y - 1])
        {
            FillRoad(y, "  ");
            Road[y] = Road[y - 1];
            FillRoad(y, "��");
        }
    if (Road[1] == Road[2] && Road[2] == Road[3] && Road[3] == Road[4] && Road[4] == Road[5])
    {
        FillRoad(0, "  ");
        while (true)
        {
            Road[0] += ((rand() & 1) ? 1 : -1) * ((rand() & 1) + 1);
            if (Road[0] > 0 && Road[0] + RoadW < BW - 1)
                break;
        }
        FillRoad(0, "��");
    }
}
// ��ʼ����Ϸ
void Initialize()
{
    system("cls");
    // ��ʼ������λ��
    PlayerX = BW - 2 >> 1;
    // ��ʾ����
    ShowPlayer();
    // ��ʼ����·
    int RootPos = BW - RoadW >> 1;
    for (int &pos : Road)
        pos = RootPos;
    // ������·
    for (int y = BH - 1; y >= 0; --y)
        FillRoad(y, "��");
}
// ������Ϸ
void Run()
{
    Initialize();
    int t = 0;
    while (true)
    {
        Sleep(1);
        ++t &= 31;
        if (!(t & 31))
            UpdataRoad();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                ClearPlayer();
                switch (getch())
                {
                case 75: // ����С�������
                {
                    bool CanMove = PlayerX != 0;
                    for (int i = 1; i <= 4; ++i)
                        CanMove &= Road[BH - i] < PlayerX;
                    if (CanMove)
                        --PlayerX;
                }
                break;
                case 77: // ����С�����Ҽ�
                {
                    bool CanMove = PlayerX != BW - RacingW;
                    for (int i = 1; i <= 4; ++i)
                        CanMove &= PlayerX < Road[BH - i] + RacingW - 1;
                    if (CanMove)
                        ++PlayerX;
                }
                break;
                }
                ShowPlayer();
            }
            else if (ch == 122)
                UpdataRoad();
            else if (ch == 32)
                Pause();
        }
    }
}

int main()
{
    SetConsole("��������", 30, 30, "80");
    srand((int)time(0));
    Run();
}