#include "../Engine/BrickEngine.h"

const int BW(25), BH(30); // BW琛ㄧず鍦烘櫙鐨勫�藉害, BH琛ㄧず鍦烘櫙鐨勯珮搴�
// PlayerX琛ㄧず鐜╁�剁殑妯�鍧愭爣, EnemyBX銆丒nemyBY鍒嗗埆琛ㄧず鏁屾柟瀛愬脊鐨勬í妯�鍧愭爣銆佺旱鍧愭爣
int PlayerX, EnemyBX, EnemyBY, Score;
// Wall琛ㄧず璇ュ�勬槸鍚︽湁澧�, Bullet琛ㄧず璇ュ�勬槸鍚︽湁瀛愬脊
bool Wall[25 * 30], Bullet[25 * 30];

void Run();
// 鏄剧ず鐜╁��
void ShowPlayer()
{
    FillStr(PlayerX, BH - 2, "鈻�");
    FillStr(PlayerX - 1, BH - 1, "鈻犫枲鈻�");
}
// 鐜╁�舵�讳骸
void GameOver()
{
    // 鍏堝湪涓�闂寸暀涓€鍧楃┖鐧藉尯鍩�, 鐒跺悗灞呬腑鏄剧ずGame Over!
    FillRec(BW - 6 >> 1, (BH >> 1) - 2, 7, 3, "  ");
    FillStr(BW - 5 >> 1, (BH >> 1) - 1, "Game Over!");
    SubmitScore(3, Score);
    // 鏀瑰彉鐜╁�舵樉绀烘牱寮�
    FillStr(PlayerX, BH - 2, "脳");
    FillStr(PlayerX - 1, BH - 1, "脳脳脳");
    // 娓呴櫎瀛愬脊
    FillStr(EnemyBX, EnemyBY, "  ");
    Pause();
    Run();
}
// 鏄剧ず澧欎綋
void ShowWall()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Wall[idx])
            FillStr(idx % BW, idx / BW, "鈻�");
}
// 娣诲姞澧欎綋
void AddWall()
{
    // 鍏堝垽鏂�鐜╁�舵槸鍚︽�讳骸
    int n = BW * (BH - 3) + BW - 1; // 鍊掓暟绗�3琛屾渶鍚庝竴涓�鍧愭爣
    for (int idx = n - BW + 1; idx < n; ++idx)
        if (Wall[idx]) // 濡傛灉鍊掓暟绗�3琛屽瓨鍦ㄥ�欎綋, 鍒欑帺瀹舵�讳骸
            GameOver();
    // 涓嬬Щ鎵€鏈夊�欎綋
    for (int idx = BW * (BH - 1) - 1; idx >= 0; --idx)
        if (Wall[idx])
        {
            Wall[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            Wall[idx + BW] = true;
        }
    // 鎵€鏈夊�欎綋鍚戜笅绉诲姩涓€琛�, 姝ゆ椂鏈€涓婃柟閭ｈ�岀┖鍑烘潵浜�, 涓虹┖鍑烘潵鐨勯偅琛屾坊鍔犲�欎綋
    for (int x = 1; x < BW - 1; ++x)
        if (rand() & 1) // 鏈変竴鍗婄殑姒傜巼鐢熸垚澧欎綋
            Wall[x] = true;
    // 鏄剧ず绉诲姩鍚庣殑澧欎綋
    ShowWall();
}
// 鏄剧ず瀛愬脊
void ShowBullet()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
            FillStr(idx % BW, idx / BW, "鈻�");
}
// 鐜╁�跺瓙寮圭Щ鍔�
void PlayerBulletMove()
{
    for (int idx = 0; idx < BW * BH; ++idx)
        if (Bullet[idx])
        {
            Bullet[idx] = false;
            FillStr(idx % BW, idx / BW, "  ");
            if (idx > BW) // 瀛愬脊鏈�鎶佃揪杈圭晫
            {
                if (Wall[idx - BW]) // 瀛愬脊涓嬩竴浣嶇疆鏄�澧�,瀛愬脊鍜屽�欎竴鍚屾秷闄�
                {
                    Wall[idx - BW] = false;
                    ++Score;
                    FillStr(idx % BW, idx / BW - 1, "  ");
                }
                else
                    Bullet[idx - BW] = true;
            }
        }
    // 鏄剧ず绉诲姩鍚庣殑瀛愬脊
    ShowBullet();
}
// 鏁屼汉瀛愬脊绉诲姩
void EnemyBulletMove()
{
    if (!EnemyBX) // 鏁屼汉瀛愬脊鏄�鍚﹀瓨鍦�
        return;
    if ((EnemyBY == BH - 2 && (EnemyBX == PlayerX - 1 || EnemyBX == PlayerX + 1)) ||
        EnemyBY == BH - 3 && EnemyBX == PlayerX) // 瀛愬脊鍑讳腑鐜╁��
        GameOver();
    if (!EnemyBY)
    {
        int MaxY = 0; // 鏁屾柟瀛愬脊鎵€澶勫垪涓璚all鐨勬渶澶х旱鍧愭爣
        for (int y = BH - 2; y >= 0; --y)
            if (Wall[y * BW + EnemyBX])
            {
                MaxY = y;
                break;
            }
        EnemyBY = MaxY + 1;
        FillStr(EnemyBX, EnemyBY, "鈻�");
    }
    if (EnemyBY < BH - 1) // 瀛愬脊鏈�鎶佃揪杈圭晫
    {
        // 鏁屾柟瀛愬脊鍜岀帺瀹跺瓙寮硅窛绂诲皬浜�2鏍�, 鍒嗗埆绉诲姩1鏍煎悗涓や釜瀛愬脊灏嗕細鍙戠敓纰版挒
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
        FillStr(EnemyBX, EnemyBY, "鈻�");
    }
    else // 瀛愬脊鎶佃揪杈圭晫
    {
        FillStr(EnemyBX, EnemyBY, "  ");
        EnemyBX = 0;
    }
}
// 杩愯�屾父鎴�
void Run()
{
    // 鍒濆�嬪寲娓告垙
    ClearScreen();
    for (bool &w : Wall)
        w = false;
    for (bool &b : Bullet)
        b = false;
    PlayerX = (BW - 1 >> 1);
    EnemyBX = EnemyBY = Score = 0;
    ShowPlayer();
    AddWall();

    int t = 0;
    while (true)
    {
        PumpFrame(); // t鐨勫彇鍊艰寖鍥�0~511, t姣忔�″姞1, 绛変簬511鏃跺€煎彉涓�0
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            if (ch == 224) // 鐜╁�剁Щ鍔�
            {
                // 娓呴櫎瑙掕壊
                FillStr(PlayerX, BH - 2, "  ");
                FillRec(PlayerX - 1, BH - 1, 3, 1, "  ");

                switch (getch())
                {
                case 75: // 宸︾Щ
                    PlayerX -= PlayerX != 1;
                    break;
                case 77: // 鍙崇Щ
                    PlayerX += PlayerX != BW - 2;
                    break;
                }
                ShowPlayer();
            }
            else if (ch == 122) // 鐜╁�跺皠鍑�
            {
                Bullet[BW * (BH - 3) + PlayerX] = true;
                ShowBullet();
            }
            else if (ch == 32) // 鏆傚仠娓告垙
                Pause();
        }
        if (DueLogicTick())
        {
            ++t &= 511;
        if (!(t & 1)) // 瀛愬脊绉诲姩
        {
            PlayerBulletMove();
            EnemyBulletMove();
        }
        if (!(t & 127)) // 娣诲姞鏂扮殑涓€琛�
            AddWall();
        if (!(t & 511)) // 鏁屼汉灏勫嚮
        {
            EnemyBX = rand() % (BW - 3) + 1; // 1鍒�23
            EnemyBY = 0;
        }
        }
    }
}

int main()
{
    SetConsoleFromGameId(3);
    srand((int)time(0));
    Run();
}
