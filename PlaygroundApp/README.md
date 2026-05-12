# PlaygroundApp

A WinUI 3 desktop playground application written in C++/WinRT for experimenting with Windows APIs — file pickers, process launching, image resizing, video frame extraction, and custom XAML controls.

## Solution Structure

```
PlaygroundApp.sln
├── Playground.App        (App1.vcxproj)       – Main WinUI 3 application (C++/WinRT)
├── Playground.Controls   (C++/WinRT)          – Reusable custom XAML controls library
└── Playground.Package    (.wapproj)           – MSIX packaging project
```

### Playground.App

The main application window exposes a button-driven UI to exercise various Windows APIs. Each feature area is encapsulated in its own test module:

| Module | Description |
|---|---|
| **TestApplicationData** | Benchmarks `ApplicationData::LocalSettings` — init, write and read 1 000 string entries with timing, plus multi-threaded read/write stress test. |
| **TestPickerApis** | File open/save pickers via WinRT (`FileOpenPicker`, `FileSavePicker`), Win32 (`GetSaveFileNameA`), and COM Shell (`IFileSaveDialog`). Supports image and video file filters. |
| **TestLauncher** | Launches processes using five different approaches: `ShellExecuteEx`, `CreateProcessW`, `IApplicationActivationManager` (by AUMID), `ActivateForFile`, and protocol URI launch (`ms-photos:`). |
| **TestImageResize** | Image resizing through two pipelines: **WinRT** (`BitmapDecoder` + `BitmapTransform`) and **WIC** (`IWICBitmapScaler`). A third **Direct2D** path (`D2DImageResizer`) is present but still WIP. Results are displayed in an `Image` control. |
| **TestVideoApis** | Video playback with `MediaPlayer` / `MediaPlayerElement`, frame extraction via `MediaComposition.GetThumbnailAsync`, and a Video Frame Server mode using D3D11 surfaces to capture individual frames and display them as `SoftwareBitmap`. |

#### Supporting Utilities

| File | Purpose |
|---|---|
| `DebugLog.h/cpp` | Lightweight stopwatch for timing API calls (returns elapsed ms). |
| `D3DResources.h` | Creates a D3D11 device + BGRA texture + WinRT `IDirect3DSurface` for video frame server capture. |
| `D2DImageResizer.h` | Direct2D + WIC image decode/scale pipeline (hardware-accelerated via `HwndRenderTarget`). |
| `WICImageResizer.h` | CPU-based WIC image decode, format-convert, and scale. Also supports extracting thumbnails from the Windows Thumbnail Cache (`IThumbnailCache`). |
| `ImageView.h` | Simple pixel-buffer wrapper (width, height, pixel format, raw bytes). |
| `NamespacesFramework.h` | Namespace aliases for WinRT/WinAppSDK namespaces to reduce verbosity across the project. |

### Playground.Controls

A C++/WinRT component library providing custom XAML controls. Currently contains:

- **SwapChainPanelControl** — A `UserControl` wrapping a `SwapChainPanel` with pointer-move tracking. Intended for Direct3D/Direct2D rendering scenarios within XAML.

### Playground.Package

MSIX packaging project that bundles the app for deployment. Registers an app execution alias (`PlaygroundApp.exe`) so the app can be launched from the command line or via `ShellExecuteEx`.

## Dependencies

| Package | Version |
|---|---|
| Windows App SDK | 1.6.241114003 |
| C++/WinRT | 2.0.240405.15 |
| WIL (Windows Implementation Library) | 1.0.240803.1 |
| WebView2 | 1.0.2651.64 |
| Windows SDK Build Tools | 10.0.26100.1591 |

## Platforms

- **x64** and **ARM64**
- Debug / Release configurations
- Minimum OS: Windows 10 1809 (build 17763)

## Building

1. Open `PlaygroundApp.sln` in Visual Studio 2022 (v17.10+).
2. Set **Playground.Package** as the startup project.
3. Select platform (x64 or ARM64) and configuration (Debug or Release).
4. Build and deploy (F5).

## UI Overview

The main window is split into a 2×2 grid:

- **Top-left** — Scrollable button panel for triggering each test.
- **Bottom-left** — `RichTextBlock` feedback area showing timing results and status messages.
- **Right column** — `Image` control for displaying resized images or extracted video frames.
