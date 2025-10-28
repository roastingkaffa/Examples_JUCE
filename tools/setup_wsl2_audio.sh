#!/bin/bash
# WSL2 音频环境配置脚本

echo "================================================"
echo "  WSL2 音频环境配置"
echo "================================================"
echo ""

echo "步骤 1: 安装 ALSA PulseAudio 插件..."
sudo apt-get install -y libasound2-plugins

echo ""
echo "步骤 2: 验证 ALSA 配置..."
aplay -L | grep -E "(pulse|default)" | head -5

echo ""
echo "步骤 3: 测试音频播放..."
speaker-test -t sine -f 440 -c 2 -l 1 2>/dev/null || echo "请手动测试"

echo ""
echo "================================================"
echo "  配置完成！"
echo "  请重启 Simple Gain Plugin 以应用新设置"
echo "================================================"
