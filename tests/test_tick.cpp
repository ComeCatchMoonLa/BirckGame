#include "Tick.h"
#include "doctest.h"
#include <Windows.h>

TEST_CASE("machine speed clamps and defaults to 5")
{
    SetEnvironmentVariableW(L"BRICK_SPEED", nullptr);
    ResetTickState();
    CHECK(GetMachineSpeed() == 5);
    CHECK(ClampMachineSpeed(0) == 1);
    CHECK(ClampMachineSpeed(11) == 10);
    SetMachineSpeed(0);
    CHECK(GetMachineSpeed() == 1);
    SetMachineSpeed(11);
    CHECK(GetMachineSpeed() == 10);
    SetMachineSpeed(7);
    CHECK(GetMachineSpeed() == 7);
}

TEST_CASE("logic interval is slower at SPEED 1 than 10")
{
    ResetTickState();
    SetMachineSpeed(1);
    const int slow = LogicIntervalMs();
    SetMachineSpeed(10);
    const int fast = LogicIntervalMs();
    CHECK(slow == 700);
    CHECK(fast == 60);
    CHECK(slow > fast);
}

TEST_CASE("DueLogicTick is false on the first call")
{
    ResetTickState();
    SetMachineSpeed(10);
    CHECK_FALSE(DueLogicTick());
}
