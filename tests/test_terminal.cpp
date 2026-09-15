#include "Terminal.h"
#include "doctest.h"

TEST_CASE("LogicalToColumns maps logical cells to console columns")
{
    CHECK(LogicalToColumns(20, 2) == 40);
    CHECK(LogicalToColumns(0, 2) == 0);
    CHECK(LogicalToColumns(-3, 2) == 0);
    CHECK(LogicalToColumns(10, 1) == 10);
    CHECK(LogicalToColumns(10, 0) == 10);
    CHECK(LogicalToColumns(10, -2) == 10);
}

TEST_CASE("wrap width equals each game SetConsole columns")
{
    CHECK(LogicalToColumns(40, 2) == 80);
    CHECK(LogicalToColumns(15, 2) == 30);
    CHECK(LogicalToColumns(25, 2) == 50);
}

TEST_CASE("ProbeTerminal returns a valid kind and does not crash")
{
    const TerminalInfo info = ProbeTerminal();
    const int kind = static_cast<int>(info.kind);
    CHECK(kind >= static_cast<int>(TerminalKind::ConHostLegacy));
    CHECK(kind <= static_cast<int>(TerminalKind::Redirected));
    CHECK(info.cellColumnsPerBlock >= 1);
}

TEST_CASE("SetCellColumnsPerBlock clamps to at least 1")
{
    SetCellColumnsPerBlock(1);
    CHECK(CachedTerminal().cellColumnsPerBlock == 1);
    SetCellColumnsPerBlock(0);
    CHECK(CachedTerminal().cellColumnsPerBlock == 1);
    SetCellColumnsPerBlock(2);
    CHECK(CachedTerminal().cellColumnsPerBlock == 2);
    ResetTerminalCache();
}

TEST_CASE("CanResizeHost is true only for classic conhost")
{
    CHECK(CanResizeHost(true, TerminalKind::ConHostLegacy));
    CHECK(CanResizeHost(true, TerminalKind::ConHostModern));
    CHECK_FALSE(CanResizeHost(false, TerminalKind::ConHostLegacy));
    CHECK_FALSE(CanResizeHost(false, TerminalKind::ConHostModern));
    CHECK_FALSE(CanResizeHost(true, TerminalKind::WindowsTerminal));
    CHECK_FALSE(CanResizeHost(true, TerminalKind::UnknownConPTY));
    CHECK_FALSE(CanResizeHost(false, TerminalKind::Redirected));
    CHECK_FALSE(CanResizeHost(true, TerminalKind::Redirected));
}

TEST_CASE("ProbeTerminal canResize is false without a lockable host")
{
    const TerminalInfo info = ProbeTerminal();
    if (info.kind != TerminalKind::ConHostLegacy && info.kind != TerminalKind::ConHostModern)
        CHECK_FALSE(info.canResize);
}
