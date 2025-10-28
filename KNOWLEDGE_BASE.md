# JUCE Examples Project - Knowledge Base

## 專案概述 (Project Overview)

### 專案名稱
**JUCE Examples** - JUCE GUI 框架程式化範例集合

### 專案目的
此專案展示如何純粹透過 C++17 程式碼使用 [JUCE](https://juce.com) 元件，不依賴視覺化編輯器。提供給開發者學習 JUCE 框架各種元件的實作範例。

### 技術棧
- **語言**: C++17
- **框架**: JUCE 7.0.12 或更高版本
- **建置系統**: CMake 3.22 或更高版本
- **支援平台**: Windows, macOS, Linux

---

## 專案架構 (Project Architecture)

### 目錄結構
```
Examples_JUCE/
├── juce_events/              # 事件處理範例
│   └── Timers/              # 計時器範例
│       └── Timer/           # Timer 類別範例
├── juce_gui_basics/         # GUI 基礎元件範例
│   ├── HelloWorlds/         # Hello World 範例集
│   ├── Applications/        # 應用程式範例
│   ├── Buttons/             # 按鈕元件範例
│   ├── Widgets/             # 小工具範例
│   └── Others/              # 其他範例
├── build/                   # CMake 建置輸出目錄
├── .claude/                 # Claude Code 配置
│   ├── commands/           # 自訂斜線命令
│   └── agents/             # 代理配置
└── CMakeLists.txt          # 根 CMake 配置檔
```

---

## 模組詳細說明 (Module Details)

### 1. juce_events 模組

#### 1.1 Timer (計時器)
**檔案位置**: `juce_events/Timers/Timer/src/Timer.cpp`

**核心類別**:
- `CounterTimer`: 繼承自 `juce::Timer`
  - **用途**: 自訂計時器類別，使用 function callback
  - **關鍵成員**:
    - `std::function<void()> onTick`: 計時器觸發時的回調函式
    - `void timerCallback() override`: 實作計時器回調

**功能展示**:
- 啟動/停止計時器
- 100ms 週期更新
- 動態文字顯示 (計數器累加)
- 按鈕狀態切換 ("Start"/"Stop")

**JUCE API 使用**:
- `Timer::startTimer(int milliseconds)`
- `Timer::stopTimer()`
- `Timer::isTimerRunning()`
- `Timer::timerCallback()` (override)

---

### 2. juce_gui_basics 模組

#### 2.1 Hello Worlds 子模組

##### HelloWorldJUCE
**檔案位置**: `juce_gui_basics/HelloWorlds/HelloWorldJUCE/src/HelloWorldJUCE.cpp`

**核心類別**:
1. **HelloWorldDemo** (繼承 `Component`)
   - **UI 元件**:
     - `Label helloWorldLabel`: 顯示 "Hello World!" 文字
     - `TextButton quitButton`: 退出按鈕
     - `Path internalPath`: 向量圖形路徑 (語音氣泡形狀)

   - **關鍵方法**:
     - `paint(Graphics& g)`: 繪製背景和向量圖形
     - `resized()`: 處理元件大小調整和路徑更新

2. **Window1** (繼承 `DocumentWindow`)
   - 主視窗容器
   - 管理 HelloWorldDemo 元件生命週期

3. **Application** (繼承 `JUCEApplication`)
   - 應用程式進入點
   - 處理初始化和關閉

**視覺特效**:
- 淡藍色背景 (`0xffc1d0ff`)
- 白色語音氣泡路徑
- 灰色描邊 (5.2px)
- 40pt 粗體置中文字

**尺寸**: 600x300 px

---

##### HelloWorldPaint
**檔案位置**: `juce_gui_basics/HelloWorlds/HelloWorldPaint/src/HelloWorldPaint.cpp`

**特點**: 純粹使用 `paint()` 方法繪製 "Hello, World!" 文字，不使用 Label 元件

---

##### HelloWorldLabel
**檔案位置**: `juce_gui_basics/HelloWorlds/HelloWorldLabel/src/HelloWorldLabel.cpp`

**特點**: 使用 `Label` 元件顯示 "Hello, World!" 文字

---

##### HelloWorldAlertWindow / HelloWorldAlertWindow2
**特點**: 使用 `AlertWindow` 對話框顯示訊息

---

##### HelloWorldNativeMessageBox / HelloWorldNativeMessageBox2
**特點**: 使用原生訊息框 (`NativeMessageBox`) 顯示訊息

---

##### HelloWorldSay
**檔案位置**: `juce_gui_basics/HelloWorlds/HelloWorldSay/src/HelloWorldSay.cpp`

**特點**: 使用 `TextButton` 搭配語音功能

---

#### 2.2 Buttons 子模組

##### TextButton
**檔案位置**: `juce_gui_basics/Buttons/TextButton/src/TextButton.cpp`

**功能展示**:
- 兩個 TextButton 實例
- 點擊計數器功能
- 按鈕重複速度設定 (`setRepeatSpeed(300, 100)`)
- Lambda 回調處理點擊事件

**UI 佈局**:
- Button 1: 75x25 px at (50, 50)
- Button 2: 200x75 px at (50, 100) - 支援長按重複觸發
- 兩個 Label 顯示各自的點擊次數

**關鍵程式碼模式**:
```cpp
button1.onClick = [&] {
    label1.setText("Button 1 clicked " + String{++button1Clicked} + " times",
                   NotificationType::dontSendNotification);
};
```

---

##### CheckBox
**檔案位置**: `juce_gui_basics/Buttons/CheckBox/src/CheckBox.cpp`

**特點**: 使用 `ToggleButton` 實作核取框

---

##### RadioButton
**檔案位置**: `juce_gui_basics/Buttons/RadioButton/src/RadioButton.cpp`

**特點**: 使用 `ToggleButton` 實作單選按鈕群組

---

##### ToggleButton
**檔案位置**: `juce_gui_basics/Buttons/ToggleButton/src/ToggleButton.cpp`

**特點**: 展示切換按鈕功能

---

##### ArrowButton
**檔案位置**: `juce_gui_basics/Buttons/ArrowButton/src/ArrowButton.cpp`

**特點**: 方向箭頭按鈕 (`ArrowButton`)

---

##### HyperlinkButton
**檔案位置**: `juce_gui_basics/Buttons/HyperlinkButton/src/HyperlinkButton.cpp`

**特點**: 超連結按鈕功能

---

#### 2.3 Widgets 子模組

##### Slider
**檔案位置**: `juce_gui_basics/Widgets/Slider/src/Slider.cpp`

**功能展示**:
- 水平滑桿控制進度條
- 範圍: 0.0 ~ 1.0
- 即時百分比顯示
- 無文字輸入框模式

**元件連動**:
```
Slider (slider1)
    ↓ onValueChange
ProgressBar (progressBar1) + Label (label1)
```

**關鍵配置**:
```cpp
slider1.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
slider1.setRange(.0, 1.0);
slider1.onValueChange = [&] {
    progressValue1 = slider1.getValue();
    label1.setText(String{static_cast<int>(progressValue1 * 100)},
                   NotificationType::dontSendNotification);
};
```

---

##### ProgressBar
**檔案位置**: `juce_gui_basics/Widgets/ProgressBar/src/ProgressBar.cpp`

**特點**: 進度條元件，可連動數值變數

---

##### Label
**檔案位置**: `juce_gui_basics/Widgets/Label/src/Label.cpp`

**特點**: 文字標籤元件

---

##### ComboBox
**檔案位置**: `juce_gui_basics/Widgets/ComboBox/src/ComboBox.cpp`

**特點**: 下拉式選單

---

##### ListBox
**檔案位置**: `juce_gui_basics/Widgets/ListBox/src/ListBox.cpp`

**特點**: 列表框元件

---

##### ImageComponent
**檔案位置**: `juce_gui_basics/Widgets/ImageComponent/src/ImageComponent.cpp`

**特點**: 圖片顯示元件

---

#### 2.4 Applications 子模組

##### Application
**檔案位置**: `juce_gui_basics/Applications/Application/src/Application.cpp`

**特點**: 基本 JUCE 應用程式範例

---

##### ApplicationWithProjectInfo
**檔案位置**: `juce_gui_basics/Applications/ApplicationWithProjectInfo/src/ApplicationWithProjectInfo.cpp`

**特點**: 展示如何使用 `ProjectInfo` 命名空間

---

##### ApplicationUsingNativeTitleBar
**檔案位置**: `juce_gui_basics/Applications/ApplicationUsingNativeTitleBar/src/ApplicationUsingNativeTitleBar.cpp`

**特點**: 使用原生標題列的應用程式視窗

---

##### ApplicationAndException
**檔案位置**: `juce_gui_basics/Applications/ApplicationAndException/src/ApplicationAndException.cpp`

**特點**: 展示例外處理

---

#### 2.5 Others 子模組

##### DisplayInformations
**檔案位置**: `juce_gui_basics/Others/DisplayInformations/src/DisplayInformations.cpp`

**特點**: 顯示系統顯示器資訊

---

##### ProgressBarCircular
**檔案位置**: `juce_gui_basics/Others/ProgressBarCircular/src/ProgressBarCircular.cpp`

**特點**: 圓形進度條

---

## 程式碼模式與最佳實踐 (Code Patterns & Best Practices)

### 1. 標準應用程式結構
所有範例都遵循三層架構：

```cpp
namespace ExampleName {
  // 1. 主元件 (UI邏輯)
  class MainComponent : public Component { ... }

  // 2. 視窗容器
  class Window1 : public DocumentWindow {
    void closeButtonPressed() override {
      JUCEApplication::quit();
    }
  };

  // 3. 應用程式進入點
  class Application : public JUCEApplication {
    const String getApplicationName() override {
      return ProjectInfo::projectName;
    }
    const String getApplicationVersion() override {
      return ProjectInfo::versionString;
    }
    void initialise(const String& commandLine) override {
      mainWindow.setVisible(true);
    }
    void shutdown() override {}
  };
}

START_JUCE_APPLICATION(ExampleName::Application)
```

### 2. 元件初始化模式

#### 方法 A: DocumentWindow 建構子中直接配置
```cpp
Window1() : DocumentWindow {...} {
  button1.setBounds(50, 50, 75, 25);
  button1.onClick = [&] { /* handler */ };
  mainComponent.addAndMakeVisible(&button1);
}
```

#### 方法 B: Component 類別中使用 addAndMakeVisible
```cpp
HelloWorldDemo() {
  addAndMakeVisible(helloWorldLabel);
  helloWorldLabel.setFont({40.00f, Font::bold});
  helloWorldLabel.setJustificationType(Justification::centred);
}
```

### 3. Lambda 回調模式
所有事件處理都使用 lambda 函式：

```cpp
button.onClick = [&] {
  // 使用 capture-by-reference [&] 存取類別成員
  label.setText("Updated", NotificationType::dontSendNotification);
};
```

### 4. 色彩定義
```cpp
// 十六進位色彩
Colour{0xffc1d0ff}  // ARGB 格式

// 預定義色彩
Colours::black
Colours::white
Colours::dodgerblue
```

### 5. 字型設定
```cpp
// 簡單設定
Font{40.0f, Font::bold}

// 複合樣式
Font{68, Font::FontStyleFlags::bold | Font::FontStyleFlags::italic}
```

---

## CMake 建置系統 (Build System)

### 根層級 CMakeLists.txt
**路徑**: `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.22)
project(JUCEExamples)

# 尋找 JUCE 套件
if (NOT JUCE_FOUND)
  find_package(JUCE CONFIG REQUIRED)
endif()

# 啟用資料夾組織
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 子專案
add_subdirectory(juce_events)
add_subdirectory(juce_gui_basics)
```

### 子專案結構
每個範例都有獨立的 `CMakeLists.txt`，遵循標準 JUCE 專案配置。

---

## 建置與執行指南 (Build & Run Guide)

### Windows
```shell
mkdir build && cd build
cmake ..
start JUCEExamples.sln
# Ctrl+F5 建置並執行
```

### macOS
```shell
mkdir build && cd build
cmake .. -G "Xcode"
open ./JUCEExamples.xcodeproj
# Cmd+R 建置並執行
```

### Linux (直接建置)
```shell
mkdir build && cd build
cmake ..
cmake --build . --config Debug
./AnyProject
```

### Linux (Code::Blocks)
```shell
mkdir build && cd build
cmake .. -G "CodeBlocks - Unix Makefiles"
xdg-open ./JUCEExamples.cbp > /dev/null 2>&1
# F10 建置並執行
```

---

## JUCE API 參考快速索引 (Quick API Reference)

### 核心類別

| 類別 | 用途 | 文件連結 |
|------|------|----------|
| `JUCEApplication` | 應用程式生命週期管理 | [docs](https://docs.juce.com/master/classJUCEApplication.html) |
| `Component` | UI 元件基礎類別 | [docs](https://docs.juce.com/master/classComponent.html) |
| `DocumentWindow` | 文件視窗容器 | [docs](https://docs.juce.com/master/classDocumentWindow.html) |
| `Graphics` | 2D 繪圖上下文 | [docs](https://docs.juce.com/master/classGraphics.html) |
| `Timer` | 計時器基礎類別 | [docs](https://docs.juce.com/master/classTimer.html) |

### 按鈕元件

| 類別 | 用途 |
|------|------|
| `TextButton` | 標準文字按鈕 |
| `ToggleButton` | 切換/核取方塊按鈕 |
| `ArrowButton` | 方向箭頭按鈕 |
| `HyperlinkButton` | 超連結按鈕 |

### 小工具

| 類別 | 用途 |
|------|------|
| `Label` | 文字標籤 |
| `Slider` | 滑桿控制項 |
| `ComboBox` | 下拉選單 |
| `ListBox` | 列表框 |
| `ProgressBar` | 進度條 |
| `ImageComponent` | 圖片顯示 |

### 對話框

| 類別 | 用途 |
|------|------|
| `AlertWindow` | 標準警告對話框 |
| `NativeMessageBox` | 原生訊息框 |

---

## 常見設計模式 (Common Design Patterns)

### 1. 事件處理 (Event Handling)
```cpp
// 按鈕點擊
button.onClick = [&] { /* handler */ };

// 滑桿值改變
slider.onValueChange = [&] { /* handler */ };

// 計時器回調
void timerCallback() override { /* handler */ }
```

### 2. 元件生命週期
```cpp
Constructor → resized() → paint() → Destructor
```

### 3. 父子元件關係
```cpp
// 加入子元件並設為可見
addAndMakeVisible(childComponent);

// 設定子元件邊界
childComponent.setBounds(x, y, width, height);
```

### 4. 繪圖流程
```cpp
void paint(Graphics& g) override {
  g.fillAll(backgroundColor);      // 1. 填滿背景
  g.setColour(foregroundColour);   // 2. 設定前景色
  g.fillRect(...);                 // 3. 繪製形狀
  g.drawText(...);                 // 4. 繪製文字
}
```

---

## 專案統計 (Project Statistics)

- **總範例數**: 27 個
- **C++ 原始檔**: 27 個 .cpp 檔案
- **CMakeLists 檔案**: 35 個
- **README 文件**: 32 個
- **支援的 JUCE 模組**: 2 個 (juce_events, juce_gui_basics)
- **程式碼標準**: C++17
- **最低 CMake 版本**: 3.22
- **最低 JUCE 版本**: 7.0.12

---

## 學習路徑建議 (Recommended Learning Path)

### 初學者
1. **HelloWorldLabel** - 最簡單的 Label 元件
2. **HelloWorldJUCE** - 組合多種元件
3. **TextButton** - 事件處理入門
4. **Application** - 應用程式架構

### 中級
5. **Slider** - 元件連動與資料綁定
6. **Timer** - 計時器與週期性任務
7. **ComboBox** - 選單與選項處理
8. **ListBox** - 列表與資料模型

### 進階
9. **ProgressBarCircular** - 自訂繪圖
10. **DisplayInformations** - 系統資訊存取
11. **ApplicationAndException** - 錯誤處理

---

## 故障排除 (Troubleshooting)

### 常見問題

#### 1. JUCE 找不到
```
CMake Error: Could not find a package configuration file provided by "JUCE"
```
**解決方法**: 安裝 JUCE 並設定 `CMAKE_PREFIX_PATH` 或使用 `find_package(JUCE CONFIG REQUIRED)`

#### 2. 編譯錯誤: C++17 特性
**解決方法**: 確保編譯器支援 C++17 (GCC 7+, Clang 5+, MSVC 2017+)

#### 3. 連結錯誤
**解決方法**: 確認所有 JUCE 模組相依性都已正確連結

---

## 擴充與客製化 (Extension & Customization)

### 新增範例的步驟
1. 在對應模組目錄下建立新資料夾
2. 建立 `src/` 子目錄並放入 `.cpp` 檔案
3. 建立 `CMakeLists.txt` 配置建置
4. 建立 `README.md` 說明範例用途
5. 更新父層級的 `CMakeLists.txt` (加入 `add_subdirectory`)
6. 更新父層級的 `README.md` (加入文件連結)

### 客製化範例
所有範例都可作為模板：
- 複製現有範例目錄
- 修改命名空間名稱
- 調整 UI 佈局和邏輯
- 更新 CMakeLists.txt 中的專案名稱

---

## 相關資源 (Related Resources)

### 官方文件
- [JUCE 官網](https://juce.com)
- [JUCE API 文件](https://docs.juce.com/master/)
- [JUCE 教學](https://docs.juce.com/master/tutorial_getting_started_windows.html)

### 專案連結
- [GitHub Repository](https://github.com/gammasoft71/Examples_JUCE)

### 社群
- [JUCE Forum](https://forum.juce.com/)
- [JUCE Discord](https://discord.gg/juce)

---

## 版本資訊 (Version Information)

- **專案版本**: 最新 (追蹤 master 分支)
- **最後更新**: 2025-10-27
- **授權**: (請參考專案根目錄的授權檔案)

---

## 貢獻指南 (Contributing)

如需貢獻新範例或修復問題：
1. Fork 專案儲存庫
2. 建立特性分支
3. 遵循現有程式碼風格
4. 為新範例建立完整文件
5. 提交 Pull Request

---

**文件產生時間**: 2025-10-27
**產生工具**: Claude Code (Anthropic)
**文件版本**: 1.0
