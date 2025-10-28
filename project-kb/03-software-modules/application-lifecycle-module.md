---
title: Application Lifecycle Module
owners: [fw-team]
last_updated: 2025-10-28
tags: [juce_events, application, lifecycle, entry-point]
---

# Application Lifecycle Module

## Purpose
Defines the standard JUCE application lifecycle pattern used across all examples. Manages application initialization, main window creation, event loop execution, and graceful shutdown.

## Files
- **Pattern Used In**: All example files (27 .cpp files)
- **Representative Example**: `juce_gui_basics/Applications/Application/src/Application.cpp`
- **Module**: juce_events
- **Common Namespace Pattern**: `<ExampleName>::Application`

## Public APIs

### JUCEApplication Base Class (JUCE Framework)

```cpp
class Application : public JUCEApplication {
protected:
    // Mandatory overrides
    const String getApplicationName() override;
    const String getApplicationVersion() override;
    void initialise(const String& commandLine) override;
    void shutdown() override;

    // Optional overrides
    void systemRequestedQuit() override;
    void anotherInstanceStarted(const String& commandLine) override;
    void suspended() override;
    void resumed() override;
    void unhandledException(const std::exception*, const String&, int) override;

private:
    // Application state
    Window1 mainWindow;  // Main window instance
};
```

**Entry Point Macro**:
```cpp
START_JUCE_APPLICATION(ExampleName::Application)
```

## Initialization/Deinitialization

### Startup Sequence

#### Phase 1: Pre-Main Initialization
```
OS Launches Executable
    ↓
START_JUCE_APPLICATION macro expands
    ↓
JUCE creates JUCEApplicationBase instance
    ↓
Platform-specific initialization (Windows: WinMain, macOS: NSApplicationMain, Linux: main)
```

#### Phase 2: initialise() Callback
```cpp
void initialise(const String& commandLine) override {
    mainWindow.setVisible(true);  // Show main window
}
```
Called after JUCE framework initialization completes.

**Typical Actions**:
1. Parse command-line arguments
2. Load configuration/preferences
3. Create and show main window
4. Start background services (if needed)

#### Phase 3: Message Loop
```
while (application is running) {
    Process OS events (mouse, keyboard, timers, etc.)
    Dispatch to JUCE components
    Handle custom messages
}
```

### Shutdown Sequence

#### Phase 1: Quit Request
**Trigger Sources**:
- User closes last window (closeButtonPressed() → quit())
- User selects "Quit" menu item
- OS shutdown signal (Ctrl+C, logout, etc.)

```cpp
void closeButtonPressed() override {
    JUCEApplication::quit();  // Request termination
}
```

#### Phase 2: shutdown() Callback
```cpp
void shutdown() override {
    // Clean up resources
    // Save application state
    // Close network connections
    // Stop background threads
}
```

**IMPORTANT**: After `shutdown()` returns, application terminates immediately.

#### Phase 3: Cleanup
```
shutdown() completes
    ↓
Component destructors called (mainWindow, etc.)
    ↓
JUCE framework cleanup
    ↓
Process exits (return code 0)
```

## Threads/IRQs

### Threading Model
- **Main Thread**: Runs message loop, handles all GUI events
- **Background Threads**: Must be explicitly created (Thread class)
- **MessageManager**: Central event dispatcher (singleton)

**Thread Safety Rules**:
1. **GUI operations**: Main thread ONLY
2. **Component access**: Main thread ONLY (unless using MessageManagerLock)
3. **Heavy computation**: Offload to background threads

### Message Loop Execution
```cpp
// Internal JUCE pseudocode
int main(int argc, char* argv[]) {
    Application app;
    app.initialise(commandLine);

    while (!quitRequested) {
        MessageManager::dispatchNextMessage();  // Blocks until event
    }

    app.shutdown();
    return 0;
}
```

## Dependencies

### Internal Dependencies
```
Application (JUCEApplication)
    ├─ MessageManager (event loop)
    ├─ Window1 (DocumentWindow)
    │   └─ MainComponent (content)
    └─ System Platform Layer
        ├─ Windows: WinMain
        ├─ macOS: NSApplicationMain
        └─ Linux: main + X11/Wayland
```

### External Dependencies
- **JUCE Modules**:
  - `juce_events` - JUCEApplication, MessageManager
  - `juce_gui_basics` - DocumentWindow, Component
  - `juce_core` - String, memory management

