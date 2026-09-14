#include "../Engine/BrickEngine.h"

// BW琛ㄧず鍦烘櫙鐨勫�藉害, BH琛ㄧず鍦烘櫙鐨勯珮搴�, PW琛ㄧず鐜╁�剁殑瀹藉害, PH琛ㄧず鐜╁�剁殑楂樺害, PS琛ㄧず鐜╁�剁殑姝ラ暱
const int BW(160), BH(120), PW(8), PH(8), PS(4);
// PlayerX琛ㄧず鐜╁�剁殑妯�鍧愭爣, PlayerY琛ㄧず鐜╁�剁殑绾靛潗鏍�, ballnum琛ㄧず瀛愬脊鐨勬暟閲�
int PlayerX(BW >> 1), PlayerY(BH >> 1), ballnum(10), Score;
struct Ball
{
    int BallX, BallY, BallD; // 寮圭悆鐨勬í鍧愭爣銆佺旱鍧愭爣銆佹柟鍚�
    Ball(int BallX, int BallY, int BallD)
        : BallX(BallX), BallY(BallY), BallD(BallD)
    {
    }
};
// 鐜╁�惰�掕壊鍍忕礌鐐归泦鍚� 8脳8
Sites Player = {
    {2, 0}, {3, 0}, {4, 0}, {5, 0}, 
    {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1},
    {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2},
    {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3},
    {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4},
    {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5},
    {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6},
    {2, 7}, {3, 7}, {4, 7}, {5, 7}, 
};
std::vector<Ball> ball;

// 娣诲姞寮圭悆
void AddBall()
{
    int x(0), y(0);
    bool Ball_CD = false;
    while (true)
    {
        x = rand() % BW, y = rand() % BH;
        for (const Ball &b : ball)
            if (b.BallX == x && b.BallY == y)
            {
                Ball_CD = true;
                break;
            }
        if (!Ball_CD)
            break;
        Ball_CD = false;
    }
    ball.emplace_back(x, y, (rand() & 3) + 1);
}
// 鍒濆�嬪寲娓告垙
void Initialize(const int &BallNum)
{
    ball.reserve(BallNum);
    for (int i = BallNum; i--;)
        AddBall();
}
// 娓呴櫎寮圭悆
void ClearBall(const Ball &b)
{
    FillStr(b.BallX, b.BallY, "  ");
}
// 鏄剧ず寮圭悆
void ShowBall(const Ball &b)
{
    FillStr(b.BallX, b.BallY, "鈻�");
}
// 娓呴櫎鐜╁��
void ClearPlayer(int x, int y)
{
    FillRec(x, y, PW, PH, "  "); // 姣擣illArea()鍑芥暟鎬ц兘鏇翠紭
}
// 鏄剧ず鐜╁��
void AddPlayer(int x, int y)
{
    FillArea(x, y, Player, "鈻�");
}
// 鏀瑰彉鏂瑰悜鈥斺€旇緟鍔╁嚱鏁�
void ChangeD_Helper(int &BallD, bool CD1, bool CD2, int a, int b, int c)
{
    BallD = CD1 ? (CD2 ? a : b) : (!CD2 ?: c);
}
// 鏀瑰彉鏂瑰悜
void Change_D(Ball &b)
{
    switch (b.BallD)
    {
    case 1:
        ChangeD_Helper(b.BallD, b.BallX == BW - 1, !b.BallY, 3, 4, 2);
        break;
    case 2:
        ChangeD_Helper(b.BallD, b.BallX == BW - 1, b.BallY == BH - 1, 4, 3, 1);
        break;
    case 3:
        ChangeD_Helper(b.BallD, !b.BallX, b.BallY == BH - 1, 1, 2, 4);
        break;
    case 4:
        ChangeD_Helper(b.BallD, !b.BallX, !b.BallY, 2, 1, 3);
        break;
    }
}
// 绉诲姩寮圭悆鈥斺€旇緟鍔╁嚱鏁�
void MoveBall_Helper(Ball &b, bool CD1, bool CD2, int dx, int dy)
{
    if (CD1 && CD2)
        b.BallX += dx, b.BallY += dy;
    else
        Change_D(b);
}
// 绉诲姩寮圭悆
void MoveBall()
{
    for (Ball &b : ball)
    {
        ClearBall(b);
        switch (b.BallD)
        {
        case 1:
            MoveBall_Helper(b, b.BallY > 0, b.BallX < BW - 1, 1, -1);
            break;
        case 2:
            MoveBall_Helper(b, b.BallX < BW - 1, b.BallY < BH - 1, 1, 1);
            break;
        case 3:
            MoveBall_Helper(b, b.BallX > 0, b.BallY < BH - 1, -1, 1);
            break;
        case 4:
            MoveBall_Helper(b, b.BallX > 0, b.BallY > 0, -1, -1);
            break;
        }
        ShowBall(b);
    }
}
// 鐜╁�舵�讳骸
void PlayerDead()
{
    for (const Ball &b : ball)
        if ((b.BallX >= PlayerX && b.BallX <= PlayerX + PW) &&
            (b.BallY >= PlayerY && b.BallY <= PlayerY + PH))
        {
            FillArea(PlayerX, PlayerY, Player, "脳");
            if (Score > 0)
                SubmitScore(6, Score);
            Score = 0;
            Pause();
            AddPlayer(PlayerX, PlayerY);
        }
}
// 杩愯�屾父鎴�
void Run()
{
    Initialize(ballnum);
    AddPlayer(PlayerX, PlayerY);
    int t(0);
    Score = 0;
    while (true)
    {
        PumpFrame(); // t鐨勫彇鍊艰寖鍥�0~511, 褰搕绛変簬511鏃�, t鍔�1鍊煎彉涓�0
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            if (ch == 224)
            {
                ClearPlayer(PlayerX, PlayerY);
                switch (getch())
                {
                case 72: // 鎸変笅閿�鐩樹笂閿�
                    PlayerY -= PlayerY >= PS ? PS : 0;
                    break;
                case 80: // 鎸変笅閿�鐩樹笅閿�
                    PlayerY += PlayerY <= BH - PW - PS ? PS : 0;
                    break;
                case 75: // 鎸変笅閿�鐩樺乏閿�
                    PlayerX -= PlayerX >= PS ? PS : 0;
                    break;
                case 77: // 鎸変笅閿�鐩樺彸閿�
                    PlayerX += PlayerX <= BW - PH - PS ? PS : 0;
                    break;
                }
                AddPlayer(PlayerX, PlayerY);
            }
        }
        if (DueLogicTick())
        {
            ++t &= 511;
            ++Score;
        if (!(t & 1)) // 鎺у埗寮圭悆鐨勭Щ鍔ㄩ€熷害
        {
            MoveBall();
            PlayerDead();
        }
        if (!(t & 511)) // 鎺у埗娣诲姞寮圭悆鐨勬椂闂撮棿闅�
            AddBall();
        }
    }
}

int main()
{
    SetConsoleFromGameId(6);
    srand((int)time(0));
    Run();
}
