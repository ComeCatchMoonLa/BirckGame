#pragma once
#include "Terminal.h"
#include "GameRegistry.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <queue>

struct Vector2
{
    int x, y;
};

typedef std::vector<Vector2> Sites;

void SetPos(int i, short j);
void SetConsole(std::string name, int width, int height, std::string color);
void Pause();
void FillStr(int x, int y, const std::string& fillstr);
void FillRec(int x, int y, int width, int height, const std::string& fillstr);
void FillArea(int x, int y, const Sites& sites, const std::string& fillstr);
