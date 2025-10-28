#!/bin/bash
# HelloWorldLabel 啟動腳本

echo "================================================"
echo "  啟動 HelloWorldLabel 範例程式"
echo "================================================"
echo ""
echo "預期視覺效果："
echo "  - 視窗標題: Hello world (Label)"
echo "  - 綠色文字: Hello new work labe1"
echo "  - 關閉按鈕: Close Window"
echo ""
echo "測試項目："
echo "  1. 確認文字顯示為 'Hello new work labe1'"
echo "  2. 點擊 'Close Window' 按鈕"
echo "  3. 確認視窗正常關閉"
echo ""
echo "啟動中..."
echo "================================================"
echo ""

cd "$(dirname "$0")/build"
./juce_gui_basics/HelloWorlds/HelloWorldLabel/HelloWorldLabel_artefacts/Release/HelloWorldLabel

echo ""
echo "================================================"
echo "程式已結束"
echo "================================================"
