# 0.3 TODO：硬编码与宿主去重

## 1. 引擎

- [x] ClearScreen
- [x] SpawnGameInClassicHost
- [x] canResize 对经典 conhost 为 true

## 2. 启动器 / 游戏

- [x] LaunchGame 改走 SpawnGameInClassicHost
- [x] 菜单 8 款 SetConsole 读注册表
- [x] system("cls") 改为 ClearScreen

## 3. 范围控制

- 抽 Game 基类 -> 否
- 改玩法 -> 否
