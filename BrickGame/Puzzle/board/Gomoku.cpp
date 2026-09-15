#include "../../Engine/BrickEngine.h"
#include <cstring>
#include <string>

const int N = 13;
int b[N][N], turn, cx, cy;
void Run();

int WinAt(int x, int y)
{
    const int d[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int t = b[y][x];
    for (int k = 0; k < 4; ++k)
    {
        int c = 1;
        for (int s = -1; s <= 1; s += 2)
        {
            int nx = x + d[k][0] * s, ny = y + d[k][1] * s;
            while (nx >= 0 && ny >= 0 && nx < N && ny < N && b[ny][nx] == t)
            {
                ++c;
                nx += d[k][0] * s;
                ny += d[k][1] * s;
            }
        }
        if (c >= 5)
            return 1;
    }
    return 0;
}

void Draw()
{
    ClearScreen();
    FillStr(0, 0, turn == 1 ? "黑" : "白");
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
        {
            char c = b[y][x] == 1 ? 'X' : (b[y][x] == 2 ? 'O' : '+');
            std::string s(1, c);
            if (x == cx && y == cy)
                s = "[" + s + "]";
            else
                s = " " + s + " ";
            FillStr(x * 2, y + 1, s);
        }
}

void End(int t)
{
    SubmitScore(26, t == 1 ? 2 : 1);
    FillStr(0, N + 2, t == 1 ? "Black win" : "White win");
    Pause();
    Run();
}

void Run()
{
    std::memset(b, 0, sizeof(b));
    turn = 1;
    cx = cy = N / 2;
    Draw();
    while (true)
    {
        PumpFrame();
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32 && !b[cy][cx])
            {
                b[cy][cx] = turn;
                if (WinAt(cx, cy))
                    End(turn);
                turn = 3 - turn;
                Draw();
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cx)
                    --cx;
                if (k == 77 && cx < N - 1)
                    ++cx;
                if (k == 72 && cy)
                    --cy;
                if (k == 80 && cy < N - 1)
                    ++cy;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(26);
    Run();
}
