#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <ctime>
#include <string>

const int N = 6;
int g[N][N], drops, cx, cy, score;
void Run();

void Draw()
{
    FillStr(0, 0, std::string("DROPS ") + std::to_string(drops));
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
        {
            std::string s = std::to_string(g[y][x]);
            if (x == cx && y == cy)
                s = "[" + s + "]";
            else
                s = " " + s + " ";
            FillStr(x * 4, y * 2 + 2, s);
        }
}

void Burst(int x, int y);

void Splash(int x, int y, int dx, int dy)
{
    int nx = x + dx, ny = y + dy;
    while (nx >= 0 && ny >= 0 && nx < N && ny < N)
    {
        if (g[ny][nx] == 0)
        {
            nx += dx;
            ny += dy;
            continue;
        }
        ++g[ny][nx];
        if (g[ny][nx] >= 5)
            Burst(nx, ny);
        break;
    }
}

void Burst(int x, int y)
{
    g[y][x] = 0;
    ++score;
    Splash(x, y, 1, 0);
    Splash(x, y, -1, 0);
    Splash(x, y, 0, 1);
    Splash(x, y, 0, -1);
}

int Empty()
{
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
            if (g[y][x])
                return 0;
    return 1;
}

void Win()
{
    SubmitScore(12, 100 + drops);
    FillStr(0, 16, "Clear");
    Pause();
    Run();
}

void Fail()
{
    SubmitScore(12, score);
    FillStr(0, 16, "No drops");
    Pause();
    Run();
}

void Run()
{
    ClearScreen();
    drops = 10;
    score = cx = cy = 0;
    for (int y = 0; y < N; ++y)
        for (int x = 0; x < N; ++x)
            g[y][x] = rand() % 4;
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
                if (drops <= 0)
                    Fail();
                --drops;
                ++g[cy][cx];
                if (g[cy][cx] >= 5)
                    Burst(cx, cy);
                Draw();
                if (Empty())
                    Win();
                else if (drops <= 0)
                    Fail();
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
    SetConsoleFromGameId(12);
    srand((unsigned)time(0));
    Run();
}
