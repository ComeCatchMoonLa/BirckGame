# Brick Game

复现 Brick Game 掌机的经典游戏。当前引擎 **0.5**；**0.x 原作**与 **1.1～1.3** 已落地并手测。**1.4～1.14 待人工核实**。

进度入口：[`Docs/开发计划/README.md`](Docs/开发计划/README.md)。

## 环境

- Windows 10/11，CMake ≥ 3.31，C++26
- MinGW-w64 g++ 15（或同级）或 Visual Studio（MSVC）
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

- 启动器：`build/bin/brick_launcher.exe`（输入 1～88，`s` 改 SPEED，0 退出）
- 单测：`build/bin/brick_tests.exe`
- 也可单独运行各游戏 exe。主菜单进增强版：`TetrisPro` / `SnackPro` / `MazeOne`

游戏内 **Esc** 结束进程并回到启动器。空格仍暂停；Game Over 后空格仍重开。

## 目录

```text
BrickGame/Engine     终端探测与兼容 API
BrickGame/Launcher   菜单
BrickGame/<Game>     各游戏（含 1.x 变体）
Docs/                调研、0.x 四件套、1.x 需求试卷
tests/               doctest
```
