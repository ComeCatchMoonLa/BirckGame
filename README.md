# Brick Game

复现 Brick Game 掌机的经典游戏。当前 **0.1** 是壳（终端探测、CMake、启动器）。**0.x 后续先完善引擎**（掌机速度、硬编码优化），未实现玩法放到 1.x。

进度与文档入口：[`Docs/开发计划/README.md`](Docs/开发计划/README.md)。你用来打勾的清单：[`Docs/开发计划/Ver 0.x/0.1/TODO.md`](Docs/开发计划/Ver%200.x/0.1/TODO.md)。

## 环境

- Windows 10/11，CMake ≥ 3.20
- MinGW-w64 g++ 或 Visual Studio（MSVC）
- Cursor 配置用 **Cpp Dev** Profile。C++ 扩展是 `anysphere.cpptools` + `ms-vscode.cmake-tools`（Cursor 市场没有 `ms-vscode.cpptools`）

## 编译

```text
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

MSVC 示例：

```text
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
```

可执行文件在 `build/bin/`。

## 运行

- 启动器：`build/bin/brick_launcher.exe`（输入 1～88，0 退出）
- 单测：`build/bin/brick_tests.exe`
- 也可单独运行各游戏 exe。主菜单进增强版：`TetrisPro` / `SnackPro` / `MazeOne`

旧游戏 Game Over 后会在进程内重开，回到启动器请关闭该游戏窗口。

## 目录

```text
BrickGame/Engine     终端探测与兼容 API
BrickGame/Launcher   菜单
BrickGame/<Game>     各游戏（旧源码）
Docs/                调研与版本三件套 + 接口设计
tests/               doctest
```
