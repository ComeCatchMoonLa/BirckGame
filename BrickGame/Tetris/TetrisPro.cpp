#include "../Engine/BrickEngine.h"

const int BW(14);             // BW��ʾ�������
const int BH(24);             // BH��ʾ�����߶�
const int BX(1);              // BX��ʾ�����ĺ�����
const int BY(0);              // BY��ʾ������������
const int HomeX(BW - 1 >> 1); // HomeX��ʾͼ�γ�ʼ������
const int HomeY(1);           // HomeY��ʾͼ�γ�ʼ������
bool Board[BW * BH];          // Board[x + y * BW]��ʾ(x, y)���Ƿ�ΪС����
int sharp;                    // ͼ�ε���״(��״�������)
int x, y;                     // ͼ�εĺ�������
int score;                    // ��Ϸ�÷�
int speed;                    // ͼ��������ٶ�

// ������19��ͼ��
// ͼ��4�����ص������˳���޹ؽ�Ҫ
// ��ÿ��ͼ�α��밴��ת�任��˳����������
Vector2 From[19][4] = {
    // ����
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    // ����
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
    // L��
    {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
    {{2, 0}, {1, 0}, {0, 0}, {0, 1}},
    {{2, 2}, {2, 1}, {2, 0}, {1, 0}},
    {{0, 2}, {1, 2}, {2, 2}, {2, 1}},
    // ����L��
    {{2, 0}, {2, 1}, {2, 2}, {1, 2}},
    {{2, 2}, {1, 2}, {0, 2}, {0, 1}},
    {{0, 2}, {0, 1}, {0, 0}, {1, 0}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
    // T��
    {{1, 1}, {0, 2}, {1, 2}, {2, 2}},
    {{1, 1}, {0, 0}, {0, 1}, {0, 2}},
    {{1, 1}, {2, 0}, {1, 0}, {0, 0}},
    {{1, 1}, {2, 0}, {2, 1}, {2, 2}},
    // ������
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    {{2, 0}, {1, 0}, {1, 1}, {0, 1}},
    // ����������
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
    {{1, 0}, {1, 1}, {0, 1}, {0, 2}}};

void run();
// �������ת��������
Vector2 SiteChange(Vector2 p)
{
    return {x + p.x, y + p.y};
}
// ��ʾ�����ͼ�εĸ�������
void ShowOrClear(bool exist, const std::string &fill)
{
    for (int i = 0; i < 4; ++i)
    {
        Vector2 Pixel = SiteChange(From[sharp][i]);
        if (Board[Pixel.x + Pixel.y * BW] == exist) // �ô��Ѵ���ͼ��, ��Ϸ����
        {
            SetPos(7, BH + 1);
            std::cout << "Game Over!";
            Pause();
            run();
        }
        Board[Pixel.x + Pixel.y * BW] = exist;
        FillStr(Pixel.x + BX, Pixel.y + BY, fill);
    }
}
// ��ʾͼ��
void ShowGraph() { ShowOrClear(true, "��"); }
// ���ͼ��
void ClearGraph() { ShowOrClear(false, "  "); }
// ���ͼ��
void AddGraph()
{
    sharp = rand() % 19;
    x = HomeX;
    y = HomeY;
    ShowGraph();
}
// ��������
void LineRemove()
{
    int JudgeY = BH - 1; // ��ǰ�жϵ�����
    while (JudgeY >= 0)
    {
        int cnt = 0;
        for (int i = 0; i < BW; ++i)
            if (Board[BW * JudgeY + i])
                ++cnt;
        if (cnt != BW) // δ����, �ж���һ��
            --JudgeY;
        else // ��������
        {
            // �Ƴ�JudgeY��һ��
            for (int i = 0; i < BW; ++i)
            {
                Board[i + JudgeY * BW] = false;
                FillStr(i + BX, JudgeY + BY, "  ");
            }
            // ��JudgeY�����Ϸ������з�������һ��
            for (int i = BW * JudgeY - 1; i >= 0; --i)
                if (Board[i])
                {
                    Board[i] = false;
                    FillStr(i % BW + BX, i / BW + BY, "  ");
                    if (!Board[i + BW])
                    {
                        Board[i + BW] = true;
                        FillStr(i % BW + BX, i / BW + 1 + BY, "��");
                    }
                }
            ++score;
            std::cout << std::setw(4);
            FillStr(BW + 3, 4, std::to_string(score));
            std::cout << std::setw(2);
            FillStr(BW + 4, 8, std::to_string(speed));
        }
    }
}
// ��ײ��⣺ͼ���½�
bool Drop_CD()
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // ���õ��Ƿ�Ϊ�����±ߵ����ص�
        Vector2 Pixel0 = SiteChange(From[sharp][i]);

        // �Ƿ�Խ��
        if (Pixel0.y > BH - 2)
            return true;
        // �Ƿ������ϰ���
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.x == Pixel0.x && Pixel1.y > Pixel0.y)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x + (Pixel0.y + 1) * BW])
            return true;
    }
    return false;
}
// ͼ���½�
void Drop()
{
    if (!Drop_CD())
    {
        ClearGraph();
        ++y;
        ShowGraph();
    }
    else
    {
        LineRemove();
        AddGraph();
    }
}
// ��ײ��⣺ͼ������
bool Left_CD()
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // ���õ��Ƿ�Ϊ��������ߵ����ص�
        Vector2 Pixel0 = SiteChange(From[sharp][i]);

        // �Ƿ�Խ��
        if (Pixel0.x < 1)
            return true;
        // �Ƿ������ϰ���
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.y == Pixel0.y && Pixel1.x < Pixel0.x)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x - 1 + Pixel0.y * BW])
            return true;
    }
    return false;
}
// ͼ������
void Left()
{
    if (!Left_CD())
    {
        ClearGraph();
        --x;
        ShowGraph();
    }
}
// ��ײ��⣺ͼ������
bool Right_CD()
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // ���õ��Ƿ�Ϊ�������ұߵ����ص�
        Vector2 Pixel0 = SiteChange(From[sharp][i]);

        // �Ƿ�Խ��
        if (Pixel0.x > BW - 2)
            return true;
        // �Ƿ������ϰ���
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.y == Pixel0.y && Pixel1.x > Pixel0.x)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x + 1 + Pixel0.y * BW])
            return true;
    }
    return false;
}
// ͼ������
void Right()
{
    if (!Right_CD())
    {
        ClearGraph();
        ++x;
        ShowGraph();
    }
}
// ��ײ��⣺ͼ����ת
bool Ratote_CD(int NextSharp)
{
    for (int i = 0; i < 4; ++i)
    {
        bool Vector2_CD = true; // ���õ���һλ���Ƿ����������ص�
        Vector2 Pixel0 = SiteChange(From[NextSharp][i]);

        // �Ƿ�Խ��
        if (Pixel0.x < 0 || BW - 1 < Pixel0.x || Pixel0.y > BH - 1)
            return true;
        // �Ƿ������ϰ���
        for (int j = 0; j < 4; ++j)
        {
            Vector2 Pixel1 = SiteChange(From[sharp][j]);
            if (Pixel1.x == Pixel0.x && Pixel1.y == Pixel0.y)
            {
                Vector2_CD = false;
                break;
            }
        }
        if (Vector2_CD && Board[Pixel0.x + Pixel0.y * BW])
            return true;
    }
    return false;
}
// ͼ����ת
void Ratote()
{
    int NextSharp = sharp;
    if (sharp == 6 || sharp == 10 || sharp == 14)
        NextSharp -= 3;
    else if (sharp == 1 || sharp == 16 || sharp == 18)
        --NextSharp;
    else if (sharp != 2)
        ++NextSharp;
    if (!Ratote_CD(NextSharp))
    {
        ClearGraph();
        sharp = NextSharp;
        ShowGraph();
    }
}
// ��ʼ����Ϸ
void Initialize()
{
    system("cls");
    AddGraph();
    score = 0;
    speed = 1;
    for (bool b : Board)
        b = false;
    // ��ʼ������
    FillRec(BX - 1, BY, 1, BH, "��");
    FillRec(BX + BW, BY, 1, BH, "��");
    FillRec(BX, BY + BH, BW * 2 - 1, 1, "��");
    FillStr(BX - 1, BY + BH, "�t");
    FillStr(BX + BW, BY + BH, "�s");

    FillStr(BW + 2, 3, " SCORE");
    std::cout << std::setw(4);
    FillStr(BW + 3, 4, std::to_string(score));
    FillStr(BW + 2, 7, " SPEED");
    std::cout << std::setw(2);
    FillStr(BW + 4, 8, std::to_string(speed));
}
// ������Ϸ
void run()
{
    Initialize();

    int t = 0;
    while (true)
    {
        // ʱ�临�Ӷ�O(BW * BH * BH)
        Sleep(1);
        speed = score / 5 + 1;
        if (t++ > (30 - speed))
        {
            Drop();
            t = 0;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 72:      // ���¼����ϼ�
                    Ratote(); // ��תͼ��
                    break;
                case 80:    // ���¼����¼�
                    Drop(); // �½�ͼ��
                    break;
                case 75:    // ���¼������
                    Left(); // ����ͼ��
                    break;
                case 77:     // ���¼����Ҽ�
                    Right(); // ����ͼ��
                    break;
                }
            }
            else if (ch == 32)
                Pause();
        }
    }
}

int main()
{
    SetConsole("����˹����", 40, 27, "80");
    srand((int)time(0));
    run();
}