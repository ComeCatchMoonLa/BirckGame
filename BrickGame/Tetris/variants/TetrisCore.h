#pragma once
#include "../../Engine/BrickEngine.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#ifndef GAME_ID
#error GAME_ID required
#endif
#ifndef GOAL_SCORE
#define GOAL_SCORE 30
#endif
#ifndef BOARD_W
#define BOARD_W 14
#endif
#ifndef BOARD_H
#define BOARD_H 22
#endif

const int BX = 1;
const int BY = 0;

int board[BOARD_H][BOARD_W];
int px, py, rot, kind, curColor;
int score, ticks, linesCleared, garbageClock;
int rotLeft, invBlink;
bool bombPiece;
int speedShown;

#ifdef RULE_PENTO
const int kKinds = 7;
const int kCells = 5;
const int kShape[7][4][5][2] = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}},
     {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}},
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}},
     {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}}},
    {{{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}}, {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {0, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}, {{2, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}}},
    {{{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}}, {{2, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 2}},
     {{0, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 2}}, {{2, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 2}}},
    {{{0, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {1, 0}, {1, 1}, {0, 2}, {1, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {2, 1}}, {{0, 0}, {1, 0}, {0, 1}, {0, 2}, {1, 2}}},
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}}, {{2, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}},
     {{1, 0}, {1, 1}, {0, 2}, {1, 2}, {2, 2}}, {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {0, 2}}},
    {{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}}, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}}, {{3, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}},
};
#else
const int kKinds = 7;
const int kCells = 4;
const int kShape[7][4][4][2] = {
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}, {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 1}}, {{2, 0}, {2, 1}, {2, 2}, {2, 3}}},
    {{{1, 0}, {2, 0}, {1, 1}, {2, 1}}, {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {2, 0}, {1, 1}, {2, 1}}, {{1, 0}, {2, 0}, {1, 1}, {2, 1}}},
    {{{0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
     {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
     {{1, 1}, {2, 1}, {0, 2}, {1, 2}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{2, 0}, {1, 1}, {2, 1}, {1, 2}},
     {{0, 1}, {1, 1}, {1, 2}, {2, 2}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}}},
    {{{0, 0}, {0, 1}, {0, 2}, {1, 2}}, {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {0, 1}, {1, 1}, {2, 1}}},
    {{{1, 0}, {1, 1}, {1, 2}, {0, 2}}, {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {0, 2}}, {{0, 0}, {1, 0}, {2, 0}, {2, 1}}},
};
#endif

const char* ColorGlyph(int c)
{
#ifdef RULE_COLOR
    if (c == 2)
        return "□";
    if (c == 3)
        return "◆";
#endif
    (void)c;
    return "■";
}

void PaintCell(int x, int y, const char* g)
{
    if (y < 0)
        return;
    FillStr(x + BX, y + BY, g);
}

void DrawLocked(int x, int y, int v)
{
    if (v == 0)
        PaintCell(x, y, "  ");
    else
        PaintCell(x, y, ColorGlyph(v));
}

bool Hits(int nx, int ny, int nrot)
{
    for (int i = 0; i < kCells; ++i)
    {
        int x = nx + kShape[kind][nrot][i][0];
        int y = ny + kShape[kind][nrot][i][1];
        if (x < 0 || x >= BOARD_W || y >= BOARD_H)
            return true;
        if (y >= 0 && board[y][x])
            return true;
    }
    return false;
}

bool PieceShown()
{
#ifdef RULE_INVISIBLE
    int land = py;
    while (!Hits(px, land + 1, rot))
        ++land;
    if (py * 2 < land)
        return true;
    return (GetTickCount() / 500) % 2 == 0;
#else
    return true;
#endif
}

void DrawPiece(bool show)
{
#ifdef RULE_INVISIBLE
    if (show && !PieceShown())
        show = false;
#endif
    for (int i = 0; i < kCells; ++i)
    {
        int x = px + kShape[kind][rot][i][0];
        int y = py + kShape[kind][rot][i][1];
        if (show)
            PaintCell(x, y, bombPiece ? "※" : ColorGlyph(curColor));
        else if (y >= 0 && !board[y][x])
            PaintCell(x, y, "  ");
    }
}

void DrawHud()
{
    FillStr(BOARD_W + 3, 2, "SCORE");
#ifdef RULE_FORTY
    FillStr(BOARD_W + 3, 3, std::to_string(score) + "/" + std::to_string(GOAL_SCORE) + " ");
#else
    FillStr(BOARD_W + 3, 3, std::to_string(score) + "   ");
#endif
    FillStr(BOARD_W + 3, 5, "SPEED");
    FillStr(BOARD_W + 3, 6, std::to_string(speedShown) + " ");
#ifdef RULE_MIRROR
    FillStr(BOARD_W + 3, 8, "左右对调");
#endif
#ifdef RULE_NO_ROTATE
    FillStr(BOARD_W + 3, 8, "ROT");
    FillStr(BOARD_W + 3, 9, std::to_string(rotLeft) + "/3 ");
#endif
}

void RedrawBoard()
{
    for (int y = 0; y < BOARD_H; ++y)
        for (int x = 0; x < BOARD_W; ++x)
            DrawLocked(x, y, board[y][x]);
    DrawPiece(true);
    DrawHud();
}

void Run();

void Finish(bool won)
{
    DrawPiece(true);
    SubmitScore(GAME_ID, score);
    const char* msg = won ? "Win" : "Game Over";
    const int field = 9;
    std::string s((field - (int)strlen(msg)) / 2, ' ');
    s += msg;
    s.append(field - (int)s.size(), ' ');
    FillStr(BX + (BOARD_W - field) / 2, BOARD_H / 2, s);
    Pause();
    Run();
}

int LineFull(int y)
{
    int first = 0;
    for (int x = 0; x < BOARD_W; ++x)
    {
        if (!board[y][x])
            return 0;
        if (x == 0)
            first = board[y][x];
#ifdef RULE_COLOR
        if (board[y][x] != first)
            return 0;
#else
        (void)first;
#endif
    }
    return 1;
}

void CompactColumns()
{
    for (int x = 0; x < BOARD_W; ++x)
    {
        int w = BOARD_H - 1;
        for (int y = BOARD_H - 1; y >= 0; --y)
        {
            if (board[y][x])
            {
                int v = board[y][x];
                board[y][x] = 0;
                board[w][x] = v;
                --w;
            }
        }
    }
}

void ShiftDown(int clearedY)
{
    for (int y = clearedY; y > 0; --y)
        for (int x = 0; x < BOARD_W; ++x)
            board[y][x] = board[y - 1][x];
    for (int x = 0; x < BOARD_W; ++x)
        board[0][x] = 0;
}

int ClearLines()
{
    int n = 0;
#ifdef RULE_GRAVITY
    bool again = true;
    while (again)
    {
        again = false;
        for (int y = BOARD_H - 1; y >= 0; --y)
        {
            if (LineFull(y))
            {
                for (int x = 0; x < BOARD_W; ++x)
                    board[y][x] = 0;
                ++n;
                ++score;
                again = true;
            }
        }
        if (again)
            CompactColumns();
    }
#else
    for (int y = BOARD_H - 1; y >= 0; --y)
    {
        if (LineFull(y))
        {
            ShiftDown(y);
            ++n;
#ifndef RULE_FORTY
            ++score;
#endif
            ++y;
        }
    }
#endif
    linesCleared += n;
#ifdef RULE_FORTY
    if (n > 0)
        score += n * 2 - 1;
#endif
    return n;
}

void ExplodeBomb()
{
    for (int i = 0; i < kCells; ++i)
    {
        int cx = px + kShape[kind][rot][i][0];
        int cy = py + kShape[kind][rot][i][1];
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
            {
                int x = cx + dx, y = cy + dy;
                if (x >= 0 && x < BOARD_W && y >= 0 && y < BOARD_H)
                    board[y][x] = 0;
            }
    }
}

void AddGarbage()
{
    for (int x = 0; x < BOARD_W; ++x)
        if (board[0][x])
        {
            Finish(false);
            return;
        }
    for (int y = 0; y < BOARD_H - 1; ++y)
        for (int x = 0; x < BOARD_W; ++x)
            board[y][x] = board[y + 1][x];
    int hole = rand() % BOARD_W;
    for (int x = 0; x < BOARD_W; ++x)
        board[BOARD_H - 1][x] = (x == hole) ? 0 : 1;
}

void Spawn()
{
    kind = rand() % kKinds;
    rot = 0;
    rotLeft = 3;
    px = BOARD_W / 2 - 1;
    py = 0;
    curColor = 1;
#ifdef RULE_COLOR
    curColor = 1 + rand() % 3;
#endif
    bombPiece = false;
#ifdef RULE_BOMB
    bombPiece = (rand() % 6) == 0;
#endif
    if (Hits(px, py, rot))
        Finish(false);
}

void Lock()
{
    DrawPiece(false);
    if (bombPiece)
        ExplodeBomb();
    else
    {
        for (int i = 0; i < kCells; ++i)
        {
            int x = px + kShape[kind][rot][i][0];
            int y = py + kShape[kind][rot][i][1];
            if (y >= 0)
                board[y][x] = curColor;
        }
    }
    ClearLines();
#ifdef RULE_FORTY
    if (score >= GOAL_SCORE)
    {
        RedrawBoard();
        Finish(true);
        return;
    }
#endif
#ifdef RULE_GARBAGE
    ++garbageClock;
    if (garbageClock >= 8)
    {
        garbageClock = 0;
        AddGarbage();
    }
#endif
    RedrawBoard();
    Spawn();
    DrawPiece(true);
    DrawHud();
}

void TryMove(int dx, int dy, int drot)
{
    int nr = (rot + drot) & 3;
    if (!Hits(px + dx, py + dy, nr))
    {
        DrawPiece(false);
        px += dx;
        py += dy;
        rot = nr;
        DrawPiece(true);
    }
    else if (dy == 1 && dx == 0 && drot == 0)
        Lock();
}

void Drop() { TryMove(0, 1, 0); }
void Left() { TryMove(-1, 0, 0); }
void Right() { TryMove(1, 0, 0); }
void RandomShape()
{
    int oldK = kind;
    int oldR = rot;
    DrawPiece(false);
    for (int attempt = 0; attempt < 16; ++attempt)
    {
        kind = rand() % kKinds;
        rot = rand() % 4;
        if (kind == oldK)
            continue;
        if (!Hits(px, py, rot))
        {
            DrawPiece(true);
            return;
        }
    }
    kind = oldK;
    rot = oldR;
    DrawPiece(true);
}

void Rotate()
{
#ifdef RULE_NO_ROTATE
    if (rotLeft <= 0)
        return;
    int old = rot;
    TryMove(0, 0, 1);
    if (rot != old)
        --rotLeft;
    DrawHud();
#elif defined(RULE_SHAPE_SHIFT)
    RandomShape();
#else
    TryMove(0, 0, 1);
#endif
}

void InitBoard()
{
    ClearScreen();
    std::memset(board, 0, sizeof(board));
    score = ticks = linesCleared = garbageClock = 0;
    invBlink = 1;
    speedShown = GetMachineSpeed();
    FillRec(BX - 1, BY, 1, BOARD_H, "│");
    FillRec(BX + BOARD_W, BY, 1, BOARD_H, "│");
    FillRec(BX, BY + BOARD_H, BOARD_W, 1, "━");
    Spawn();
    RedrawBoard();
}

void Run()
{
    InitBoard();
    while (true)
    {
        PumpFrame();
#ifdef RULE_INVISIBLE
        {
            int phase = PieceShown() ? 1 : 0;
            if (phase != invBlink)
            {
                invBlink = phase;
                DrawPiece(true);
            }
        }
#endif
        if (DueLogicTick())
        {
            ++ticks;
            Drop();
            DrawHud();
        }
        if (kbhit())
        {
            int ch = getch();
            if (ch == 27)
                QuitToLauncher();
            else if (ch == 32)
                Pause();
#ifdef RULE_SHAPE_SHIFT
            else if (ch == '8')
                Rotate();
#endif
            else if (ch == 224)
            {
                int k = getch();
                if (k == 72)
                    Rotate();
                else if (k == 80)
                    Drop();
                else if (k == 75)
#ifdef RULE_MIRROR
                    Right();
#else
                    Left();
#endif
                else if (k == 77)
#ifdef RULE_MIRROR
                    Left();
#else
                    Right();
#endif
            }
        }
    }
}

int main()
{
    SetConsoleFromGameId(GAME_ID);
    srand((unsigned)time(0));
    Run();
}
