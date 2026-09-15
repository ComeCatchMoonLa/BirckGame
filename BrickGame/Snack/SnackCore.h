#pragma once
#include "SnackField.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#ifndef GAME_ID
#error GAME_ID required
#endif

const int kDx[4] = {0, 1, 0, -1};
const int kDy[4] = {-1, 0, 1, 0};

std::vector<Vector2> p1;
#ifdef MODE_DUAL
std::vector<Vector2> p2;
int d2;
bool dead2;
#endif
int d1, fx, fy, score, foods;
bool dead1;
int bx = -1, by = -1;
int gx = -1, gy = -1, hx = -1, hy = -1;
int skipPortal;
bool inner;
DWORD timeEnd, bonusBorn, bonusGap, innerUntil, slowUntil;

void Run();

bool OnBody(const std::vector<Vector2>& b, int x, int y)
{
    for (const Vector2& s : b)
        if (s.x == x && s.y == y)
            return true;
    return false;
}

bool Busy(int x, int y)
{
    if (!Inside(x, y) || MapWall(x, y))
        return true;
    if (OnBody(p1, x, y))
        return true;
#ifdef MODE_DUAL
    if (OnBody(p2, x, y))
        return true;
#endif
    if (fx >= 0 && x == fx && y == fy)
        return true;
    if (bx >= 0 && x == bx && y == by)
        return true;
    if (gx >= 0 && x == gx && y == gy)
        return true;
    if (hx >= 0 && x == hx && y == hy)
        return true;
    return false;
}

void RandEmpty(int& x, int& y)
{
    int nx = 0;
    int ny = 0;
    do
    {
        nx = 1 + rand() % (MW - 2);
        ny = MY + 1 + rand() % (MH - 2);
    } while (Busy(nx, ny));
    x = nx;
    y = ny;
}

void PlaceFood()
{
    RandEmpty(fx, fy);
    FillStr(fx, fy, "●");
}

void PlaceBonus()
{
#if defined(MODE_TIMED) || defined(MODE_SPEED)
    RandEmpty(bx, by);
    bonusBorn = GetTickCount();
    FillStr(bx, by, "⚪");
#endif
}

bool BonusLive()
{
    return bx >= 0 && GetTickCount() - bonusBorn < 5000;
}

bool DrawBonusNow()
{
    if (!BonusLive())
        return false;
    const DWORD left = 5000 - (GetTickCount() - bonusBorn);
    if (left > 2000)
        return true;
    return (GetTickCount() / 200) % 2 == 0;
}

void Hud()
{
    std::string extra = std::string("SCORE ") + std::to_string(score);
#ifdef MODE_TIMED
    int sec = 0;
    if (timeEnd > GetTickCount())
        sec = (int)((timeEnd - GetTickCount()) / 1000);
    extra += "  TIME " + std::to_string(sec) + "s";
#endif
#ifdef MODE_SPEED
    int step = 1 + foods;
    if (GetTickCount() < slowUntil)
        step = step > 5 ? step - 5 : 1;
    extra += "  STEP " + std::to_string(step);
#endif
#ifdef MODE_WARP
    extra += "  wrap";
#endif
#ifdef MODE_PORTAL
    extra += "  portal";
#endif
#ifdef MODE_MIRROR
    extra += inner ? "  INNER" : "  outer";
    if (inner)
    {
        int sec = 0;
        if (innerUntil > GetTickCount())
            sec = (int)((innerUntil - GetTickCount()) / 1000);
        extra += " " + std::to_string(sec) + "s";
    }
#endif
#ifdef MODE_DUAL
    extra += "  P1 arrows P2 WASD";
#endif
    FillStr(2, 1, extra + "  SPEED " + std::to_string(GetMachineSpeed()) + "     ");
#ifdef MODE_PICK_MAP
    FillStr(2, 2, std::string("MAP ") + std::to_string(MapNum) + "     ");
#endif
}

void DrawBg(int x, int y)
{
    if (!inner && fx >= 0 && x == fx && y == fy)
    {
        FillStr(x, y, "●");
        return;
    }
#ifdef MODE_PORTAL
    if ((x == gx && y == gy) || (x == hx && y == hy))
    {
        FillStr(x, y, "⚪");
        return;
    }
#endif
#ifdef MODE_MIRROR
    if (!inner && gx >= 0 && x == gx && y == gy)
    {
        FillStr(x, y, "⚪");
        return;
    }
#endif
    if (MapWall(x, y))
        FillStr(x, y, inner ? "□" : "■");
    else if (inner)
        FillStr(x, y, "·");
    else
        FillStr(x, y, "  ");
}

