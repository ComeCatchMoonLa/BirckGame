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
    const int implemented[] = {1, 2, 3, 4, 5, 6, 7, 17};
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
