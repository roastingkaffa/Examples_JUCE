#!/bin/bash
# WSL2 音频设备验证脚本

echo "================================================"
echo "  WSL2 音频设备检测"
echo "================================================"
echo ""

echo "1. 检查 PulseAudio 服务器连接..."
echo "   PULSE_SERVER: $PULSE_SERVER"
if [ -z "$PULSE_SERVER" ]; then
    echo "   ❌ 未设置 PULSE_SERVER 环境变量"
    echo "   请确保 WSLg 已启用"
else
    echo "   ✅ PulseAudio 服务器已配置"
fi
echo ""

echo "2. 检查 PulseAudio 工具是否安装..."
if command -v pactl &> /dev/null; then
    echo "   ✅ pactl 已安装"
    echo ""
    echo "3. 列出音频输出设备（耳机）..."
    pactl list sinks short
    echo ""
    echo "4. 列出音频输入设备（麦克风）..."
    pactl list sources short
    echo ""
    echo "5. 测试音频输出（播放测试音）..."
    echo "   按 Ctrl+C 停止..."
    paplay /usr/share/sounds/alsa/Front_Center.wav 2>/dev/null || echo "   ℹ️  无测试音频文件，跳过"
else
    echo "   ❌ pactl 未安装"
    echo ""
    echo "请先安装 PulseAudio 工具："
    echo "   sudo apt-get install pulseaudio-utils alsa-utils"
fi

echo ""
echo "================================================"
echo "  检测完成"
echo "================================================"
