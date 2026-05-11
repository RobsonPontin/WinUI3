# MultiApp

A WinUI 3 (C++/WinRT) solution demonstrating how to package and launch multiple executables from a single MSIX package — a parent app that spawns child processes, each running in its own window, with inter-process communication via Named Pipes.

## Architecture

The solution contains four projects:

```
MultiApp.sln
├── MultiApp            → Main (parent) application
├── ChildApp            → Child application (spawned by MultiApp)
├── ProcessBridge       → Shared DLL: process launching, IPC, protocol
└── MultiApp (Package)  → Windows Application Packaging Project (MSIX)
```

### ProcessBridge (Shared DLL)

A generic, app-agnostic dynamic library that provides:

- **Process launching** — `IChildProcessLauncher` interface with DI-friendly factory; callers specify the child executable name.
- **Named Pipe IPC** — `IIpcServer` / `IIpcClient` interfaces for bidirectional request/response communication.
- **Protocol constants** — shared command-line switch names, pipe name prefix, and utility functions (`GetOwnDirectory`, `TryGetSwitchValue`).

All types live under the `ProcessBridge::` namespace (sub-namespaces: `Protocol::`, `Ipc::`).

### MultiApp (Parent)

The main application presents a window with a **"Launch Child App"** button. When clicked, it:

1. Launches `ChildApp.exe` on a **worker thread** (keeps the UI responsive).
2. Performs the security handshake (see below).
3. Connects to the child's Named Pipe server with a 5-second retry timeout.
4. Runs a health-check loop, pinging the child every 2 seconds.
5. Displays IPC status with color indicators: 🟡 connecting, 🟢 connected, 🔴 disconnected.
6. Shows data received from the child (e.g., random numbers).

### ChildApp (Child)

The child application validates its parent before showing UI:

1. Parses command-line arguments for the validation event, parent PID, and **pipe name**.
2. Verifies that the parent executable is `MultiApp.exe` co-located in the same directory.
3. Signals the named event to confirm validation.
4. Starts a Named Pipe IPC server on a background thread.
5. Handles `status`, `ping`, and data delivery commands.
6. Provides a **"Send Random Number"** button — generates a random number (1–1000) and delivers it to the parent on the next ping.

### Launch Validation + IPC Handshake

```
┌────────────┐                              ┌────────────┐
│  MultiApp  │                              │  ChildApp  │
└─────┬──────┘                              └─────┬──────┘
      │  1. Generate random token                  │
      │  2. Create named event                     │
      │  3. CreateProcessW(ChildApp.exe,           │
      │     --mp-validation-event=<token>          │
      │     --mp-validation-pid=<pid>              │
      │     --mp-ipc-pipe=<pipeName>)              │
      │───────────────────────────────────────────►│
      │                                            │ 4. Parse args
      │                                            │ 5. Open parent by PID
      │                                            │ 6. Verify parent is
      │                                            │    MultiApp.exe (same dir)
      │                    7. SetEvent(token)       │
      │◄───────────────────────────────────────────│
      │  8. Wait satisfied → launch succeeded      │ 9. Start IPC server
      │                                            │
      │  10. Connect to pipe (retry up to 5 s)     │
      │───────────────────────────────────────────►│
      │                                            │ 11. Accept connection
      │  12. Send "status" request                 │
      │───────────────────────────────────────────►│
      │◄───────────────────────────────────────────│ 13. Respond "ready"
      │                                            │
      │  14. Ping loop (every 2 s)                 │
      │◄──────────── bidirectional ───────────────►│
      │     Picks up child data (random numbers)   │
```

## IPC Design

- **Transport:** Windows Named Pipes (`\\.\pipe\MultiAppIpc-<token>`)
- **Wire format:** Length-prefixed binary — `[4B cmd len][cmd wchars][4B payload len][payload wchars]`
- **Pattern:** Request/response — parent sends, child responds. Child pushes data by attaching it to the next ping response.
- **Threading:** All IPC work runs on background threads. UI updates are marshalled via `DispatcherQueue.TryEnqueue()`.
- **Lifetime safety:** Worker threads capture `get_strong()` to extend the WinRT object lifetime. `m_closing` atomic flag prevents XAML access during teardown.

## Packaging

Both executables and `ProcessBridge.dll` are bundled into a single **MSIX package** via the Windows Application Packaging Project (`.wapproj`). The packaging project:

- References `MultiApp.vcxproj`, `ChildApp.vcxproj`, and `ProcessBridge.vcxproj`.
- Includes `ProcessBridge.dll` as manual Content (native DLLs require explicit deployment in WAP).
- Sets `MultiApp` as the entry point application.
- Declares the `runFullTrust` capability.
- Targets Windows 10 version 1809 (build 17763) and above.

## Prerequisites

- **Visual Studio 2022** (v17.0+)
- **Windows App SDK** workload
- **C++ Desktop Development** workload
- **Windows 10 SDK** (10.0.17763.0 or later)
- **C++20** standard (configured in all projects)

## NuGet Packages

Both app projects use the following packages (managed via `packages.config`):

| Package | Purpose |
|---------|---------|
| `Microsoft.WindowsAppSDK` | Core Windows App SDK runtime |
| `Microsoft.WindowsAppSDK.WinUI` | WinUI 3 XAML framework |
| `Microsoft.Windows.CppWinRT` | C++/WinRT language projection |
| `Microsoft.Windows.ImplementationLibrary` | WIL (Windows Implementation Library) |
| `Microsoft.Windows.SDK.BuildTools` | Windows SDK build tools |
| `Microsoft.Web.WebView2` | WebView2 runtime |

ProcessBridge uses a minimal subset: `Microsoft.Windows.SDK.BuildTools` and `Microsoft.Windows.ImplementationLibrary`.

## Build & Run

1. Open `MultiApp.sln` in Visual Studio 2022.
2. Restore NuGet packages (should happen automatically).
3. Set **MultiApp (Package)** as the startup project.
4. Select a platform (x64, x86, or ARM64).
5. **Rebuild** the solution (Build → Rebuild Solution).
6. Deploy and run (F5).

> **Note:** After adding or renaming `x:Name` elements in XAML, always do a **Rebuild** (not just Build) to regenerate the XAML codegen files.

> **Note:** The packaging project has code signing disabled (`AppxPackageSigningEnabled = false`). For distribution, configure a signing certificate in the project properties.

## Platform Support

| Platform | Supported |
|----------|-----------|
| x64      | ✅        |
| x86      | ✅        |
| ARM64    | ✅        |

## License

See repository root for license information.