### Platform Dependencies
| Platform | Native Application Type |
|----------|-------------------------|
| Windows  | Win32 application (WinMain) |
| macOS    | Cocoa application (NSApplication) |
| Linux    | X11/Wayland application |
| iOS      | UIApplication delegate |
| Android  | Activity subclass |

## Performance/Memory

### Memory Usage
- **Application Instance**: ~1 KB
  - Virtual function table: 64 bytes
  - Member variables: ~512 bytes (mainWindow reference)
  - JUCE framework overhead: ~256 bytes

### Startup Time
- **Cold Start**: 100-500 ms (platform-dependent)
  - JUCE initialization: 50-200 ms
  - Window creation: 20-100 ms
  - First paint: 10-50 ms
- **Warm Start**: 50-200 ms (shared libraries cached)

### Event Loop Performance
- **Idle CPU**: <0.1% (blocking wait on event queue)
- **Event Processing**: 10-50 μs per event
- **Message Queue Depth**: Unlimited (heap-allocated)

## Common Pitfalls

### 1. Heavy Work in initialise()
**Problem**: Blocking main thread during startup
```cpp
void initialise(const String& commandLine) override {
    loadHugeDatabase();          // BAD: Blocks for seconds
    connectToRemoteServer();     // BAD: Network I/O on main thread
    mainWindow.setVisible(true); // Window appears late
}
```

**Solution**: Defer heavy work or use background thread
```cpp
void initialise(const String& commandLine) override {
    mainWindow.setVisible(true);  // Show window immediately

    Thread::launch([this] {
        loadHugeDatabase();       // Background thread
        MessageManager::callAsync([this] {
            updateUIAfterLoad();  // Update UI on main thread
        });
    });
}
```

### 2. Forgetting to Show Window
**Problem**: Application runs but nothing visible
```cpp
void initialise(const String& commandLine) override {
    // mainWindow created but never shown
    // User sees nothing, process runs forever
}
```

**Solution**: Always call setVisible(true)
```cpp
void initialise(const String& commandLine) override {
    mainWindow.setVisible(true);  // Essential!
}
```

### 3. Cleanup in Wrong Place
**Problem**: Cleaning up in destructor instead of shutdown()
```cpp
~Application() {
    database.close();  // BAD: May be too late
}
```

**Solution**: Use shutdown() for explicit cleanup
```cpp
void shutdown() override {
    database.close();  // Guaranteed to run before exit
}
```

### 4. Not Handling systemRequestedQuit()
**Problem**: Application ignores OS shutdown signals
```cpp
// Default implementation quits immediately
// Problem: User loses unsaved data
```

**Solution**: Override to prompt user
```cpp
void systemRequestedQuit() override {
    if (hasUnsavedChanges()) {
        AlertWindow::showYesNoCancelBox(
            MessageBoxIconType::QuestionIcon,
            "Quit?",
            "Save changes before quitting?",
            "Yes", "No", "Cancel",
            nullptr,
            ModalCallbackFunction::create([this](int result) {
                if (result == 1) {  // Yes
                    saveChanges();
                    quit();
                } else if (result == 2) {  // No
                    quit();
                }
                // Cancel: do nothing
            }));
    } else {
        quit();
    }
}
```

### 5. Multiple Windows Lifecycle Issues
**Problem**: Quitting when any window closes
```cpp
void closeButtonPressed() override {
    JUCEApplication::quit();  // BAD: Quits even if other windows open
}
```

**Solution**: Track window count
```cpp
class Application : public JUCEApplication {
    std::vector<std::unique_ptr<DocumentWindow>> windows;

    void closeWindow(DocumentWindow* window) {
        windows.erase(std::remove_if(windows.begin(), windows.end(),
            [window](auto& w) { return w.get() == window; }),
            windows.end());

        if (windows.empty()) {
            quit();  // Only quit when all windows closed
        }
    }
};
```

## Implementation Notes

### Standard Application Pattern
All JUCE examples follow this structure:

