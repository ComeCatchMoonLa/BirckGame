#include "../Engine/BrickEngine.h"

const int BW(15), BH(20); // BW��ʾ�����Ŀ��, BH��ʾ�����ĸ߶�
int PlayerX;              // PlayerX��ʾ��ҵĺ�����
// Wall��ʾ�ô��Ƿ���ǽ, Bullet��ʾ�ô��Ƿ����ӵ�
bool Wall[15 * 20], Bullet[15 * 20];

void Run();
// ��ʾ��ɫ
void ShowRole()
{
    FillStr(PlayerX, BH - 2, "��");
    FillStr(PlayerX - 1, BH - 1, "������");
}
// �������
void PlayerDead()
{
    int n = BW * (BH - 3) + BW - 1; // ������3�����һ������
    for (int idx = n - BW + 1; idx < n; ++idx)
        if (Wall[idx]) // ���������3�д���ǽ��, ���������
        {
            // �����м���һ��հ�����, Ȼ�������ʾGame Over!
            FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
            FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
            // �ı������ʾ��ʽ
            FillStr(PlayerX, BH - 2, "��");
            FillStr(PlayerX - 1, BH - 1, "������");
            Pause();
            Run();
        }
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
    PlayerDead();
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
// �ƶ��ӵ�
void MoveBullet()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            if (idx > BW) // �ӵ�δ�ִ�߽�
            {
                if (Wall[idx - BW]) // �ӵ���һλ����ǽ, �ӵ����ǽ
                    Wall[idx] = true;
                else // �ӵ���һλ��Ϊ��, �ӵ��ƶ�
                {
                    Bullet[idx - BW] = true;
                    FillStr(idx % BW, idx / BW, "  ");
                }
            }
            else // �ӵ��ִ�߽�, �ӵ����ǽ
                Wall[idx] = true;
        }
    // ��ʾ�ƶ�����ӵ�
    ShowBullet();
}
// ��������
void LineClear()
{
    int count = 0; // ��¼���е�ǽ������
    for (int idx = 0; idx < BW * BH; ++idx)
    {
        if (Wall[idx])
            ++count;
        if (!((idx + 1) % BW)) // ɨ�赽���н�β
        {
            if (count == BW - 2) // ��������, ��������
            {
                for (int i = idx - BW; i < BW * BH; ++i)
                    if (Wall[i])
                    {
                        Wall[i] = false;
                        FillStr(i % BW, i / BW, "  ");
                        if (i > idx)
                            Wall[i - BW] = true;
                    }
                ShowWall();
                idx -= BW; // ��������, ����ɨ����һ��
            }
            count = 0;
        }
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
    ShowRole();
    AddWall();

    int t = 0;
    while (true)
    {
        Sleep(1);
        ++t &= 255; // t��ȡֵ��Χ0~255, ��t����255ʱ, t��1ֵ��Ϊ0
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
                ShowRole();
            }
            else if (ch == 122) // ������
            {
                Bullet[BW * (BH - 3) + PlayerX] = true;
                ShowBullet();
            }
            else if (ch == 32) // ��ͣ��Ϸ
                Pause();
        }
        if (!(t & 1)) // 2�ı���
        {
            MoveBullet();
            LineClear();
        }
        if (!(t & 255)) // 255�ı���
            AddWall();
    }
}

int main()
{
    SetConsole("����", 30, 20, "80");
    srand((int)time(0));
    Run();
}