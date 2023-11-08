#include "../Engine/BrickEngine.h"

void run();
std::list<Vector2> snack;   // ̰����
int FoodX, FoodY, SnackD; // FoodX��ʾʳ��ĺ�����, FoodY��ʾʳ���������, SnackD��ʾ̰���ߵķ���
bool Dead;                // Dead��ʾ̰�����Ƿ�����

// �ƶ�̰����
void MoveSnack(std::list<Vector2> &snack)
{
    // ������ͷ�����������
    int dx = 0, dy = 0;
    SnackD & 1 ? dx = 2 - SnackD : dy = SnackD - 1;
    snack.insert(begin(snack), {snack.front().x + dx, snack.front().y + dy});
    // ��ʾ̰����
    FillStr(snack.front().x, snack.front().y, "��");
    FillStr((*++snack.begin()).x, (*++snack.begin()).y, "��");
    // û�Ե�ʳ�����̰����β��
    if (snack.front().x != FoodX || snack.front().y != FoodY)
    {
        FillStr(snack.back().x, snack.back().y, "  ");
        snack.pop_back();
    }
    else // �Ե�ʳ��
    {
        bool food_CD = false;
        while (true) // ����ʳ������
        {
            FoodX = rand() % 40, FoodY = rand() % 25;
            for (Vector2 &s : snack)
                if (s.x == FoodX && s.y == FoodY) // �ж�ʳ�������Ƿ�������
                {
                    food_CD = true;
                    break;
                }
            if (!food_CD)
                break;
            food_CD = false;
        }
        FillStr(FoodX, FoodY, "��"); // ��ʾʳ��
    }
    // �ж�����
    for (std::list<Vector2>::iterator it = ++snack.begin(); it != snack.end(); ++it)
        if ((*it).x == snack.front().x && (*it).y == snack.front().y)
            Dead = true;
    if (!Dead && (snack.front().x < 0 || snack.front().x > 39 || snack.front().y < 0 || snack.front().y > 24))
        Dead = true;
    if (Dead) // ��Ϸ����
    {
        FillStr(17, 12, "Game Over!");
        FillStr(13, 13, "press Spacebar to restart...");
        Pause();
        run();
    }
}

// ������Ϸ
void run() 
{
    // ��ʼ����Ϸ
    system("cls");
    FoodX = 20, FoodY = 16, SnackD = 1, Dead = false, snack = {{15, 17}, {14, 17}, {13, 17}};
    FillStr(FoodX, FoodY, "��");

    int t = 0;         // ����̰���ߵ��ٶ�
    bool move = false; // ʹ̰���ߣ�ÿ�ƶ�һ������ı�һ�η���
    while (true)
    {
        Sleep(30);
        if (t++ > 10)
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
            else if (ch == 32)  // ��ͣ��Ϸ | ���¿�ʼ
                Pause();
        }
    }
}

int main()
{
    SetConsole("̰����", 80, 25, "80");
    srand((int)time(0));
    run();
}