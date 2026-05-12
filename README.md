# WinUI3

A collection of WinUI 3 sample projects exploring different approaches, architectures, and possibilities with the Windows App SDK. Each solution is self-contained and focuses on a specific scenario.

## Projects

| Project | Language | Description |
|---|---|---|
| [PlaygroundApp](./PlaygroundApp) | C++/WinRT | Test harness for Windows APIs — file pickers (WinRT/Win32/COM), process launching (5 methods), image resizing (WinRT/WIC/D2D), video frame extraction (MediaPlayer/D3D11), and `ApplicationData` benchmarking. |
| [PlaygroundApp.Cs](./PlaygroundApp.Cs) | C# | Minimal WinUI 3 app demonstrating custom `ContentDialog` usage and `XamlRoot` setup. |
| [AutoPlayListenerApp](./AutoPlayListenerApp) | C++/WinRT | Listens for Windows AutoPlay events and handles protocol activation, with a COM server and console CLI launcher. |
| [COMServerApp](./COMServerApp) | C++/WinRT | Out-of-process COM server/client in a packaged app — COM class registration, proxy/stub, and cross-process activation. |
| [CsWinRT.App](./CsWinRT.App) | C# + C++/WinRT | Full CsWinRT projection pipeline — C++/WinRT component projected to C# and consumed by a WinUI 3 app. |
| [DotNetSelfContainedNativeApp](./DotNetSelfContainedNativeApp) | C++/WinRT + C# | Native app with self-contained .NET runtime loaded on demand via DNNE, for fast native startup with optional managed code. |
| [MigrationProgIdDehApp](./MigrationProgIdDehApp) | C++/WinRT | File type association ProgID migration using `desktop11:MigrateApplicationProgIds` manifest extension. |
| [MultiApp](./MultiApp) | C++/WinRT | Multi-executable MSIX package — parent/child apps communicating via Named Pipes IPC with launch validation handshake. |
| [MultiAppsPackage](./MultiAppsPackage) | C++/WinRT | Experimental two-app MSIX packaging skeleton (WIP). |
| [MultiprocessApp](./MultiprocessApp) | C++/WinRT | Multi-process sample with both WinUI 3 and UWP entry points, full-trust service process, and activation redirection. |
| [NativeAotApp](./NativeAotApp) | C++/WinRT + C# | WinUI 3 app consuming a .NET library published as NativeAOT — no .NET runtime needed at runtime. |
| [NativeAppContainerForCs.App](./NativeAppContainerForCs.App) | C++/WinRT + C# | Hybrid native container hosting a C# WinUI 3 app via `WinRT.Host.dll` and CsWinRT interop. |
| [PackageMultipleApps](./PackageMultipleApps) | C++/WinRT + UWP | UWP and WinUI 3 apps in one WAP package — UWP acts as entry point and redirects activations to WinUI 3. |
| [PdfJsApp](./PdfJsApp) | C# + C++/WinRT | PDF viewer wrapping Mozilla PDF.js inside WebView2, with a C++/WinRT host-object adapter bridge. |
| [TrimmedApp](./TrimmedApp) | C# | Tests publish-trimmed behavior with WinUI 3 and Microsoft Graph types to identify trimmer compatibility. |
| [WasdkSelfContainedNativeApp](./WasdkSelfContainedNativeApp) | C++/WinRT | Self-contained Windows App SDK deployment — no framework package dependency, DPI-aware windowing, C# component hosting. |
| [WinUI3_GenCsProjection](./WinUI3_GenCsProjection) | C# + C++/WinRT | End-to-end pipeline: author a C++/WinRT component, generate a C# projection, and consume it from a WinUI 3 app. |
| [WinUI3_cppcx](./WinUI3_cppcx) | C++/CX + C++/WinRT + C# | Cross-language sample consuming a C++/CX component from both C++/WinRT and C# WinUI 3 apps. |

## Common Stack

All projects target **Windows 10 1809+ (build 17763)** and use:

- **Windows App SDK** (WinUI 3)
- **Visual Studio 2022** (v17.10+)
- **x64** and/or **ARM64** platforms
- **MSIX** packaging (WAP or single-project)

## Building

1. Open the desired `.sln` in Visual Studio 2022.
2. Restore NuGet packages (automatic on build).
3. Set the packaging project as startup.
4. Select platform (x64/ARM64) and build (F5).
