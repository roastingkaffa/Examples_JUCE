# WSL2 耳機+麥克風測試 Simple Gain Plugin 指南

## 🎯 你的環境狀況

✅ **已確認**:
- WSL2 環境 (Linux 5.15.167.4-microsoft-standard-WSL2)
- WSLg 已啟用 (PULSE_SERVER=/mnt/wslg/PulseServer)
- Simple Gain Plugin 已編譯完成
- 使用帶麥克風的耳機插入 PC 耳機孔

❌ **問題**:
- ALSA lib seq_hw.c:466 錯誤 → **可忽略**（Plugin 不使用 MIDI）
- 缺少 PulseAudio 客戶端工具
- WSL2 無法直接訪問 /dev/snd 音訊設備

---

## 🔧 解決方案

### 步驟 1: 安裝音訊工具

```bash
# 安裝 PulseAudio 和 ALSA 工具
sudo apt-get update
sudo apt-get install -y pulseaudio-utils alsa-utils

# 驗證安裝
pactl --version
```

### 步驟 2: 驗證音訊設備

運行音訊設備檢測腳本：

```bash
./tools/test_audio_setup.sh
```

**預期輸出**：
```
✅ PulseAudio 服務器已配置
✅ pactl 已安裝

音頻輸出設備（耳機）：
0    alsa_output.pci-0000_00_1f.3.analog-stereo    ...

音頻輸入設備（麥克風）：
1    alsa_input.pci-0000_00_1f.3.analog-stereo    ...
```

### 步驟 3: 確認 Windows 音訊設置

**在 Windows 主機上**（非常重要！）：

1. **打開 Windows 聲音設置**:
   - 右鍵點擊任務欄音量圖標
   - 選擇 "聲音設置" 或 "Open Sound settings"

2. **設置輸入設備（麥克風）**:
   ```
   輸入 → 選擇輸入設備
   └── 選擇你的耳機麥克風
   └── 調整輸入音量 (建議 70-80%)
   └── 測試麥克風（對著說話，應該看到音量條移動）
   ```

3. **設置輸出設備（耳機）**:
   ```
   輸出 → 選擇輸出設備
   └── 選擇你的耳機
   └── 調整輸出音量 (建議 50-70%)
   └── 測試喇叭（點擊測試按鈕）
   ```

4. **確認設備作為默認設備**:
   - 右鍵耳機設備
   - 選擇 "設置為默認設備"
   - 選擇 "設置為默認通訊設備"

### 步驟 4: 啟動 Simple Gain Plugin

```bash
./run_gain_plugin.sh
```

### 步驟 5: 配置 Plugin 音訊設置

當 Plugin 視窗打開後：

1. **打開音訊設置視窗**:
   - 點擊視窗菜單 `Options` → `Audio/MIDI Settings`
   - 或按快捷鍵 `Ctrl+,`

2. **配置音訊參數**:
   ```
   Audio Device Type: PulseAudio (或 ALSA 如果可用)

   Input Device (輸入):
   └── 選擇 "Default" 或包含 "analog-stereo" 的設備

   Output Device (輸出):
   └── 選擇 "Default" 或包含 "analog-stereo" 的設備

   Sample Rate (取樣率):
   └── 44100 Hz (推薦) 或 48000 Hz

   Buffer Size (緩衝區):
   └── 256 samples (平衡延遲與穩定性)
       或 512 samples (如果有音訊斷續問題)
   ```

3. **確認通道啟用**:
   - Active Input Channels: ✅ Channel 1, ✅ Channel 2
   - Active Output Channels: ✅ Channel 1, ✅ Channel 2

4. **點擊 "OK" 或 "Apply"** 應用設置

### 步驟 6: 測試麥克風→耳機音訊流

⚠️ **重要提醒**: 請戴上耳機測試，避免喇叭回授！

1. **戴上你的耳機**

2. **對著麥克風說話**
   - 應該能在耳機中聽到自己的聲音
   - 可能有輕微延遲（正常現象）

3. **調整 Gain 旋鈕測試效果**:
   ```
   Gain = 0.0 → 完全靜音（應該聽不到聲音）
   Gain = 0.5 → 音量減半
   Gain = 1.0 → 原始音量（1:1 傳輸）
   Gain = 1.5 → 放大 1.5 倍
   Gain = 2.0 → 放大 2 倍（最大值）
   ```

4. **觀察數值顯示**:
   - 旋轉旋鈕時，數值應該即時更新
   - 音量變化應該明顯可聽

---

## ✅ 測試成功的標誌

- [ ] Plugin 視窗正常開啟，無崩潰
- [ ] 能在音訊設置中看到輸入/輸出設備
- [ ] 對著麥克風說話，能在耳機中聽到自己的聲音
- [ ] 旋轉 Gain 旋鈕，聽到音量明顯變化
- [ ] Gain = 0.0 時完全靜音
- [ ] Gain = 2.0 時音量明顯放大
- [ ] 數值顯示與旋鈕位置一致

---

## 🔧 常見問題排解

### ❌ 問題 1: 完全沒有聲音

**檢查清單**:
1. Windows 音訊設置中，耳機是否設為默認設備？
2. Windows 麥克風權限是否允許應用訪問？
   - 設置 → 隱私 → 麥克風 → 允許應用訪問
3. Plugin 音訊設置中，Input/Output 是否選擇了正確設備？
4. Gain 參數是否設為 0？（調到 1.0 測試）
5. 系統音量是否開啟？

