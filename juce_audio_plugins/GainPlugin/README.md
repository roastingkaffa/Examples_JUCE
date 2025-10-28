# Simple Gain Plugin

一個簡單的音訊增益（Gain）VST3/Standalone 插件，用於演示 JUCE 音訊插件開發。

## 功能特性

- 🎚️ **單一增益參數**: 0.0x ~ 2.0x 可調範圍
- 🎛️ **旋鈕式界面**: 直觀的旋轉滑桿控制
- 📊 **即時顯示**: 數值即時更新顯示
- 🎵 **雙格式支援**: VST3 插件 + Standalone 應用程式
- 💻 **跨平台**: Linux/Windows/macOS (需重新編譯)

## 快速開始

### 執行 Standalone 版本
```bash
cd /home/ateam/workspace/positivegrid/Examples_JUCE
./run_gain_plugin.sh
```

### 使用 VST3 插件
```bash
# 安裝到系統目錄
cp -r build/juce_audio_plugins/GainPlugin/GainPlugin_artefacts/Release/VST3/Simple\ Gain.vst3 ~/.vst3/

# 然後在你的 DAW 中掃描並載入
```

## 檔案結構

```
GainPlugin/
├── src/
│   ├── PluginProcessor.h      # AudioProcessor 介面定義
│   ├── PluginProcessor.cpp    # DSP 音訊處理實作
│   ├── PluginEditor.h         # GUI 編輯器介面定義
│   └── PluginEditor.cpp       # GUI 實作與事件處理
├── CMakeLists.txt             # 建置配置
└── README.md                  # 本文件
```

## 技術細節

### AudioProcessor
- **基礎類別**: `juce::AudioProcessor`
- **處理方法**: `processBlock(AudioBuffer<float>&, MidiBuffer&)`
- **參數管理**: `juce::AudioParameterFloat`
- **支援格式**: Mono/Stereo

### GUI Editor
- **基礎類別**: `juce::AudioProcessorEditor`
- **主要元件**:
  - `Slider` - 旋鈕式增益控制
  - `Label` - 標題與數值顯示
- **視窗大小**: 400×300 px

### 增益處理邏輯
```cpp
void processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
    float gain = gainParameter->get();

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            data[sample] *= gain;  // 簡單的增益乘法
        }
    }
}
```

## 編譯

### 重新編譯
```bash
cd build
cmake --build . --target GainPlugin_Standalone --config Release -j4
```

### 生成檔案
- **Standalone**: `build/.../Standalone/Simple Gain` (5.4 MB)
- **VST3**: `build/.../VST3/Simple Gain.vst3` (bundle 目錄)

## 參數說明

| 參數 | 範圍 | 預設值 | 說明 |
|------|------|--------|------|
| Gain | 0.0 ~ 2.0 | 1.0 | 音訊增益倍數 |

**使用建議**:
- **0.0**: 靜音
- **0.5**: 降低 -6dB
- **1.0**: 原始音量（不變）
- **2.0**: 增加 +6dB

## 依賴項

### JUCE 模組
- `juce_audio_basics` - 音訊緩衝區
- `juce_audio_processors` - AudioProcessor 基礎類別
- `juce_audio_utils` - 音訊工具
- `juce_gui_basics` - GUI 元件

### 系統需求
- **JUCE**: 7.0.12+ (已測試 8.0.10)
- **CMake**: 3.22+
- **編譯器**: GCC 7+ / Clang 5+ / MSVC 2017+
- **C++ 標準**: C++17

## 已知限制

1. **無參數平滑**: 快速調整可能產生爆音
2. **無過載保護**: Gain > 1.0 可能導致削波
3. **單一參數**: 僅支援增益控制
4. **無 MIDI**: 不處理 MIDI 訊息

## 改進建議

### 短期 (容易實作)
- [ ] 新增參數平滑 (`SmoothedValue`)
- [ ] 新增輸出削波保護
- [ ] 新增峰值表顯示

### 中期
- [ ] 支援多頻段增益（3-band EQ）
- [ ] 新增預設管理
- [ ] 美化 GUI (自訂 LookAndFeel)

### 長期
- [ ] SIMD 優化處理
- [ ] 支援 surround (5.1/7.1)
- [ ] 新增自動增益控制 (AGC)

## 參考資源

- [JUCE AudioProcessor 文件](https://docs.juce.com/master/classAudioProcessor.html)
- [JUCE Plugin 教學](https://docs.juce.com/master/tutorial_create_projucer_basic_plugin.html)
- [VST3 SDK 文件](https://steinbergmedia.github.io/vst3_doc/)

## 授權

此範例程式碼用於教學目的。實際專案請遵守 JUCE 授權條款。

## 作者

- 建立日期: 2025-10-28
- 開發工具: Claude Code (Anthropic)
- JUCE 版本: 8.0.10
