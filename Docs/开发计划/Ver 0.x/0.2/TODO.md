# 0.2 TODO：整机 SPEED 节拍

> 需求以 [游戏设计.md](游戏设计.md) 为准，技术边界以 [技术设计.md](技术设计.md) 为准，签名以 [接口设计.md](接口设计.md) 为准。

## 1. 引擎

- [x] Tick.h / Tick.cpp：夹取、读写档、PumpFrame、DueLogicTick、ResetTickState
- [x] brick_engine 链入 Tick.cpp；BrickEngine.h 包含 Tick.h
- [x] tests/test_tick.cpp：夹取、缺省、间隔 SPEED1 > SPEED10

## 2. 启动器

- [x] 顶栏 SPEED；s 循环 1～10
- [x] CreateProcess 前 SetMachineSpeed

## 3. 已实现游戏

- [x] Tetris / TetrisPro：PumpFrame + DueLogicTick 下落
- [x] Snack / SnackPro：同上；Z 仍加速一步
- [x] Eliminate / FullUp / CantMove / Racing / NaughtyBrick：PumpFrame + 逻辑拍上走旧 t++
- [x] Maze One / Maze Root：循环里 PumpFrame

## 4. 范围控制

- ITickable / LEVEL / 双缓冲 -> 否
- 改 SetConsole 签名 -> 否
- 未实现玩法 -> 1.x

## 5. 手测（留给你）

- [x] SPEED 1 的方块或蛇慢于 SPEED 10
- [ ] 迷宫不再风扇狂转
