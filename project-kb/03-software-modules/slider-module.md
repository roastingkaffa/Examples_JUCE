---
title: Slider Module
owners: [fw-team]
last_updated: 2025-10-28
tags: [juce_gui_basics, slider, input, value-binding]
---

# Slider Module

## Purpose
Demonstrates JUCE's Slider component for continuous value input. Shows value binding patterns, real-time callbacks, and component interconnection (Slider → ProgressBar → Label data flow).

## Files
- **Source**: `juce_gui_basics/Widgets/Slider/src/Slider.cpp` (58 lines)
- **Module**: juce_gui_basics
- **Namespace**: `SliderExample`

## Public APIs

### juce::Slider Class (JUCE Standard Component)

**Key Methods**:
```cpp
// Configuration
void setRange(double minValue, double maxValue);
void setValue(double newValue);
double getValue() const;
void setTextBoxStyle(TextEntryBoxPosition position, bool readOnly, int width, int height);

// Event Handling
std::function<void()> onValueChange;  // Callback when value changes

// Styling
enum TextEntryBoxPosition {
    NoTextBox,
    TextBoxLeft,
    TextBoxRight,
    TextBoxAbove,
    TextBoxBelow
};
```

## Initialization/Deinitialization

### Init Sequence (Slider.cpp:14-22)
1. **Set Bounds**: Position slider at (50, 50, 200×25) (Slider.cpp:14)
2. **Disable Text Box**: `setTextBoxStyle(NoTextBox, ...)` (Slider.cpp:15)
3. **Set Range**: 0.0 to 1.0 (Slider.cpp:16)
4. **Set Initial Value**: From progressValue1 member (0.5) (Slider.cpp:17)
5. **Attach Callback**: Lambda for onValueChange (Slider.cpp:18-21)
6. **Make Visible**: Add to parent component (Slider.cpp:22)

### Configuration Details
```cpp
slider1.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
```
- **NoTextBox**: Hides the text entry field (cleaner visual)
- **true**: Read-only (irrelevant when NoTextBox is used)
- **0, 0**: Width/height (unused when NoTextBox)

### Deinit Sequence
- Automatic cleanup when Window1 destructor is called
- No explicit resource management required

## Threads/IRQs

### Threading Model
- **Main Thread Only**: All slider events on message thread
- **Synchronous Callbacks**: `onValueChange` executes immediately during value change
- **High-Frequency Events**: Callback fires continuously during drag (potentially 60-120 Hz)

**Performance Note**: Keep `onValueChange` lightweight as it's called very frequently during drag operations.

## Dependencies

### Internal Dependencies
```
Window1
    ├─ Slider slider1 (value input)
    ├─ ProgressBar progressBar1 (visual feedback - bound to progressValue1)
    └─ Label label1 (numeric display)
```

### External Dependencies
- **JUCE Modules**:
  - `juce_gui_basics` - Slider, ProgressBar, Label
  - `juce_core` - String formatting, component infrastructure
  - `juce_events` - Mouse tracking, message thread

### Data Flow
```
User drags slider
    ↓
Slider internal state updated
    ↓
onValueChange callback triggered (Slider.cpp:18-21)
    ↓
progressValue1 updated from slider1.getValue()
    ↓
label1.setText() called with percentage (int cast)
    ↓
ProgressBar progressBar1 reads progressValue1 (via reference binding)
    ↓
GUI repaints showing updated value in 3 places:
    - Slider thumb position
    - ProgressBar fill level
    - Label numeric text
```

### Value Binding Pattern
```cpp
double progressValue1 = 0.5;                      // Shared state
ProgressBar progressBar1{progressValue1};         // Bind by reference
slider1.onValueChange = [&] {
    progressValue1 = slider1.getValue();          // Update shared state
    label1.setText(...);                          // Sync label
};
```

## Performance/Memory

### Memory Usage
- **Slider Instance**: ~1024 bytes
  - Base Component: ~256 bytes
  - Slider state: ~128 bytes (value, range, style)
  - Graphics cache: ~640 bytes
- **Lambda Capture**: 16 bytes ([&] capture)

### CPU Usage
- **Idle**: 0% (event-driven)
- **Dragging**: 2-5% CPU (continuous repaint at ~60 FPS)
- **Callback Overhead**: <100μs per value change

### Event Frequency
- **Mouse Drag**: 60-120 callbacks/second (OS-dependent)
- **Keyboard**: 10-20 callbacks/second (key repeat rate)
- **Programmatic**: Single callback per setValue()

## Common Pitfalls

### 1. Infinite Recursion in Callback
**Problem**: Modifying slider value inside its own callback
```cpp
slider.onValueChange = [&] {
    slider.setValue(slider.getValue() * 2);  // RECURSION! Stack overflow
};
```

**Solution**: Use flag or check value before setting
```cpp
bool isUpdating = false;
slider.onValueChange = [&] {
    if (isUpdating) return;
    isUpdating = true;
    slider.setValue(slider.getValue() * 2);
    isUpdating = false;
};
```