**測試步驟**:
```bash
# 測試 PulseAudio 連接
pactl info

# 測試播放音效
paplay /usr/share/sounds/alsa/Front_Center.wav

# 如果以上都無聲音，問題在 WSL2 音訊路由
```

### ❌ 問題 2: 有輸出但沒有輸入

**原因**: Windows 麥克風未正確配置

**解決**:
1. 在 Windows 中測試麥克風 (設置 → 系統 → 聲音 → 測試麥克風)
2. 確認 WSL2 能訪問麥克風：
   ```bash
   # 錄製 5 秒音頻測試
   arecord -d 5 -f cd test.wav
   aplay test.wav
   ```

### ❌ 問題 3: 音訊斷斷續續

**原因**: Buffer 太小或 CPU 不足

**解決**:
1. 增加 Buffer Size: 256 → 512 → 1024
2. 關閉其他占用 CPU 的程式
3. 降低 Sample Rate: 48000 → 44100

### ❌ 問題 4: 延遲太高（超過 100ms）

**原因**: Buffer Size 太大

**解決**:
1. 降低 Buffer Size: 512 → 256 → 128
2. 注意：太小可能導致音訊斷續

### ❌ 問題 5: ALSA sequencer 錯誤持續出現

```
ALSA lib seq_hw.c:466:(snd_seq_hw_open) open /dev/snd/seq failed
```

**說明**: 這是 MIDI 相關錯誤，不影響音訊功能

**為什麼出現**:
- WSL2 不支援 /dev/snd/seq MIDI sequencer 設備
- Simple Gain Plugin 不使用 MIDI
- JUCE 在初始化時會嘗試檢測 MIDI 設備

**處理方式**:
✅ **可以完全忽略**，Plugin 音訊功能不受影響

---

## 🎵 替代測試方案

如果 WSL2 音訊問題無法解決，可以使用以下替代方案：

### 方案 A: Windows 原生編譯（推薦）

1. **安裝 Visual Studio 2019/2022** (Community 版免費)
2. **在 Windows CMD/PowerShell 中編譯**:
   ```powershell
   cd C:\path\to\Examples_JUCE
   mkdir build-win
   cd build-win
   cmake -G "Visual Studio 17 2022" ..
   cmake --build . --config Release
   ```
3. **運行 Windows 版本**:
   ```powershell
   .\build-win\juce_audio_plugins\GainPlugin\GainPlugin_artefacts\Release\Standalone\Simple Gain.exe
   ```

### 方案 B: 使用虛擬音訊設備

1. **安裝 VB-CABLE** (Windows 虛擬音訊驅動)
2. **配置音訊路由**:
   ```
   音樂播放器 → VB-Cable Input
   Simple Gain Plugin Input ← VB-Cable Output
   Simple Gain Plugin Output → 耳機
   ```

### 方案 C: 使用 VST3 在 DAW 中測試

1. **安裝 VST3 插件**:
   ```bash
   mkdir -p ~/.vst3
   cp -r "build/juce_audio_plugins/GainPlugin/GainPlugin_artefacts/Release/VST3/Simple Gain.vst3" ~/.vst3/
   ```

2. **使用 DAW** (如 Reaper, Bitwig, Ardour):
   - 創建音訊軌道
   - 載入音訊文件
   - 插入 Simple Gain 插件
   - 調整參數聽效果

---

## 📊 完整測試流程總結

```
1. 安裝音訊工具
   └── sudo apt-get install pulseaudio-utils alsa-utils

2. 驗證音訊設備
   └── ./tools/test_audio_setup.sh

3. 配置 Windows 音訊
   └── 設置耳機為默認輸入/輸出設備
   └── 測試麥克風和喇叭

4. 啟動 Plugin
   └── ./run_gain_plugin.sh

5. 配置 Plugin 音訊
   └── Options → Audio/MIDI Settings
   └── Input: Default / analog-stereo
   └── Output: Default / analog-stereo
   └── Sample Rate: 44100 Hz
   └── Buffer Size: 256 samples

6. 測試音訊流
   └── 戴上耳機
   └── 對著麥克風說話
   └── 聽到自己的聲音 ✅

7. 測試 Gain 效果
   └── 旋轉旋鈕 0.0 → 2.0
   └── 聽到音量變化 ✅
```

---

## 📝 預期結果

**成功案例**:
```
✅ 麥克風輸入 → Simple Gain Plugin (× Gain) → 耳機輸出
✅ Gain = 1.0: 聽到原始音量的自己說話
✅ Gain = 0.5: 聽到較小聲的自己說話
✅ Gain = 2.0: 聽到放大的自己說話
✅ 數值顯示正確更新
```

**驗證 Plugin 功能**:
- ✅ 音訊路由正常 (Input → Processing → Output)
- ✅ Gain 參數控制正常
- ✅ UI 與音訊處理同步
- ✅ 即時處理無崩潰

---

## 🆘 仍有問題？

提供以下資訊以便診斷：

```bash
# 1. 系統資訊
uname -a
echo $PULSE_SERVER

# 2. 音訊設備狀態
pactl list sinks short
pactl list sources short

# 3. 測試音訊輸出
paplay /usr/share/sounds/alsa/Front_Center.wav

# 4. Plugin 啟動訊息
./run_gain_plugin.sh 2>&1 | tee plugin_output.log
```

將輸出結果提供給我，我會幫你進一步診斷！

---

**祝測試順利！🎵🎧**
