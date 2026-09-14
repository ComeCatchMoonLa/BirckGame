cmake_minimum_required(VERSION 3.20)
project(BrickGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(brick_engine STATIC
    BrickGame/Engine/Terminal.cpp
    BrickGame/Engine/BrickEngine.cpp
    BrickGame/Engine/GameRegistry.cpp
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
