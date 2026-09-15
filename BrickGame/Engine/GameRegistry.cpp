#include "GameRegistry.h"

namespace
{
const GameEntry kGames[] = {
    {1, "俄罗斯方块", "TetrisPro", GameStatus::Implemented, 40, 27},
    {2, "贪吃蛇", "SnackPro", GameStatus::Implemented, 80, 30},
    {3, "消除射击", "Eliminate", GameStatus::Implemented, 50, 30},
    {4, "填补射击", "FullUp", GameStatus::Implemented, 30, 20},
    {5, "淘气方块", "NaughtyBrick", GameStatus::Implemented, 80, 30},
    {6, "寸步难行", "CantMove", GameStatus::Implemented, 320, 120},
    {7, "方块赛车", "Racing", GameStatus::Implemented, 30, 30},
    {8, "青蛙过河", nullptr, GameStatus::Unimplemented, 0, 0},
    {9, "碰撞弹球", nullptr, GameStatus::Unimplemented, 0, 0},
    {10, "桌上足球", nullptr, GameStatus::Unimplemented, 0, 0},
    {11, "接水果", nullptr, GameStatus::Unimplemented, 0, 0},
    {12, "十滴水", nullptr, GameStatus::Unimplemented, 0, 0},
    {13, "切图片", nullptr, GameStatus::Unimplemented, 0, 0},
    {14, "打蜜蜂", nullptr, GameStatus::Unimplemented, 0, 0},
    {15, "拼图", nullptr, GameStatus::Unimplemented, 0, 0},
    {16, "数字魔方", nullptr, GameStatus::Unimplemented, 0, 0},
    {17, "迷宫", "MazeOne", GameStatus::Implemented, 102, 31},
    {18, "数独", nullptr, GameStatus::Unimplemented, 0, 0},
    {19, "2048", nullptr, GameStatus::Unimplemented, 0, 0},
    {20, "坦克大战", nullptr, GameStatus::Unimplemented, 0, 0},
    {21, "吃豆人", nullptr, GameStatus::Unimplemented, 0, 0},
    {22, "flappy bird", nullptr, GameStatus::Unimplemented, 0, 0},
    {23, "推箱子", nullptr, GameStatus::Unimplemented, 0, 0},
    {24, "井字棋", nullptr, GameStatus::Unimplemented, 0, 0},
    {25, "黑白棋", nullptr, GameStatus::Unimplemented, 0, 0},
    {26, "五子棋", nullptr, GameStatus::Unimplemented, 0, 0},
    {27, "围棋", nullptr, GameStatus::Unimplemented, 0, 0},
    {28, "打砖块", nullptr, GameStatus::Unimplemented, 0, 0},
    {29, "乒乓球", nullptr, GameStatus::Unimplemented, 0, 0},
    {30, "太空侵略者", nullptr, GameStatus::Unimplemented, 0, 0},
    {31, "打地鼠", nullptr, GameStatus::Unimplemented, 0, 0},
    {32, "连连看", nullptr, GameStatus::Unimplemented, 0, 0},
    {33, "扫雷", nullptr, GameStatus::Unimplemented, 0, 0},
    {34, "记忆翻牌", nullptr, GameStatus::Unimplemented, 0, 0},
    {35, "华容道", nullptr, GameStatus::Unimplemented, 0, 0},
    {36, "镜像方块", "MirrorTetris", GameStatus::Implemented, 40, 27},
    {37, "五格方块", "PentoTetris", GameStatus::Implemented, 50, 27},
    {38, "隐形方块", "InvisibleTetris", GameStatus::Implemented, 40, 27},
    {39, "只出方形", "OOnlyTetris", GameStatus::Implemented, 40, 27},
    {40, "炸弹方块", "BombTetris", GameStatus::Implemented, 40, 27},
    {41, "双人方块", nullptr, GameStatus::Unimplemented, 0, 0},
    {42, "四十行挑战", "FortyLineTetris", GameStatus::Implemented, 40, 27},
    {43, "垃圾行方块", "GarbageTetris", GameStatus::Implemented, 40, 27},
    {44, "无旋转方块", "NoRotateTetris", GameStatus::Implemented, 40, 27},
    {45, "同色消行", "ColorTetris", GameStatus::Implemented, 40, 27},
    {46, "重力方块", "GravityTetris", GameStatus::Implemented, 40, 27},
    {47, "宽场方块", "WideTetris", GameStatus::Implemented, 50, 27},
    {48, "窄场方块", "NarrowTetris", GameStatus::Implemented, 32, 27},
    {49, "穿墙蛇", nullptr, GameStatus::Unimplemented, 0, 0},
    {50, "双人蛇", nullptr, GameStatus::Unimplemented, 0, 0},
    {51, "障碍蛇", nullptr, GameStatus::Unimplemented, 0, 0},
    {52, "限时蛇", nullptr, GameStatus::Unimplemented, 0, 0},
    {53, "加速蛇", nullptr, GameStatus::Unimplemented, 0, 0},
    {54, "围墙蛇", nullptr, GameStatus::Unimplemented, 0, 0},
    {55, "对向赛车", nullptr, GameStatus::Unimplemented, 0, 0},
    {56, "加油赛车", nullptr, GameStatus::Unimplemented, 0, 0},
    {57, "双向过河", nullptr, GameStatus::Unimplemented, 0, 0},
    {58, "炸弹水果", nullptr, GameStatus::Unimplemented, 0, 0},
    {59, "双人接水果", nullptr, GameStatus::Unimplemented, 0, 0},
    {60, "落石躲避", nullptr, GameStatus::Unimplemented, 0, 0},
    {61, "过马路", nullptr, GameStatus::Unimplemented, 0, 0},
    {62, "接降落伞", nullptr, GameStatus::Unimplemented, 0, 0},
    {63, "双炮射击", nullptr, GameStatus::Unimplemented, 0, 0},
    {64, "限弹填补", nullptr, GameStatus::Unimplemented, 0, 0},
    {65, "护盾打蜂", nullptr, GameStatus::Unimplemented, 0, 0},
    {66, "迷宫坦克", nullptr, GameStatus::Unimplemented, 0, 0},
    {67, "UFO侵略", nullptr, GameStatus::Unimplemented, 0, 0},
    {68, "飞机大战", nullptr, GameStatus::Unimplemented, 0, 0},
    {69, "多球打砖", nullptr, GameStatus::Unimplemented, 0, 0},
    {70, "激光打砖", nullptr, GameStatus::Unimplemented, 0, 0},
    {71, "对打乒乓", nullptr, GameStatus::Unimplemented, 0, 0},
    {72, "多球弹球", nullptr, GameStatus::Unimplemented, 0, 0},
    {73, "迷雾迷宫", nullptr, GameStatus::Unimplemented, 0, 0},
    {74, "限时迷宫", nullptr, GameStatus::Unimplemented, 0, 0},
    {75, "追逐吃豆", nullptr, GameStatus::Unimplemented, 0, 0},
    {76, "冰面推箱", nullptr, GameStatus::Unimplemented, 0, 0},
    {77, "六宫数独", nullptr, GameStatus::Unimplemented, 0, 0},
    {78, "三格2048", nullptr, GameStatus::Unimplemented, 0, 0},
    {79, "五格2048", nullptr, GameStatus::Unimplemented, 0, 0},
    {80, "中级扫雷", nullptr, GameStatus::Unimplemented, 0, 0},
    {81, "高级扫雷", nullptr, GameStatus::Unimplemented, 0, 0},
    {82, "计时连连看", nullptr, GameStatus::Unimplemented, 0, 0},
    {83, "横刀立马", nullptr, GameStatus::Unimplemented, 0, 0},
    {84, "旋转拼图", nullptr, GameStatus::Unimplemented, 0, 0},
    {85, "计时翻牌", nullptr, GameStatus::Unimplemented, 0, 0},
    {86, "数字华容道", nullptr, GameStatus::Unimplemented, 0, 0},
    {87, "汉诺塔", nullptr, GameStatus::Unimplemented, 0, 0},
    {88, "一笔画", nullptr, GameStatus::Unimplemented, 0, 0},
};

const int kGameCount = static_cast<int>(sizeof(kGames) / sizeof(kGames[0]));
} // namespace

int GetGameCount()
{
    return kGameCount;
}

const GameEntry& GetGameAt(int index)
{
    if (index < 0 || index >= kGameCount)
    {
        static const GameEntry kInvalid{0, "", nullptr, GameStatus::Unimplemented, 0, 0};
        return kInvalid;
    }
    return kGames[index];
}

const GameEntry* FindGameById(int id)
{
    for (int i = 0; i < kGameCount; ++i)
    {
        if (kGames[i].id == id)
            return &kGames[i];
    }
    return nullptr;
}

bool IsGameImplemented(int id)
{
    const GameEntry* entry = FindGameById(id);
    return entry != nullptr && entry->status == GameStatus::Implemented;
}
