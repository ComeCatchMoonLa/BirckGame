#include "../Engine/BrickEngine.h"

const int BW(40), BH(30); // BW��ʾ�����Ŀ��, BH��ʾ�����ĸ߶�
enum Sharp
{
    Sharp1 = 1,
    Sharp2,
    Sharp3,
    Sharp4,
};
Sharp TS1, TS2, TS3, YS1, YS2, YS3; // TS��ʾĿ��ͼ��, YS��ʾ���ͼ��
int TSy(5), score(0);               // TSy��ʾĿ��ͼ�ε�������, score��ʾ��õķ���

void Run();
// ���ͼ��
void ClearGraph(int x, int y)
{
    FillRec(x, y, 2, 2, "  ");
}
// �����������ͼ��
void ClearAllGraph(int x1, int x2, int x3, int y)
{
    FillRec(x1, y, 2, 2, "  ");
    FillRec(x2, y, 2, 2, "  ");
    FillRec(x3, y, 2, 2, "  ");
}
// ��ʾͼ��
void ShowGraph(int x, int y, const Sharp &s)
{
    switch (s)
    {
    case 1:
        FillRec(x, y + 1, 1, 1, "��");
        break;
    case 2:
        FillRec(x, y, 1, 2, "��");
        break;
    case 3:
        FillArea(x, y, {{0, 0}, {0, 1}, {1, 1}}, "��");
        break;
    case 4:
        FillRec(x, y, 2, 2, "��");
        break;
    }
}
// ��ʾ��������ͼ��
void ShowAllGraph(int x1, int x2, int x3, int y, const Sharp &s1, const Sharp &s2, const Sharp &s3)
{
    ShowGraph(x1, y, s1);
    ShowGraph(x2, y, s2);
    ShowGraph(x3, y, s3);
}
// ����µ�Ŀ��ͼ��
void AddGarph(int y, Sharp &s1, Sharp &s2, Sharp &s3)
{
    s1 = Sharp((rand() & 3) + 1);
    s2 = Sharp((rand() & 3) + 1);
    s3 = Sharp((rand() & 3) + 1);
    ShowAllGraph(10, 20, 30, y, s1, s2, s3);
}
// Ŀ��ͼ���½�
void DropGraph(int &y, Sharp &s1, Sharp &s2, Sharp &s3)
{
    if (y < BH - 4) // Ŀ��ͼ�κ����ͼ�λ�δ�Ӵ�, Ŀ��ͼ��ֱ���½�
    {
        ClearAllGraph(10, 20, 30, y);
        ++y;
        ShowAllGraph(10, 20, 30, y, s1, s2, s3);
    }
    else if (s1 == YS1 && s2 == YS2 && s3 == YS3) // Ŀ��ͼ�νӴ��������ͼ��, �ж���״�Ƿ�һ��, ����״һ���������µ�ͼ��
    {
        ClearAllGraph(10, 20, 30, y);
        ++score;
        y = 5;
        AddGarph(y, s1, s2, s3);
    }
    else // ����״��һ��, ����ʾ��Ϸ����
    {
        FillStr((BW >> 1) - 2, (BH >> 1), "Game Over!");
        Pause();
        Run();
    }
}
// �ı����ͼ�ε���״
void ChangeSharp(int x, Sharp &s)
{
    ClearGraph(x, BH - 2);
    Sharp TmpSharp;
    do
    {
        TmpSharp = Sharp((rand() & 3) + 1); // Sharp���͵�ֵֻ����1��2��3��4
    } while (s == TmpSharp);
    s = TmpSharp;
    ShowGraph(x, BH - 2, s);
}
// ������Ϸ
void Run()
{
    // ��ʼ��
    system("cls");
    TSy = 5;
    AddGarph(BH - 2, YS1, YS2, YS3);
    AddGarph(TSy, TS1, TS2, TS3);

    int time(0);
    while (true)
    {
        Sleep(1);
        if (++time > 20 - score * 0.2)
        {
            DropGraph(TSy, TS1, TS2, TS3);
            time = 0;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // ���¼������
                    ChangeSharp(10, YS1);
                    break;
                case 72: // ���¼����ϼ�
                    ChangeSharp(20, YS2);
                    break;
                case 77: // ���¼����Ҽ�
                    ChangeSharp(30, YS3);
                    break;
                }
            }
            else if (ch == 122) // ����
                DropGraph(TSy, TS1, TS2, TS3);
            else if (ch == 32) // ��ͣ
                Pause();
        }
    }
}

int main()
{
    SetConsole("��������", 80, 30, "80");
    srand((int)time(0));
    Run();
}