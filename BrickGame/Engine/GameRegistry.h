#pragma once

enum class GameStatus
{
    Implemented,
    Unimplemented
};

struct GameEntry
{
    int id;
    const char* name;
    const char* exeFileName;
    GameStatus status;
    int windowColumns;
    int windowRows;
};

int GetGameCount();
const GameEntry& GetGameAt(int index);
const GameEntry* FindGameById(int id);
bool IsGameImplemented(int id);
