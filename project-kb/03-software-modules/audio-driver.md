---
title: Audio Driver
owners: [fw-team]
last_updated: 2025-10-28
tags: [audio, driver, juce/rtos]
status: draft
---

# Module: Audio Driver
**Files**: `Drivers/audio_driver.c`, `Drivers/audio_driver.h`  
**Purpose**: 提供 I2S/PCM 音訊輸入輸出、緩衝區管理與中斷服務。  
**Runtime Context**: IRQ + 低延遲循環；禁止在 ISR 內做阻塞或 malloc。

## Public APIs
- `int audio_init(const audio_cfg_t* cfg);`
- `int audio_start(void);`
- `int audio_stop(void);`
- `int audio_set_volume(int db);`
- `int audio_deinit(void);`

## Initialization / De-init
- `audio_init()`：設定 clock、pinmux、DMA page；失敗要回傳負值並記錄 log
- `audio_deinit()`：停止 DMA、關閉中斷、釋放資源

## Threads / IRQs
- IRQ: I2S RX/TX 完成 → 將 page 推入 lock-free queue  
- Worker (optional): 從 queue 取出資料做處理（禁止阻塞 GUI/Message thread）

## Buffers & Memory
- DMA pages: `N * page_size`；**確保 page_size 與硬體對齊**  
- **禁止** page 在中斷內做動態配置；建議預先配置並重複使用

## Dependencies
- Clock/PLL、DMA、GPIO/Pinmux、AMP HAL（若有）、Mixer

## JUCE 對接（若為 Plugin）
- `prepareToPlay()`：配置 buffer、清零  
- `processBlock()`：禁止鎖／阻塞 I/O；用 lock-free 結構交換資料  
- `releaseResources()`：釋放 buffer、停止背景工作

## Performance
- 量測點：ISR 佔用、XRUN 次數、end-to-end latency
- 目標：XRUN=0、latency < X ms（依產品規格）

## Common Pitfalls
- 在 ISR 內呼叫非可重入 API  
- Message thread 阻塞導致 GUI 卡頓  
- DMA page size 與處理框大小不一致

## Test Checklist
- [ ] 初始化失敗路徑覆蓋（clock/pinmux/DMA 任一失敗）
- [ ] 48k/96k/192k 取樣率切換
- [ ] 極限負載下不掉 buffer / 不爆音
- [ ] 音量／靜音 邏輯正確

## Change Log
- 2025-10-28: 初版模板建立（Teo）

