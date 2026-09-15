#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <string>
#include <vector>

const int N = 4;
std::vector<int> peg[3];
int cur, hold, moves;
void Run();

void Draw()
{
    ClearScreen();
    FillStr(0, 0, std::string("HANOI moves ") + std::to_string(moves) + " 空格拿/放");
    for (int p = 0; p < 3; ++p)
    {
        int x = 4 + p * 10;
        FillRec(x, 2, 1, 8, "|");
        for (size_t i = 0; i < peg[p].size(); ++i)
        {
            int w = peg[p][i];
            FillRec(x - w, 9 - (int)i, w * 2 + 1, 1, "=");
        }
        FillStr(x, 10, p == cur ? "^" : " ");
    }
    if (hold)
        FillStr(0, 12, std::string("HOLD ") + std::to_string(hold));
}

void Win()
{
    int s = 100000 - moves;
    if (s < 1)
        s = 1;
    SubmitScore(87, s);
    FillStr(0, 13, "Clear");
    Pause();
    Run();
}

void Run()
{
    peg[0] = {4, 3, 2, 1};
    peg[1].clear();
    peg[2].clear();
    cur = hold = moves = 0;
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
                if (!hold)
                {
                    if (!peg[cur].empty())
                    {
                        hold = peg[cur].back();
                        peg[cur].pop_back();
                    }
                }
                else
                {
                    if (peg[cur].empty() || peg[cur].back() > hold)
                    {
                        peg[cur].push_back(hold);
                        hold = 0;
                        ++moves;
                        if (peg[2].size() == (size_t)N)
                            Win();
                    }
                }
                Draw();
            }
            else if (ch == 224)
            {
                int k = getch();
                if (k == 75 && cur)
                    --cur;
                if (k == 77 && cur < 2)
                    ++cur;
                Draw();
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(87);
    Run();
}
