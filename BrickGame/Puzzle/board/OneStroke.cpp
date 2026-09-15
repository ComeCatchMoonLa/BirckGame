#include "../../Engine/BrickEngine.h"
#include <cstring>
#include <string>

const int N = 5, E = 8;
int ex[E] = {0, 0, 1, 1, 2, 2, 3, 3};
int ey[E] = {1, 2, 2, 3, 3, 4, 4, 1};
int used[E], cx, cy, left, moves;
void Run();

int Edge(int a, int b)
{
    for (int i = 0; i < E; ++i)
        if (!used[i] && ((ex[i] == a && ey[i] == b) || (ex[i] == b && ey[i] == a)))
            return i;
    return -1;
}

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("PATH left ") + std::to_string(left));
    for (int i = 0; i < N; ++i)
        FillStr(2 + (i % 3) * 6, 3 + (i / 3) * 4, i == cx ? "[o]" : " o ");
    FillStr(0, 12, "方向沿边走");
}

void Win()
{
    int s = 100000 - moves;
    if (s < 1)
        s = 1;
    SubmitScore(88, s);
    FillStr(0, 13, "Clear");
    Pause();
    Run();
}

void Run()
{
    std::memset(used, 0, sizeof(used));
    cx = 0;
    left = E;
    moves = 0;
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
                Pause();
            else if (ch == 224)
            {
                int k = getch();
                int nx = cx;
                if (k == 77)
                    nx = (cx + 1) % N;
                if (k == 75)
                    nx = (cx + N - 1) % N;
                if (k == 72)
                    nx = (cx + 2) % N;
                if (k == 80)
                    nx = (cx + 3) % N;
                int e = Edge(cx, nx);
                if (e >= 0)
                {
                    used[e] = 1;
                    --left;
                    cx = nx;
                    ++moves;
                    Draw();
                    if (left == 0)
                        Win();
                }
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(88);
    Run();
}
