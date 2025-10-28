# Simple Gain Plugin 音訊設定指南

## 🎧 快速設定步驟

### 1. 開啟音訊設定視窗

在 Plugin 視窗中：
- **Windows/Linux**: `Options` → `Audio/MIDI Settings`
- **macOS**: `Preferences` → `Audio/MIDI Settings`
- **快捷鍵**: 通常按 `Ctrl+,` (Linux/Windows) 或 `Cmd+,` (macOS)

### 2. 音訊設備配置

#### 📋 Audio Device Type (音訊驅動類型)
選擇合適的音訊驅動：

**Linux 系統**:
- **ALSA** (推薦) - 適用於大多數 Linux 系統
- **JACK** - 專業音訊路由（如果已安裝）
- **PulseAudio** - 部分系統可用

**Windows 系統**:
- **ASIO** (推薦) - 低延遲專業驅動
- **Windows Audio** - 系統預設
- **DirectSound** - 較舊的選項

**macOS 系統**:
- **CoreAudio** (預設) - macOS 原生音訊系統

#### 🎤 Input Device (輸入設備)
選擇音訊來源：
- **內建麥克風** - 測試麥克風輸入
- **外接音訊介面** - 如 Focusrite, Behringer 等
- **虛擬音訊裝置** - 如 VB-Cable, BlackHole

**建議設定**:
```
Input Device: Default (系統預設)
或
Input Device: Built-in Microphone
```

#### 🔊 Output Device (輸出設備)
選擇音訊輸出：
- **內建喇叭/耳機孔** - 筆電內建
- **外接音訊介面** - 專業監聽
- **USB 耳機/喇叭** - USB 音訊設備

**建議設定**:
```
Output Device: Default (系統預設)
或
Output Device: Built-in Audio (內建音訊)
```

#### 📊 Sample Rate (取樣率)
選擇取樣率（越高品質越好，但 CPU 負擔越大）：
- **44100 Hz** ✅ (CD 品質，推薦)
- **48000 Hz** ✅ (影片標準)
- **96000 Hz** (高解析音訊)
- **192000 Hz** (專業級，較耗資源)

**建議設定**: `44100 Hz` 或 `48000 Hz`

#### 🔢 Buffer Size (緩衝區大小)
控制延遲與穩定性：

| Buffer Size | 延遲 @ 48kHz | 用途 | CPU 負擔 |
|-------------|--------------|------|----------|
| 64 samples  | 1.3 ms      | 即時演奏 | 高 ⚠️ |
| 128 samples | 2.7 ms      | 一般錄音 | 中 ✅ |
| 256 samples | 5.3 ms      | 混音處理 | 低 ✅ |
| 512 samples | 10.7 ms     | 離線處理 | 極低 |

**建議設定**:
- 效能充足: `128 samples` (低延遲)
- 效能有限: `256 samples` (穩定性優先)

### 3. 測試音訊路由

#### ✅ 配置完成後的檢查清單

檢查以下項目是否正確：
- [ ] Input Device 已選擇（有音訊來源）
- [ ] Output Device 已選擇（有輸出設備）
- [ ] Sample Rate 已設定（建議 44100 或 48000）
- [ ] Buffer Size 已設定（建議 128 或 256）
- [ ] Active Input Channels 顯示已啟用通道
- [ ] Active Output Channels 顯示已啟用通道

---

## 🎚️ 測試音訊效果

### 方法 1: 使用麥克風測試（最簡單）

1. **開啟 Plugin**
2. **配置音訊設定**:
   ```
   Input: Built-in Microphone
   Output: Built-in Audio / Headphones
   ```
3. **對著麥克風說話**
4. **調整 Gain 旋鈕**:
   - Gain = 0.0 → 應該聽不到聲音
   - Gain = 1.0 → 聽到原始音量
   - Gain = 2.0 → 聽到放大 2 倍的聲音

⚠️ **注意**: 如果 Gain 設太高可能會產生回授嘯叫，請使用耳機測試！

### 方法 2: 使用音訊檔案播放器（推薦）

#### 步驟 A: 安裝 JACK Audio (Linux)
```bash
# Ubuntu/Debian
sudo apt-get install jackd2 qjackctl

# 啟動 JACK
qjackctl &
```

#### 步驟 B: 設定路由
1. 開啟 QjackCtl
2. 啟動 JACK 服務器
3. 開啟 Simple Gain Plugin，選擇 JACK 作為音訊驅動
4. 開啟音樂播放器（如 Audacity, VLC）
5. 使用 QjackCtl Connections 連接：
   ```
   [播放器 Output] → [Simple Gain Input]
   [Simple Gain Output] → [系統 Playback]
   ```

#### 步驟 C: 播放音樂並調整 Gain
- 播放音樂
- 旋轉 Gain 旋鈕
- 聽到音量變化即成功！

### 方法 3: 使用 DAW 測試（最專業）

更簡單的方式是在 DAW 中使用 VST3 版本：

1. **安裝 VST3**:
   ```bash
   cp -r "build/juce_audio_plugins/GainPlugin/GainPlugin_artefacts/Release/VST3/Simple Gain.vst3" ~/.vst3/
   ```

2. **開啟 DAW** (Reaper/Bitwig/Ardour)

3. **建立音訊軌道**

4. **載入音訊檔案或錄音**

5. **插入 Simple Gain 插件**:
   - 在 FX 鏈中加入 "Simple Gain"
   - 調整 Gain 參數
   - 即時聽到效果

---

## 🔧 常見問題排解

### ❌ 問題 1: 沒有聲音