void PaintHead(const std::vector<Vector2>& body)
{
    FillStr(body[0].x, body[0].y, inner ? "■" : "□");
    if (body.size() > 1)
        FillStr(body[1].x, body[1].y, inner ? "□" : "■");
}

void PaintAll()
{
#ifdef MODE_MIRROR
    DrawInside(inner);
#else
    DrawInside(false);
#endif
    DrawRim();
    for (size_t i = 0; i < p1.size(); ++i)
        FillStr(p1[i].x, p1[i].y, i == 0 ? (inner ? "■" : "□") : (inner ? "□" : "■"));
#ifdef MODE_DUAL
    for (size_t i = 0; i < p2.size(); ++i)
        FillStr(p2[i].x, p2[i].y, i == 0 ? "○" : "■");
#endif
    if (!inner)
        FillStr(fx, fy, "●");
#ifdef MODE_PORTAL
    FillStr(gx, gy, "⚪");
    FillStr(hx, hy, "⚪");
#endif
#ifdef MODE_MIRROR
    if (!inner && gx >= 0)
        FillStr(gx, gy, "⚪");
#endif
    if (DrawBonusNow())
        FillStr(bx, by, "⚪");
    Hud();
}

void Die()
{
    SetInvertColor(false);
    SubmitScore(GAME_ID, score);
    FillStr(14, 16, "Game Over");
    Pause();
    Run();
}

void Turn(int& dir, int nd)
{
    if (((dir + 2) & 3) == nd)
        return;
    dir = nd;
}

void StepOne(std::vector<Vector2>& body, int& dir, bool& dead)
{
    if (dead)
        return;
    bool flipped = false;
    int nx = body[0].x + kDx[dir];
    int ny = body[0].y + kDy[dir];
#ifdef MODE_WARP
    if (nx < 1)
        nx = MW - 2;
    if (nx > MW - 2)
        nx = 1;
    if (ny < MY + 1)
        ny = MY + MH - 2;
    if (ny > MY + MH - 2)
        ny = MY + 1;
#else
    if (OnRim(nx, ny))
        dead = true;
#endif
#ifdef MODE_PORTAL
    if (!dead && skipPortal <= 0)
    {
        if (nx == gx && ny == gy)
        {
            nx = hx;
            ny = hy;
            skipPortal = 2;
        }
        else if (nx == hx && ny == hy)
        {
            nx = gx;
            ny = gy;
            skipPortal = 2;
        }
    }
    else if (skipPortal > 0)
        --skipPortal;
#endif
#ifdef MODE_MIRROR
    if (!dead && !inner && gx >= 0 && nx == gx && ny == gy)
    {
        inner = true;
        innerUntil = GetTickCount() + 5000;
        gx = gy = -1;
        SetInvertColor(true);
        flipped = true;
    }
#endif
#ifndef MODE_WARP
    if (!dead && inner)
    {
        if (nx == fx && ny == fy)
            dead = true;
        else if (Inside(nx, ny) && MapWall(nx, ny))
        {
            EatWallCell(nx, ny);
            score += 10;
            body.insert(body.begin(), {nx, ny});
            PaintHead(body);
            Hud();
            return;
        }
        else if (MapWall(nx, ny))
            dead = true;
    }
    else if (!dead && MapWall(nx, ny))
        dead = true;
#endif
    if (!dead && OnBody(body, nx, ny))
        dead = true;
#ifdef MODE_DUAL
    if (!dead)
    {
        if (&body == &p1 && OnBody(p2, nx, ny))
            dead = true;
        if (&body == &p2 && OnBody(p1, nx, ny))
            dead = true;
    }
#endif
    if (dead)
    {
        FillStr(body[0].x, body[0].y, "x");
        Die();
        return;
    }
    body.insert(body.begin(), {nx, ny});
    bool grow = false;
    if (!inner && nx == fx && ny == fy)
    {
        score += 10;
        ++foods;
#ifdef MODE_TIMED
        timeEnd += 5000;
#endif
        PlaceFood();
        grow = true;
    }
    if (!inner && BonusLive() && nx == bx && ny == by)
    {
#ifdef MODE_TIMED
        timeEnd += 10000;
#endif
#ifdef MODE_SPEED
        slowUntil = GetTickCount() + 5000;
#endif
        bx = by = -1;
        bonusGap = GetTickCount() + 2000;
    }
    if (!grow)
    {
        Vector2 t = body.back();
        body.pop_back();
        if (!flipped)
            DrawBg(t.x, t.y);
    }
    if (flipped)
        PaintAll();
    else
        PaintHead(body);
}

