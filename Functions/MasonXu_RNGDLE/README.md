rngdle — Python 移植与可打包的 TUI 抽卡程序

简介
- 将 original.cpp 移植为模块化 Python 项目（rngdle），提供带动画的终端抽卡界面：逐位揭示大数字、按 rarity 着色的框与 Badge 流（队首插入）、Roll History、跳过动画与滚动浏览。

快速使用
- 运行： python3 -m rngdle.main
- 指定位数（允许前导 0，但前导 0 在显示时会被隐藏）： python3 -m rngdle.main -n 12
- 打包后运行： ./out/rngdle_cli

打包（PyInstaller / Docker，推荐在 Ubuntu 20.04 上构建）
- 本仓库包含 hook-readchar.py，用于把 readchar 的元数据一并打包（避免 frozen 二进制在运行时抛出 importlib.metadata.PackageNotFoundError）。
- 本地（Docker）一键构建： ./build_exe.sh
- 手动 Docker 构建： docker build -t rngdle-builder -f docker-build/Dockerfile . && docker run --rm --name rngdle_build rngdle-builder
- 产物位置：./out/rngdle_cli

注意事项
- 在真实终端（gnome-terminal/kitty/xterm）中运行以获得正确的 TUI 效果；在非交互 stdin（IDE 面板、systemd、重定向）可能导致 termios 报错（"Inappropriate ioctl for device"）。
- 若想彻底移除 readchar（提高 frozen 稳定性），项目已包含内部键盘读取器 rngdle._keyutil，可按需切换实现。

源码结构（重要文件）
- rngdle/cli.py — 主交互与渲染（Rich 驱动）
- rngdle/utils.py — badge 检测与 EP/rank 计算（与 original.cpp 对齐）
- rngdle/badges.py — badge 数据
- hook-readchar.py — PyInstaller hook，用于打包 readchar 元数据

反馈与测试
- 在真实终端运行后，如遇界面错位、颜色或按键行为不符，请把终端输出 / Traceback 粘贴到 issue 中。欢迎说明目标终端与 Python 版本以便复现调试。
