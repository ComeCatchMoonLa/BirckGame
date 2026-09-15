cmake_minimum_required(VERSION 3.31)
project(BrickGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(brick_engine STATIC
    BrickGame/Engine/Terminal.cpp
    BrickGame/Engine/BrickEngine.cpp
    BrickGame/Engine/GameRegistry.cpp
    BrickGame/Engine/Tick.cpp
    BrickGame/Engine/Scores.cpp
)
target_include_directories(brick_engine PUBLIC BrickGame/Engine)
if(WIN32)
    target_link_libraries(brick_engine PUBLIC user32)
endif()
if(MSVC)
    target_compile_options(brick_engine PRIVATE /utf-8 /W4)
else()
    target_compile_options(brick_engine PRIVATE -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

function(add_legacy_game name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE brick_engine)
    if(MSVC)
        target_compile_options(${name} PRIVATE /utf-8)
    else()
        target_compile_options(${name} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
    endif()
endfunction()

add_legacy_game(Tetris "BrickGame/Tetris/Tetris.cpp")
add_legacy_game(TetrisPro "BrickGame/Tetris/TetrisPro.cpp")
add_legacy_game(Snack "BrickGame/Snack/Snack.cpp")
add_legacy_game(SnackPro "BrickGame/Snack/SnackPro.cpp")
add_legacy_game(Eliminate "BrickGame/Eliminate/Eliminate.cpp")
add_legacy_game(FullUp "BrickGame/FullUp/FullUp.cpp")
add_legacy_game(NaughtyBrick "BrickGame/NaughtyBrick/NaughtyBrick.cpp")
add_legacy_game(CantMove "BrickGame/Can't Move/Can't Move.cpp")
add_legacy_game(Racing "BrickGame/Racing/Racing.cpp")
add_legacy_game(MazeOne "BrickGame/Maze/Maze One.cpp")
add_legacy_game(MazeRoot "BrickGame/Maze/Maze Root.cpp")
add_legacy_game(MirrorTetris "BrickGame/Tetris/variants/MirrorTetris.cpp")
add_legacy_game(PentoTetris "BrickGame/Tetris/variants/PentoTetris.cpp")
add_legacy_game(InvisibleTetris "BrickGame/Tetris/variants/InvisibleTetris.cpp")
add_legacy_game(OOnlyTetris "BrickGame/Tetris/variants/OOnlyTetris.cpp")
add_legacy_game(BombTetris "BrickGame/Tetris/variants/BombTetris.cpp")
add_legacy_game(FortyLineTetris "BrickGame/Tetris/variants/FortyLineTetris.cpp")
add_legacy_game(GarbageTetris "BrickGame/Tetris/variants/GarbageTetris.cpp")
add_legacy_game(NoRotateTetris "BrickGame/Tetris/variants/NoRotateTetris.cpp")
add_legacy_game(ColorTetris "BrickGame/Tetris/variants/ColorTetris.cpp")
add_legacy_game(GravityTetris "BrickGame/Tetris/variants/GravityTetris.cpp")
add_legacy_game(WideTetris "BrickGame/Tetris/variants/WideTetris.cpp")
add_legacy_game(NarrowTetris "BrickGame/Tetris/variants/NarrowTetris.cpp")
add_legacy_game(WarpSnack "BrickGame/Snack/WarpSnack.cpp")
add_legacy_game(ObstacleSnack "BrickGame/Snack/ObstacleSnack.cpp")
add_legacy_game(TimedSnack "BrickGame/Snack/TimedSnack.cpp")
add_legacy_game(SpeedSnack "BrickGame/Snack/SpeedSnack.cpp")
add_legacy_game(WallSnack "BrickGame/Snack/WallSnack.cpp")

add_executable(brick_launcher BrickGame/Launcher/Main.cpp)
target_link_libraries(brick_launcher PRIVATE brick_engine)
if(MSVC)
    target_compile_options(brick_launcher PRIVATE /utf-8 /W4)
else()
    target_compile_options(brick_launcher PRIVATE -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

add_executable(brick_tests
    tests/test_main.cpp
    tests/test_terminal.cpp
    tests/test_registry.cpp
    tests/test_tick.cpp
    tests/test_scores.cpp
)
target_link_libraries(brick_tests PRIVATE brick_engine)
target_include_directories(brick_tests PRIVATE tests/third_party)
if(MSVC)
    target_compile_options(brick_tests PRIVATE /utf-8 /W4)
else()
    target_compile_options(brick_tests PRIVATE -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

enable_testing()
add_test(NAME brick_tests COMMAND brick_tests)

if(NOT MSVC)
    get_filename_component(MINGW_BIN "${CMAKE_CXX_COMPILER}" DIRECTORY)
    foreach(dll libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        if(EXISTS "${MINGW_BIN}/${dll}")
            add_custom_command(TARGET brick_launcher POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN}/${dll}" "$<TARGET_FILE_DIR:brick_launcher>/${dll}")
        endif()
    endforeach()
endif()