**檢查項目**:
- [ ] 音訊設定中是否選擇了正確的輸入/輸出設備？
- [ ] 系統音量是否開啟（檢查系統設定）？
- [ ] 耳機/喇叭是否正確連接？
- [ ] Gain 參數是否設為 0？（調整到 1.0 測試）

**解決方法**:
```bash
# 檢查系統音訊設備
aplay -l          # 列出播放設備
arecord -l        # 列出錄音設備

# 測試系統音訊
speaker-test -t sine -f 440 -c 2
```

### ❌ 問題 2: ALSA 錯誤訊息

```
ALSA lib seq_hw.c:466:(snd_seq_hw_open) open /dev/snd/seq failed
```

**原因**: 缺少 MIDI sequencer 支援
**影響**: 無（Plugin 不使用 MIDI）
**解決**: 可忽略，或安裝：
```bash
sudo apt-get install alsa-utils
```

### ❌ 問題 3: 音訊斷斷續續 (Dropouts)

**原因**: Buffer Size 太小或 CPU 不足
**解決**:
1. 增加 Buffer Size (128 → 256 → 512)
2. 降低 Sample Rate (96000 → 48000)
3. 關閉其他占用 CPU 的程式

### ❌ 問題 4: 延遲太高

**原因**: Buffer Size 太大
**解決**:
1. 降低 Buffer Size (512 → 256 → 128)
2. 使用 ASIO (Windows) 或 JACK (Linux)
3. 使用專業音訊介面

### ❌ 問題 5: 找不到音訊設定選單

**在 Standalone 視窗中尋找**:
- 視窗頂部選單列
- 齒輪圖示按鈕
- 右鍵點擊視窗標題列

如果仍找不到，Plugin 可能需要重新編譯以包含設定 UI。

---

## 🎛️ 進階設定

### 多通道配置

在音訊設定中，您可能看到：

**Active Input Channels**:
```
□ Channel 1 (Left)
□ Channel 2 (Right)
□ Channel 3
...
```

對於立體聲處理，確保勾選：
- ✅ Channel 1
- ✅ Channel 2

**Active Output Channels**:
```
☑ Channel 1 (Left)
☑ Channel 2 (Right)
```

### 低延遲優化 (Linux)

#### 使用 JACK 設定
```bash
# 安裝 JACK
sudo apt-get install jackd2 qjackctl

# 啟動 JACK with 低延遲設定
jackd -R -d alsa -r 48000 -p 128 -n 2
```

參數說明：
- `-R`: Real-time mode (即時模式)
- `-d alsa`: 使用 ALSA 驅動
- `-r 48000`: 48kHz sample rate
- `-p 128`: 128 samples buffer size
- `-n 2`: 2 buffers

---

## 📝 推薦配置範例

### 配置 1: 低延遲即時演奏
```
Audio Device Type: JACK / ASIO
Input: Audio Interface Input 1-2
Output: Audio Interface Output 1-2
Sample Rate: 48000 Hz
Buffer Size: 128 samples
```

### 配置 2: 一般使用（平衡）
```
Audio Device Type: ALSA / Windows Audio
Input: Built-in Microphone
Output: Built-in Audio
Sample Rate: 44100 Hz
Buffer Size: 256 samples
```

### 配置 3: 穩定性優先
```
Audio Device Type: ALSA / Windows Audio
Input: Default
Output: Default
Sample Rate: 44100 Hz
Buffer Size: 512 samples
```

---

## 🎵 快速測試流程

### 5 分鐘測試計劃

1. **[1 分鐘] 開啟 Plugin**
   ```bash
   ./run_gain_plugin.sh
   ```

2. **[2 分鐘] 配置音訊**
   - Options → Audio/MIDI Settings
   - Input: Built-in Microphone
   - Output: Built-in Audio
   - Sample Rate: 44100 Hz
   - Buffer Size: 256 samples
   - 點擊 "OK" 或 "Apply"

3. **[1 分鐘] 測試麥克風**
   - 對著麥克風說話
   - 應該能聽到自己的聲音（可能有延遲）

4. **[1 分鐘] 調整 Gain**
   - 旋轉 Gain 旋鈕從 0.0 → 2.0
   - Gain = 0.0: 無聲
   - Gain = 1.0: 正常音量
   - Gain = 2.0: 放大 2 倍

5. **[30 秒] 確認效果**
   - 觀察數值顯示變化
   - 聽到音量變化即成功！

---

## 🎓 理解音訊流程

```
[麥克風/音訊輸入]
        ↓
   [Input Device]
        ↓
   [Simple Gain Plugin]
    (增益處理: × Gain)
        ↓
   [Output Device]
        ↓
  [喇叭/耳機輸出]
```

當您調整 Gain 旋鈕時：
- **0.0**: 輸出 = 輸入 × 0.0 = 靜音
- **0.5**: 輸出 = 輸入 × 0.5 = 減半音量
- **1.0**: 輸出 = 輸入 × 1.0 = 原始音量
- **2.0**: 輸出 = 輸入 × 2.0 = 雙倍音量

---

## 📞 還是沒聲音？

如果按照以上步驟仍無法聽到聲音，請提供以下資訊：

1. **系統資訊**:
   ```bash
   uname -a
   ```

2. **音訊設備列表**:
   ```bash
   aplay -l
   arecord -l
   ```

3. **Plugin 輸出訊息**:
   - 執行時的任何錯誤訊息
   - 音訊設定視窗的截圖

我會幫您進一步診斷問題！

---

**祝您使用順利！🎵**
