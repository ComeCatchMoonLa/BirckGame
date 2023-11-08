#include "../Engine/BrickEngine.h"

const int BW(25), BH(30); // BW��ʾ�����Ŀ��, BH��ʾ�����ĸ߶�
// PlayerX��ʾ��ҵĺ�����, EnemyBX��EnemyBY�ֱ��ʾ�з��ӵ��ĺ�����ꡢ������
int PlayerX, EnemyBX, EnemyBY;
// Wall��ʾ�ô��Ƿ���ǽ, Bullet��ʾ�ô��Ƿ����ӵ�
bool Wall[25 * 30], Bullet[25 * 30];

void Run();
// ��ʾ���
void ShowPlayer()
{
    FillStr(PlayerX, BH - 2, "��");
    FillStr(PlayerX - 1, BH - 1, "������");
}
// �������
void GameOver()
{
    // �����м���һ��հ�����, Ȼ�������ʾGame Over!
    FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
    FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
    // �ı������ʾ��ʽ
    FillStr(PlayerX, BH - 2, "��");
    FillStr(PlayerX - 1, BH - 1, "������");
    // ����ӵ�
    FillStr(EnemyBX, EnemyBY, "  ");
    Pause();
    Run();
}
// ��ʾǽ��
void ShowWall()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Wall[idx])
            FillStr(idx % BW, idx / BW, "��");
}
// ���ǽ��
void AddWall()
{
    // ���ж�����Ƿ�����
    int n = BW * (BH - 3) + BW - 1; // ������3�����һ������
    for (int idx = n - BW + 1; idx < n; ++idx)
        if (Wall[idx]) // ���������3�д���ǽ��, ���������
            GameOver();
    // ��������ǽ��
    for (int idx = BW * (BH - 1) - 1; idx >= 0; --idx)
        if (Wall[idx])
        {
            Wall[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            Wall[idx + BW] = true;
        }
    // ����ǽ�������ƶ�һ��, ��ʱ���Ϸ����пճ�����, Ϊ�ճ������������ǽ��
    for (int x = 1; x < BW - 1; ++x)
        if (rand() & 1) // ��һ��ĸ�������ǽ��
            Wall[x] = true;
    // ��ʾ�ƶ����ǽ��
    ShowWall();
}
// ��ʾ�ӵ�
void ShowBullet()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
            FillStr(idx % BW, idx / BW, "��");
}
// ����ӵ��ƶ�
void PlayerBulletMove()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            if (idx > BW) // �ӵ�δ�ִ�߽�
            {
                if (Wall[idx - BW]) // �ӵ���һλ����ǽ,�ӵ���ǽһͬ����
                {
                    Wall[idx - BW] = false;
                    FillStr(idx % BW, idx / BW - 1, "  ");
                }
                else
                    Bullet[idx - BW] = true;
            }
        }
    // ��ʾ�ƶ�����ӵ�
    ShowBullet();
}
// �����ӵ��ƶ�
void EnemyBulletMove()
{
    if (!EnemyBX) // �����ӵ��Ƿ����
        return;
    if ((EnemyBY == BH - 2 && (EnemyBX == PlayerX - 1 || EnemyBX == PlayerX + 1)) ||
        EnemyBY == BH - 3 && EnemyBX == PlayerX) // �ӵ��������
        GameOver();
    if (!EnemyBY)
    {
        int MaxY = 0; // �з��ӵ���������Wall�����������
        for (int y = BH - 2; y >= 0; --y)
            if (Wall[y * BW + EnemyBX])
            {
                MaxY = y;
                break;
            }
        EnemyBY = MaxY + 1;
        FillStr(EnemyBX, EnemyBY, "��");
    }
    if (EnemyBY < BH - 1) // �ӵ�δ�ִ�߽�
    {
        // �з��ӵ�������ӵ�����С��2��, �ֱ��ƶ�1��������ӵ����ᷢ����ײ
        for (int i = 1; i < 3; ++i)
            if (Bullet[(EnemyBY + i) * BW + EnemyBX])
            {
                Bullet[(EnemyBY + i) * BW + EnemyBX] = false;
                FillStr(EnemyBX, EnemyBY, "  ");
                FillStr(EnemyBX, EnemyBY + i, "  ");
                EnemyBX = 0;
                return;
            }
        ++EnemyBY;
        FillStr(EnemyBX, EnemyBY - 1, "  ");
        FillStr(EnemyBX, EnemyBY, "��");
    }
    else // �ӵ��ִ�߽�
    {
        FillStr(EnemyBX, EnemyBY, "  ");
        EnemyBX = 0;
    }
}
// ������Ϸ
void Run()
{
    // ��ʼ����Ϸ
    system("cls");
    for (bool &w : Wall)
        w = false;
    for (bool &b : Bullet)
        b = false;
    PlayerX = (BW - 1 >> 1);
    EnemyBX = EnemyBY = 0;
    ShowPlayer();
    AddWall();

    int t = 0;
    while (true)
    {
        Sleep(1);
        ++t &= 511; // t��ȡֵ��Χ0~511, tÿ�μ�1, ����511ʱֵ��Ϊ0
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224) // ����ƶ�
            {
                // �����ɫ
                FillStr(PlayerX, BH - 2, "  ");
                FillRec(PlayerX - 1, BH - 1, 3, 1, "  ");

                switch (getch())
                {
                case 75: // ����
                    PlayerX -= PlayerX != 1;
                    break;
                case 77: // ����
                    PlayerX += PlayerX != BW - 2;
                    break;
                }
                ShowPlayer();
            }
            else if (ch == 122) // ������
            {
                Bullet[BW * (BH - 3) + PlayerX] = true;
                ShowBullet();
            }
            else if (ch == 32) // ��ͣ��Ϸ
                Pause();
        }
        if (!(t & 1)) // �ӵ��ƶ�
        {
            PlayerBulletMove();
            EnemyBulletMove();
        }
        if (!(t & 127)) // ����µ�һ��
            AddWall();
        if (!(t & 511)) // �������
        {
            EnemyBX = rand() % (BW - 3) + 1; // 1��23
            EnemyBY = 0;
        }
    }
}

int main()
{
    SetConsole("�������", 50, 30, "80");
    srand((int)time(0));
    Run();
}