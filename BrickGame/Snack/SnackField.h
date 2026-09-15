#pragma once
#include "../Engine/BrickEngine.h"
#include "Map.h"

const int MW = 40;
const int MH = 26;
const int MY = 4;

int MapNum = 1;
std::string Field;

bool OnRim(int x, int y)
{
    return x <= 0 || x >= MW - 1 || y <= MY || y >= MY + MH - 1;
}

int MIdx(int x, int y)
{
    return x + (y - MY) * MW;
}

bool Inside(int x, int y)
{
    return x > 0 && x < MW - 1 && y > MY && y < MY + MH - 1;
}

bool MapWall(int x, int y)
{
    if (!Inside(x, y))
        return true;
    const int i = MIdx(x, y);
    if (i < 0 || (size_t)i >= Field.size())
        return true;
    return Field[i] == '0';
}

void EatWallCell(int x, int y)
{
    Field[MIdx(x, y)] = '.';
}

void DrawRim()
{
    FillRec(0, MY, 1, MH, "│");
    FillRec(MW - 1, MY, 1, MH, "┃");
    FillRec(0, MY, MW, 1, "━");
    FillRec(0, MY + MH - 1, MW, 1, "━");
    FillStr(0, MY, "╭");
    FillStr(MW - 1, MY, "╮");
    FillStr(0, MY + MH - 1, "╰");
    FillStr(MW - 1, MY + MH - 1, "╯");
}

void DrawInside(bool invert)
{
    for (int y = MY + 1; y < MY + MH - 1; ++y)
        for (int x = 1; x < MW - 1; ++x)
        {
            const bool wall = Field[MIdx(x, y)] == '0';
            const char* g = "  ";
            if (wall)
                g = invert ? "□" : "■";
            else if (invert)
                g = "·";
            FillStr(x, y, g);
        }
}

void LoadField()
{
    Field = Maps[MapNum - 1];
}

void StartCell(int& sx, int& sy, int& dir)
{
    if (MapNum == 5 || MapNum == 6)
    {
        dir = 0;
        sx = 36;
        sy = 24;
    }
    else
    {
        dir = 1;
        sx = 16;
        sy = 17;
    }
}

void PreviewMap()
{
    LoadField();
    ClearScreen();
    DrawInside(false);
    DrawRim();
    FillRec(14, 13, 12, 8, "  ");
    FillStr(16, 15, " Map: " + std::to_string(MapNum));
    FillStr(16, 17, "Speed: " + std::to_string(GetMachineSpeed()));
    FillStr(16, 19, "Space start");
}

void PickMap()
{
    PreviewMap();
    while (true)
    {
        PumpFrame();
        if (!kbhit())
            continue;
        int ch = getch();
        if (ch == 27)
            QuitToLauncher();
        else if (ch == 32)
            break;
        else if (ch == 224)
        {
            int k = getch();
            if (k == 75)
                MapNum -= MapNum != 1;
            if (k == 77)
                MapNum += MapNum != 6;
            PreviewMap();
        }
    }
}

void SetInvertColor(bool invert)
{
    ApplyConsoleColor(invert ? "08" : "80", CachedTerminal());
}