```cpp
namespace ExampleName {
    // 1. Content Component
    class MainComponent : public Component {
        // UI elements and logic
    };

    // 2. Window Container
    class Window1 : public DocumentWindow {
    public:
        Window1() : DocumentWindow{...} {
            setContentOwned(&mainComponent, true);
            setVisible(true);
        }

    protected:
        void closeButtonPressed() override {
            JUCEApplication::quit();
        }

    private:
        MainComponent mainComponent;
    };

    // 3. Application Lifecycle
    class Application : public JUCEApplication {
    protected:
        const String getApplicationName() override {
            return ProjectInfo::projectName;
        }

        const String getApplicationVersion() override {
            return ProjectInfo::versionString;
        }

        void initialise(const String& commandLine) override {
            mainWindow.setVisible(true);
        }

        void shutdown() override {
            // Cleanup
        }

    private:
        Window1 mainWindow;
    };
}

// 4. Entry Point
START_JUCE_APPLICATION(ExampleName::Application)
```

### ProjectInfo Integration
Most examples use CMake-generated ProjectInfo:

```cpp
const String getApplicationName() override {
    return ProjectInfo::projectName;  // From CMakeLists.txt
}

const String getApplicationVersion() override {
    return ProjectInfo::versionString;  // From CMakeLists.txt
}
```

### Command-Line Argument Parsing
```cpp
void initialise(const String& commandLine) override {
    StringArray args;
    args.addTokens(commandLine, true);  // Split by whitespace

    for (const auto& arg : args) {
        if (arg == "--verbose") {
            enableVerboseLogging();
        } else if (arg.startsWith("--file=")) {
            loadFile(arg.fromFirstOccurrenceOf("=", false, false));
        }
    }

    mainWindow.setVisible(true);
}
```

### Exception Handling
```cpp
void unhandledException(const std::exception* e,
                        const String& sourceFilename,
                        int lineNumber) override {
    String message = "Unhandled exception";
    if (e != nullptr) {
        message += ": " + String(e->what());
    }
    message += "\nFile: " + sourceFilename;
    message += "\nLine: " + String(lineNumber);

    AlertWindow::showMessageBoxAsync(
        MessageBoxIconType::WarningIcon,
        "Error", message);

    // Optionally quit
    quit();
}
```

## Related Modules
- **DocumentWindow Module** (documentwindow-module.md) - Window management
- **Component Module** (component-module.md) - UI building blocks
- **MessageManager Module** (messagemanager-module.md) - Event dispatching

## JUCE API References
- [juce::JUCEApplication Class](https://docs.juce.com/master/classJUCEApplication.html)
- [Application Basics Tutorial](https://docs.juce.com/master/tutorial_main_component.html)
- [MessageManager Class](https://docs.juce.com/master/classMessageManager.html)

## Testing Considerations
- Verify application starts and shows window
- Test graceful shutdown (no crashes)
- Validate command-line argument parsing
- Check behavior on OS logout/restart
- Test exception handling (throw in initialise())
- Verify memory cleanup (Valgrind, sanitizers)
- Test multiple instance behavior
- Validate platform-specific launch mechanisms

## Platform-Specific Notes

### Windows
- Application manifest required for DPI awareness
- Icon resource embedded in executable
- Task bar integration via Shell API

### macOS
- Info.plist required for app bundle
- Dock icon and menu bar integration
- Retina display support (NSHighResolutionCapable)

### Linux
- Desktop entry file for menu integration
- X11 window hints for desktop environment
- Wayland support via XWayland or native backend

### iOS
- UIApplicationDelegate integration
- Background modes in Info.plist
- App lifecycle (active/inactive/background)

### Android
- Activity subclass as entry point
- AndroidManifest.xml configuration
- Permissions handling

## Debugging Tips

### Enable Console Output
```cpp
void initialise(const String& commandLine) override {
    DBG("Application starting...");
    DBG("Command line: " + commandLine);
    mainWindow.setVisible(true);
    DBG("Window visible");
}
```

### Catch Initialization Failures
```cpp
void initialise(const String& commandLine) override {
    try {
        mainWindow.setVisible(true);
    } catch (const std::exception& e) {
        AlertWindow::showMessageBoxAsync(
            MessageBoxIconType::WarningIcon,
            "Startup Error",
            String("Failed to start: ") + e.what());
        quit();
    }
}
```

---

**Needs Manual Review**:
- [ ] Document recommended startup time budgets per platform
- [ ] Add guidelines for multi-document applications
- [ ] Specify memory leak detection strategies
- [ ] Document plugin vs. standalone lifecycle differences
- [ ] Add examples of application state persistence
- [ ] Specify crash reporting integration points