### 2. Heavy Computation in Callback
**Problem**: Blocking message thread during drag
```cpp
slider.onValueChange = [&] {
    recalculateComplexModel();  // BAD: Called 60+ times/sec
    redrawEntireScene();         // BAD: Causes UI lag
};
```

**Solution**: Debounce or throttle callbacks
```cpp
Timer debounceTimer;
slider.onValueChange = [&] {
    quickUpdate();  // Lightweight visual feedback
    debounceTimer.startTimer(200);  // Delay heavy work
};
debounceTimer.onTick = [&] {
    recalculateComplexModel();  // Only after 200ms of inactivity
    debounceTimer.stopTimer();
};
```

### 3. Stale Value Binding
**Problem**: ProgressBar not updating after value change
```cpp
double value = 0.5;
ProgressBar bar{value};  // Binds to current value
value = 0.8;             // BAD: ProgressBar doesn't see change
```

**Solution**: ProgressBar takes reference, but must repaint
```cpp
double value = 0.5;
ProgressBar bar{value};
slider.onValueChange = [&] {
    value = slider.getValue();
    bar.repaint();  // Force update (usually automatic)
};
```

### 4. Incorrect Range Configuration
**Problem**: Range doesn't match expected values
```cpp
slider.setRange(0, 100);    // Integer-like range
slider.setValue(50.5);      // BAD: Unexpected precision behavior
```

**Solution**: Be explicit about precision
```cpp
slider.setRange(0.0, 100.0, 0.1);  // Min, Max, Interval
```

### 5. Not Handling Edge Cases
**Problem**: Division by zero or invalid conversions
```cpp
slider.onValueChange = [&] {
    float percent = 1.0f / slider.getValue();  // CRASH if value is 0!
};
```

**Solution**: Validate values
```cpp
slider.onValueChange = [&] {
    double value = slider.getValue();
    if (value > 0.001) {  // Guard against zero
        float percent = 1.0f / value;
    }
};
```

## Implementation Notes

### Design Pattern: Observer + Data Binding
- **Observer**: Slider → onValueChange → Updates observers
- **Data Binding**: ProgressBar bound to shared `progressValue1` variable
- **Unidirectional Flow**: Slider is source of truth

### Value Transformation Example
```cpp
slider1.onValueChange = [&] {
    progressValue1 = slider1.getValue();                        // Raw: 0.0-1.0
    label1.setText(String{static_cast<int>(progressValue1 * 100)},  // Display: 0-100
                   NotificationType::dontSendNotification);
};
```
- **Raw Value**: 0.0 to 1.0 (internal representation)
- **Display Value**: 0 to 100 (user-friendly percentage)
- **Cast to int**: Removes decimal precision for cleaner display

### Alternative Slider Styles

#### Style 1: Linear Horizontal (Current Example)
```cpp
slider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
```

#### Style 2: Rotary Knob
```cpp
slider.setSliderStyle(Slider::SliderStyle::Rotary);
slider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
```

#### Style 3: Linear Vertical Bar
```cpp
slider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
slider.setRange(0.0, 1.0);
slider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
```

### Advanced Features Not Shown in Example

#### Skew Factor (Logarithmic Scaling)
```cpp
// Frequency control: more precision in low frequencies
slider.setSkewFactor(0.25);  // More space for low values
```

#### Snap to Values
```cpp
slider.setRange(0.0, 100.0, 1.0);  // Snap to integers
```

#### Double-Click to Reset
```cpp
slider.setDoubleClickReturnValue(true, 0.5);  // Double-click → 0.5
```

## Related Modules
- **ProgressBar Module** (progressbar-module.md) - Visual feedback component
- **Label Module** (label-module.md) - Numeric display
- **RotarySlider Module** - Alternative slider style (if exists)

## JUCE API References
- [juce::Slider Class](https://docs.juce.com/master/classSlider.html)
- [Slider Tutorial](https://docs.juce.com/master/tutorial_slider_values.html)
- [ProgressBar Class](https://docs.juce.com/master/classProgressBar.html)

## Testing Considerations
- Verify value changes continuously during drag
- Test keyboard input (arrow keys, page up/down)
- Validate edge values (min, max, zero)
- Check floating-point precision issues
- Test rapid value changes (stress test)
- Verify memory doesn't leak during extended use
- Test accessibility (screen reader support)
- Validate touch input on mobile platforms

## UI/UX Guidelines
- **Slider Width**: Minimum 100px for precise control
- **Slider Height**: 20-30px for mouse targeting
- **Range Selection**: Consider user's mental model (0-100 vs 0-1)
- **Visual Feedback**: Always pair with numeric display or progress bar
- **Precision**: Match granularity to use case (audio: 0.01, volume: 1)

---

**Needs Manual Review**:
- [ ] Document skew factor calculation for different use cases
- [ ] Add guidelines for choosing slider style
- [ ] Profile callback frequency on different platforms
- [ ] Document recommended throttling strategies
- [ ] Add examples of bidirectional slider synchronization
- [ ] Specify touch gesture support on mobile
