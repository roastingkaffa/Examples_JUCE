---
title: Software Modules Index
owners: [fw-team]
last_updated: 2025-10-28
---
# Modules Index
列出各模組的說明檔，依照資料夾或元件分類。

## Core Modules

### Application & Lifecycle
- [Application Lifecycle Module](./application-lifecycle-module.md) - Application initialization, event loop, and shutdown management

### Event Handling
- [Timer Module](./timer-module.md) - Periodic callback functionality using JUCE Timer class

## GUI Components

### Basic Widgets
- [TextButton Module](./textbutton-module.md) - Button component with event handling and repeat-click functionality
- [Slider Module](./slider-module.md) - Continuous value input with real-time callbacks and data binding

### Hello World Examples
- [HelloWorld JUCE Module](./helloworld-juce-module.md) - Comprehensive GUI example with vector graphics, custom painting, and responsive layout

## Module Documentation Format

Each module document contains:
- **Purpose**: Module functionality overview
- **Files**: Source file locations and line counts
- **Public APIs**: Key classes, methods, and interfaces
- **Init/Deinit**: Initialization and cleanup sequences
- **Threads/IRQs**: Threading model and concurrency details
- **Dependencies**: Internal and external dependencies with data flow
- **Performance/Memory**: Resource usage characteristics
- **Common Pitfalls**: Known issues and solutions
- **Implementation Notes**: Design patterns and code examples
- **Related Modules**: Cross-references to related documentation
- **Testing Considerations**: Validation and testing guidelines

## Quick Reference

| Module | Primary Purpose | Key JUCE Classes | Complexity |
|--------|----------------|------------------|------------|
| Application Lifecycle | App startup/shutdown | JUCEApplication | ⭐ Low |
| Timer | Periodic tasks | Timer | ⭐ Low |
| TextButton | User input (clicks) | TextButton | ⭐ Low |
| Slider | Value input (continuous) | Slider, ProgressBar | ⭐⭐ Medium |
| HelloWorld JUCE | GUI architecture demo | Component, Graphics, Path | ⭐⭐ Medium |

## Module Categories

### By JUCE Module
- **juce_events**: Application Lifecycle, Timer
- **juce_gui_basics**: TextButton, Slider, HelloWorld JUCE

### By Functionality
- **Input Components**: TextButton, Slider
- **Display Components**: HelloWorld JUCE (Label, Path rendering)
- **Core Infrastructure**: Application Lifecycle, Timer

## Adding New Module Documentation

Use this template structure for new modules:
```markdown
---
title: Module Name
owners: [fw-team]
last_updated: YYYY-MM-DD
tags: [relevant, tags, here]
---

# Module Name

## Purpose
## Files
## Public APIs
## Initialization/Deinitialization
## Threads/IRQs
## Dependencies
## Performance/Memory
## Common Pitfalls
## Implementation Notes
## Related Modules
## JUCE API References
## Testing Considerations
```
