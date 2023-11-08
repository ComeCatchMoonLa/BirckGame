#include "../Engine/BrickEngine.h"
#include "Map.h"

void run();
std::list<Vector2> snack; // ̰����
// ʳ��λ�á�̰���߷���̰�����ٶȡ��ؿ�������ҵ÷�
int FoodX, FoodY, SnackD, Speed(1), MapNum(1), Score;
bool Dead;       // ̰�����Ƿ�����
std::string Map; // �ؿ�

void LogMap(std::string Map) // �����ߵ����Բ���ӡ��ͼ
{
    if (MapNum == 5 || MapNum == 6)
        SnackD = 0, snack = {{36, 24}, {36, 25}, {36, 26}};
    else
        SnackD = 1, snack = {{16, 17}, {15, 17}, {14, 17}};
    SetPos(0, 4);
    for (int i = 0; i < 40 * 26; ++i)
        std::cout << (Map[i] == '0' ? "��" : "  ");
}
void LogSelectMap() // ��ӡԤ����ͼ
{
    Map = Maps[MapNum - 1];
    LogMap(Map);
    FillRec(14, 13, 12, 8, "  ");
    FillStr(18, 15, " Map: " + std::to_string(MapNum));
    FillStr(18, 17, "Speed: " + std::to_string(Speed));
}
void AddFood() // ����ʳ��
{
    bool food_CD = false;
    while (true)
    {
        FoodX = rand() % 40, FoodY = rand() % 24 + 5;
        if (Map[FoodX + (FoodY - 4) * 40] == '0')
            food_CD = true;
        else
            for (const Vector2 &s : snack)
                if (s.x == FoodX && s.y == FoodY)
                {
                    food_CD = true;
                    break;
                }
        if (!food_CD)
            break;
        food_CD = false;
    }
    FillStr(FoodX, FoodY, "��");
}
void SelectMap() // ѡ���ͼ
{
    while (true)
    {
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // С�������
                    MapNum -= MapNum != 1;
                    break;
                case 77: // С�����Ҽ�
                    MapNum += MapNum != 6;
                    break;
                case 72: // С�����ϼ�
                    Speed += Speed != 5;
                    break;
                case 80: // С�����¼�
                    Speed -= Speed != 1;
                    break;
                }
                LogSelectMap();
                FillStr(9, 2, ' ' + std::to_string(MapNum));
                FillStr(32, 2, std::to_string(Speed));
            }
            else if (ch == 32)
                break;
        }
    }
}
void MoveSnack(std::list<Vector2> &snack) // �ƶ�̰����
{
    // ������ͷ�����������
    int dx = 0, dy = 0;
    SnackD & 1 ? dx = 2 - SnackD : dy = SnackD - 1;
    snack.insert(begin(snack), {snack.front().x + dx, snack.front().y + dy});
    // �ж��Ƿ�Ե�ʳ��
    if (snack.front().x != FoodX || snack.front().y != FoodY) // û�Ե�ʳ�����̰����β��
    {
        FillStr(snack.back().x, snack.back().y, "  ");
        snack.pop_back();
    }
    else // �Ե�ʳ��
    {
        SetPos(20, 2);
        std::cout << std::setw(3) << ++Score;
        AddFood();
    }
    // �ж�����
    if (Map[snack.front().x + (snack.front().y - 4) * 40] == '0')
        Dead = true;
    else
        for (std::list<Vector2>::iterator it = ++snack.begin(); it != snack.end(); ++it)
            if ((*it).x == snack.front().x && (*it).y == snack.front().y)
                Dead = true;
    // ��ʾ̰����
    if (Dead)
        for (const Vector2 &s : snack)
            FillStr(s.x, s.y, "x");
    else
    {
        FillStr(snack.front().x, snack.front().y, "��");
        FillStr((*++snack.begin()).x, (*++snack.begin()).y, "��");
    }
    if (Dead) // ��Ϸ����
    {
        FillStr(17, 16, "Game Over!");
        FillStr(13, 17, "press Spacebar to restart...");
        Pause();
        run();
    }
}
void Initialize() // ��ʼ����Ϸ
{
    system("cls");
    Dead = false, Score = 0;
    FillStr(6, 1, "==========\t\t==============\t\t===========\n");
    FillStr(6, 2, "| Map: " + std::to_string(MapNum) + " |\t\t| SCORE:   " + std::to_string(Score) + " |\t\t| SPEED:" + std::to_string(Speed) + " |");
    FillStr(6, 3, "==========\t\t==============\t\t===========\n");
    LogSelectMap(); // ��ӡԤѡ��ͼ
    SelectMap();    // ѡ���ͼ
    LogMap(Map);    // ��ӡ��ѡ��ͼ
    AddFood();      // ���ʳ��
}
void run() // ������Ϸ
{
    Initialize();
    int t = 0;         // ����̰���ߵ��ٶ�
    bool move = false; // ʹ̰���ߣ�ÿ�ƶ�һ������ı�һ�η���
    while (true)
    {
        Sleep(1);
        if (t++ > 30 - Speed * 5)
        {
            MoveSnack(snack);
            move = true, t = 0;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 224 && move) // �ı�̰���ߵķ���
            {
                ch = getch();
                SnackD = SnackD & 1 ? (ch == 72 ? 0 : (ch == 80 ? 2 : SnackD)) : (ch == 75 ? 3 : (ch == 77 ? 1 : SnackD));
                move = false;
            }
            else if (ch == 122) // �����ƶ�
                MoveSnack(snack);
            else if (ch == 32) // ��ͣ��Ϸ
                Pause();
        }
    }
}

int main()
{
    SetConsole("̰����", 80, 30, "80");
    srand((int)time(0));
    run();
}