void TickBonus()
{
#if defined(MODE_TIMED) || defined(MODE_SPEED)
    if (bx >= 0 && !BonusLive())
    {
        if (!OnBody(p1, bx, by))
            DrawBg(bx, by);
        bx = by = -1;
        bonusGap = GetTickCount() + 2000;
    }
    if (bx < 0 && GetTickCount() >= bonusGap)
        PlaceBonus();
#endif
#ifdef MODE_MIRROR
    if (inner && GetTickCount() >= innerUntil)
    {
        inner = false;
        SetInvertColor(false);
        RandEmpty(gx, gy);
        PaintAll();
        if (MapWall(p1[0].x, p1[0].y))
        {
            Die();
            return;
        }
    }
#endif
#ifdef MODE_TIMED
    if (GetTickCount() >= timeEnd)
    {
        Die();
        return;
    }
#endif
}

void LogicStep()
{
    TickBonus();
    StepOne(p1, d1, dead1);
#ifdef MODE_DUAL
    StepOne(p2, d2, dead2);
#endif
}

void InitGame()
{
    SetInvertColor(false);
    dead1 = false;
    score = foods = 0;
    fx = fy = -1;
    bx = by = gx = gy = hx = hy = -1;
    skipPortal = 0;
    inner = false;
    slowUntil = 0;
    bonusGap = GetTickCount();
#ifdef MODE_PICK_MAP
    PickMap();
#else
    MapNum = 1;
    LoadField();
#endif
    int sx, sy;
    StartCell(sx, sy, d1);
    p1 = {{sx, sy}, {sx - kDx[d1], sy - kDy[d1]}, {sx - 2 * kDx[d1], sy - 2 * kDy[d1]}};
#ifdef MODE_DUAL
    d2 = 3;
    p2 = {{28, 17}, {29, 17}, {30, 17}};
    dead2 = false;
#endif
    ClearScreen();
    PlaceFood();
#ifdef MODE_PORTAL
    RandEmpty(gx, gy);
    RandEmpty(hx, hy);
#endif
#ifdef MODE_MIRROR
    RandEmpty(gx, gy);
#endif
#ifdef MODE_TIMED
    timeEnd = GetTickCount() + 30000;
    PlaceBonus();
#endif
#ifdef MODE_SPEED
    PlaceBonus();
#endif
    PaintAll();
}

int StepRate()
{
#ifdef MODE_SPEED
    int step = 1 + foods;
    if (GetTickCount() < slowUntil)
        step = step > 5 ? step - 5 : 1;
    return step;
#else
    return 1;
#endif
}

void Run()
{
    InitGame();
    bool moved = true;
    while (true)
    {
        PumpFrame();
        TickBonus();
        Hud();
#if defined(MODE_TIMED) || defined(MODE_SPEED)
        if (bx >= 0 && BonusLive() && GetTickCount() - bonusBorn >= 3000)
        {
            if (DrawBonusNow())
                FillStr(bx, by, "⚪");
            else if (!OnBody(p1, bx, by))
                DrawBg(bx, by);
        }
#endif
#ifdef MODE_SPEED
        TakeLogicSteps(StepRate(), [&] {
            LogicStep();
            moved = true;
        });
#else
        if (DueLogicTick())
        {
            LogicStep();
            moved = true;
        }
#endif
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
            {
                SetInvertColor(false);
                QuitToLauncher();
            }
            else if (ch == 32)
                Pause();
            else if (ch == 122)
            {
                StepOne(p1, d1, dead1);
                moved = true;
            }
            else if (ch == 224 && moved)
            {
                int k = getch();
                if (k == 72)
                    Turn(d1, 0);
                else if (k == 77)
                    Turn(d1, 1);
                else if (k == 80)
                    Turn(d1, 2);
                else if (k == 75)
                    Turn(d1, 3);
                moved = false;
            }
#ifdef MODE_DUAL
            else if (ch == 'w' || ch == 'W')
                Turn(d2, 0);
            else if (ch == 'd' || ch == 'D')
                Turn(d2, 1);
            else if (ch == 's' || ch == 'S')
                Turn(d2, 2);
            else if (ch == 'a' || ch == 'A')
                Turn(d2, 3);
#endif
        }
    }
}

int main()
{
    SetConsoleFromGameId(GAME_ID);
    srand((unsigned)time(0));
    Run();
}
