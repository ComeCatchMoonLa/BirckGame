#include "GameRegistry.h"
#include "doctest.h"
#include <cstring>

TEST_CASE("registry has 88 games numbered 1 to 88")
{
    REQUIRE(GetGameCount() == 88);
    for (int i = 0; i < GetGameCount(); ++i)
        CHECK(GetGameAt(i).id == i + 1);
    CHECK(FindGameById(99) == nullptr);
    CHECK(GetGameAt(-1).id == 0);
}

TEST_CASE("implemented set and exe names match 0.1 contract")
{
    const int implemented[] = {1, 2, 3, 4, 5, 6, 7, 17, 36, 37, 38, 39, 40, 42, 43, 44, 45, 46, 47, 48,
                               49, 51, 52, 53, 54};
    for (int id : implemented)
        CHECK(IsGameImplemented(id));

    CHECK_FALSE(IsGameImplemented(8));
    CHECK_FALSE(IsGameImplemented(88));
    CHECK(FindGameById(8)->exeFileName == nullptr);

    CHECK(std::strcmp(FindGameById(1)->exeFileName, "TetrisPro") == 0);
    CHECK(std::strcmp(FindGameById(2)->exeFileName, "SnackPro") == 0);
    CHECK(std::strcmp(FindGameById(3)->exeFileName, "Eliminate") == 0);
    CHECK(std::strcmp(FindGameById(4)->exeFileName, "FullUp") == 0);
    CHECK(std::strcmp(FindGameById(5)->exeFileName, "NaughtyBrick") == 0);
    CHECK(std::strcmp(FindGameById(6)->exeFileName, "CantMove") == 0);
    CHECK(FindGameById(6)->windowColumns == 320);
    CHECK(FindGameById(6)->windowRows == 120);
    CHECK(std::strcmp(FindGameById(7)->exeFileName, "Racing") == 0);
    CHECK(std::strcmp(FindGameById(17)->exeFileName, "MazeOne") == 0);

    CHECK(std::strcmp(FindGameById(36)->exeFileName, "MirrorTetris") == 0);
    CHECK(std::strcmp(FindGameById(37)->exeFileName, "PentoTetris") == 0);
    CHECK(FindGameById(37)->windowColumns == 50);
    CHECK(std::strcmp(FindGameById(38)->exeFileName, "InvisibleTetris") == 0);
    CHECK(std::strcmp(FindGameById(40)->exeFileName, "BombTetris") == 0);
    CHECK(std::strcmp(FindGameById(43)->exeFileName, "GarbageTetris") == 0);
    CHECK(std::strcmp(FindGameById(45)->exeFileName, "ColorTetris") == 0);
    CHECK(std::strcmp(FindGameById(46)->exeFileName, "GravityTetris") == 0);
    CHECK(std::strcmp(FindGameById(39)->exeFileName, "OOnlyTetris") == 0);
    CHECK(std::strcmp(FindGameById(42)->exeFileName, "FortyLineTetris") == 0);
    CHECK(std::strcmp(FindGameById(44)->exeFileName, "NoRotateTetris") == 0);
    CHECK(std::strcmp(FindGameById(47)->exeFileName, "WideTetris") == 0);
    CHECK(FindGameById(47)->windowColumns == 50);
    CHECK(std::strcmp(FindGameById(48)->exeFileName, "NarrowTetris") == 0);
    CHECK(FindGameById(48)->windowColumns == 32);
    CHECK(std::strcmp(FindGameById(49)->exeFileName, "WarpSnack") == 0);
    CHECK(FindGameById(49)->windowColumns == 80);
    CHECK(std::strcmp(FindGameById(51)->exeFileName, "ObstacleSnack") == 0);
    CHECK(std::strcmp(FindGameById(52)->exeFileName, "TimedSnack") == 0);
    CHECK(std::strcmp(FindGameById(53)->exeFileName, "SpeedSnack") == 0);
    CHECK(std::strcmp(FindGameById(54)->exeFileName, "WallSnack") == 0);
    CHECK(FindGameById(1)->windowColumns == 40);
    CHECK(FindGameById(1)->windowRows == 27);
    CHECK(FindGameById(2)->windowColumns == 80);
    CHECK(FindGameById(2)->windowRows == 30);
    CHECK(FindGameById(7)->windowColumns == 30);
    CHECK(FindGameById(7)->windowRows == 30);
    CHECK(FindGameById(17)->windowColumns == 102);
    CHECK(FindGameById(8)->windowColumns == 0);
    CHECK(std::strcmp(FindGameById(1)->name, "俄罗斯方块") == 0);
    CHECK(std::strcmp(FindGameById(8)->name, "青蛙过河") == 0);
    CHECK(std::strcmp(FindGameById(21)->name, "吃豆人") == 0);
    CHECK(std::strcmp(FindGameById(28)->name, "打砖块") == 0);
    CHECK(std::strcmp(FindGameById(35)->name, "华容道") == 0);
    CHECK(std::strcmp(FindGameById(36)->name, "镜像方块") == 0);
    CHECK(std::strcmp(FindGameById(88)->name, "一笔画") == 0);
}
