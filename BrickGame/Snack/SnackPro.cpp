#include "../Engine/BrickEngine.h"
#include "Map.h"

void run();
std::list<Vector2> snack; // 璐�鍚冭泧
// 椋熺墿浣嶇疆銆佽椽鍚冭泧鏂瑰悜銆佽椽鍚冭泧閫熷害銆佸叧鍗℃暟銆佺帺瀹跺緱鍒�
int FoodX, FoodY, SnackD, Speed(1), MapNum(1), Score;
bool Dead;       // 璐�鍚冭泧鏄�鍚︽�讳骸
std::string Map; // 鍏冲崱

void LogMap(std::string Map) // 璁剧疆铔囩殑灞炴€у苟鎵撳嵃鍦板浘
{
    if (MapNum == 5 || MapNum == 6)
        SnackD = 0, snack = {{36, 24}, {36, 25}, {36, 26}};
    else
        SnackD = 1, snack = {{16, 17}, {15, 17}, {14, 17}};
    SetPos(0, 4);
    for (int i = 0; i < 40 * 26; ++i)
        std::cout << (Map[i] == '0' ? "■" : "  ");
}
void LogSelectMap() // 鎵撳嵃棰勮�堝湴鍥�
{
    Map = Maps[MapNum - 1];
    LogMap(Map);
    FillRec(14, 13, 12, 8, "  ");
    FillStr(18, 15, " Map: " + std::to_string(MapNum));
    FillStr(18, 17, "Speed: " + std::to_string(GetMachineSpeed()));
}
void AddFood() // 鐢熸垚椋熺墿
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
    FillStr(FoodX, FoodY, "●");
}
void SelectMap() // 閫夋嫨鍦板浘
{
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            if (ch == 224)
            {
                switch (getch())
                {
                case 75: // 灏忛敭鐩樺乏閿�
                    MapNum -= MapNum != 1;
                    break;
                case 77: // 灏忛敭鐩樺彸閿�
                    MapNum += MapNum != 6;
                    break;
                }
                LogSelectMap();
                FillStr(9, 2, ' ' + std::to_string(MapNum));
                FillStr(32, 2, std::to_string(GetMachineSpeed()));
            }
            else if (ch == 32)
                break;
        }
    }
}
void MoveSnack(std::list<Vector2> &snack) // 绉诲姩璐�鍚冭泧
{
    // 鍦ㄩ摼琛ㄥご閮ㄦ彃鍏ュ潗鏍囧��
    int dx = 0, dy = 0;
    SnackD & 1 ? dx = 2 - SnackD : dy = SnackD - 1;
    snack.insert(begin(snack), {snack.front().x + dx, snack.front().y + dy});
    // 鍒ゆ柇鏄�鍚﹀悆鍒伴�熺墿
    if (snack.front().x != FoodX || snack.front().y != FoodY) // 娌″悆鍒伴�熺墿锛屼涪寮冭椽鍚冭泧灏鹃儴
    {
        FillStr(snack.back().x, snack.back().y, "  ");
        snack.pop_back();
    }
    else // 鍚冨埌椋熺墿
    {
        SetPos(20, 2);
        std::cout << std::setw(3) << ++Score;
        AddFood();
    }
    // 鍒ゆ柇姝讳骸
    if (Map[snack.front().x + (snack.front().y - 4) * 40] == '0')
        Dead = true;
    else
        for (std::list<Vector2>::iterator it = ++snack.begin(); it != snack.end(); ++it)
            if ((*it).x == snack.front().x && (*it).y == snack.front().y)
                Dead = true;
    // 鏄剧ず璐�鍚冭泧
    if (Dead)
        for (const Vector2 &s : snack)
            FillStr(s.x, s.y, "x");
    else
    {
        FillStr(snack.front().x, snack.front().y, "□");
        FillStr((*++snack.begin()).x, (*++snack.begin()).y, "■");
    }
    if (Dead) // 娓告垙缁撴潫
    {
        FillStr(17, 16, "Game Over!");
        FillStr(13, 17, "press Spacebar to restart...");
        SubmitScore(2, Score);
        Pause();
        run();
    }
}
void Initialize() // 鍒濆�嬪寲娓告垙
{
    ClearScreen();
    Dead = false, Score = 0;
    FillStr(6, 1, "==========\t\t==============\t\t===========\n");
    FillStr(6, 2, "| Map: " + std::to_string(MapNum) + " |\t\t| SCORE:   " + std::to_string(Score) + " |\t\t| SPEED:" + std::to_string(GetMachineSpeed()) + " |");
    FillStr(6, 3, "==========\t\t==============\t\t===========\n");
    LogSelectMap(); // 鎵撳嵃棰勯€夊湴鍥�
    SelectMap();    // 閫夋嫨鍦板浘
    LogMap(Map);    // 鎵撳嵃宸查€夊湴鍥�
    AddFood();      // 娣诲姞椋熺墿
}
void run() // 杩愯�屾父鎴�
{
    Initialize();
    bool move = false; // 浣胯椽鍚冭泧锛氭瘡绉诲姩涓€娆¤嚦澶氭敼鍙樹竴娆℃柟鍚�
    while (true)
    {
        PumpFrame();
        if (DueLogicTick())
        {
            MoveSnack(snack);
            move = true;
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            if (ch == 224 && move) // 鏀瑰彉璐�鍚冭泧鐨勬柟鍚�
            {
                ch = getch();
                SnackD = SnackD & 1 ? (ch == 72 ? 0 : (ch == 80 ? 2 : SnackD)) : (ch == 75 ? 3 : (ch == 77 ? 1 : SnackD));
                move = false;
            }
            else if (ch == 122) // 鍔犻€熺Щ鍔�
                MoveSnack(snack);
            else if (ch == 32) // 鏆傚仠娓告垙
                Pause();
        }
    }
}

int main()
{
    SetConsoleFromGameId(2);
    srand((int)time(0));
    run();
}
