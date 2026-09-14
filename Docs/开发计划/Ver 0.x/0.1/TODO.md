# 0.1 TODO：引擎、终端与启动器

> **预计 1～2 个有效开发日。** 风险：把 SetConsole 的 width 再乘一次 2，旧游戏窗口翻倍；启动器找不到同目录 exe。
>
> 需求以 [游戏设计.md](游戏设计.md) 为准，技术边界以 [技术设计.md](技术设计.md) 为准，签名以 [接口设计.md](接口设计.md) 为准。

## 0. 开始前检查与复核

- [x] 仓库已在 Brick Game，与 origin/main 同步后开工
- [x] Cpp Dev：去掉 Unity/C#（Profile 内），安装 Cursor 可用的 C++ 扩展（anysphere.cpptools + cmake-tools；市场无 ms-vscode.cpptools）
- [x] 规范：.editorconfig / .clang-format / .cursor/rules / reviewer
- [x] 调研与路线图已写；后续 0.x 先做引擎（0.2 掌机速度、0.3 硬编码优化仅大纲）
- [x] 接口设计无未拍 [待确认]

**阶段门槛：** 可以进第 1 节。

## 1. 引擎探测与兼容 API（约 3h）

- [x] Terminal.h / Terminal.cpp：ProbeTerminal、LogicalToColumns、ApplyConsoleSize、ApplyConsoleColor
- [x] BrickEngine.h / BrickEngine.cpp：保持四函数 + Fill* + Pause 签名；内部走探测
- [x] 旧游戏继续 include 引擎头，不新增 mode con
- [x] tests/test_terminal.cpp：列映射；探测不崩且 kind 在枚举内

**阶段门槛：** 单测相关项绿；旧游戏源码除 CMake 接入外无玩法改动。

## 2. 注册表与 CMake（约 2h）

- [x] GameRegistry.h / GameRegistry.cpp：88 款，已实现 {1-7, 17}
- [x] 根 CMakeLists.txt：brick_engine、各游戏 exe、brick_tests；输出 build/bin
- [x] 全部目标 UTF-8（源码与执行字符集）
- [x] tests/test_registry.cpp：数量、编号、exe 名、未实现为空

**阶段门槛：** brick_tests 全绿。

## 3. 启动器（约 2h）

- [x] BrickGame/Launcher/Main.cpp：菜单、0 退出、未实现提示、CreateProcess、返回后重画
- [ ] 手测：Windows Terminal 开启动器；进 1 或 17；关游戏窗口回菜单；选 8 提示未实现

**阶段门槛：** 对照游戏设计第 5 节能走通主路径。

## 4. 文档与交付

- [x] 更新根 README.md：如何配置/编译/运行
- [x] 本 TODO 实现节勾完；手测与审查项留给你打勾
- [x] 不开始 0.2 代码

## 范围控制

- 未实现玩法（青蛙过河起） -> 1.x；0.2 起先做引擎
- 单进程 RunXxx() / 双缓冲 / 跨平台 -> 否
- 为未实现项建空文件夹或空类 -> 否
- 全局卸载 Unity 扩展（弄坏 Unity Game Dev）-> 否
- YuWen 已删除，不恢复、不进菜单

只有阻止本版本验收的问题才进入当日任务。
