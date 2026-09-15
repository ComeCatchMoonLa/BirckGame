#include "../../Engine/BrickEngine.h"
#include <cstring>
#include <string>

int b[8][8], turn, cx, cy;
const int kDx[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int kDy[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
void Run();

int CountFlip(int x, int y, int t, int apply)
{
    if (b[y][x])
        return 0;
    int n = 0;
    for (int d = 0; d < 8; ++d)
    {
        int nx = x + kDx[d], ny = y + kDy[d], c = 0;
        while (nx >= 0 && ny >= 0 && nx < 8 && ny < 8 && b[ny][nx] == 3 - t)
        {
            ++c;
            nx += kDx[d];
            ny += kDy[d];
        }
        if (c && nx >= 0 && ny >= 0 && nx < 8 && ny < 8 && b[ny][nx] == t)
        {
            n += c;
            if (apply)
            {
                nx = x + kDx[d];
                ny = y + kDy[d];
                while (b[ny][nx] == 3 - t)
                {
                    b[ny][nx] = t;
                    nx += kDx[d];
                    ny += kDy[d];
                }
            }
        }
    }
    return n;
}

int Any(int t)
{
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (CountFlip(x, y, t, 0))
                return 1;
    return 0;
}

void Draw()
{
    ClearScreen();
    FillStr(0, 0, turn == 1 ? "黑走" : "白走");
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
        {
            char c = b[y][x] == 1 ? 'X' : (b[y][x] == 2 ? 'O' : '.');
            std::string s(1, c);
            if (x == cx && y == cy)
                s = "[" + s + "]";
            else
                s = " " + s + " ";
            FillStr(x * 3, y + 2, s);
        }
}

void End()
{
    int a = 0, c = 0;
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (b[y][x] == 1)
                ++a;
            else if (b[y][x] == 2)
                ++c;
    SubmitScore(25, a > c ? a : c);
    FillStr(0, 12, std::string("End ") + std::to_string(a) + ":" + std::to_string(c));
    Pause();
    Run();
}

void Run()
{
    std::memset(b, 0, sizeof(b));
    b[3][3] = b[4][4] = 2;
    b[3][4] = b[4][3] = 1;
    turn = 1;
    cx = cy = 3;
    Draw();
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
            {
                if (CountFlip(cx, cy, turn, 0))
                {
                    CountFlip(cx, cy, turn, 1);
                    b[cy][cx] = turn;
                    turn = 3 - turn;
                    if (!Any(turn))
                    {
                        turn = 3 - turn;
                        if (!Any(turn))
                            End();
                    }
                    Draw();
                }
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < 7)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < 7)
                    ++cy;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(25);
    Run();
}
