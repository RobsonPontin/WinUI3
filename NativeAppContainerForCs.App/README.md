# NativeAppContainerForCs.App

A hybrid sample where a managed WinUI 3 entry point hands off to native C++/WinRT code that owns the app container and window creation. It demonstrates a layered hosting model for combining .NET-based app startup with native Windows App SDK infrastructure.

## Solution Structure

```
NativeAppContainerForCs.App.sln
├── Apps
│   ├── ContainerForCs.App
│   └── ContainerForCs.AppCs
└── Libs
    ├── ContainerForCs.AppLib
    ├── ContainerForCs.AppLib.CsWinRT
    └── ContainerForCs.CsWinRtComponent
```

`ContainerForCs.AppCs` provides the C# entry point (`Program.cs`) and initializes the COM wrapper support needed for managed/native interop. `ContainerForCs.AppLib` is the native C++/WinRT app library that creates the XAML window, while `ContainerForCs.AppLib.CsWinRT` and `ContainerForCs.CsWinRtComponent` bridge projected WinRT APIs between the managed and native layers. `ContainerForCs.App` gives the solution a native host project alongside the managed one.

The sample is especially useful for understanding WinRT.Host-based activation, CsWinRT-assisted interop, and app startup patterns where the UI shell is native but orchestration begins in managed code.

## Building

1. Open `NativeAppContainerForCs.App.sln` in Visual Studio 2022.
2. Restore NuGet packages.
3. Set **ContainerForCs.AppCs** as the startup project for the managed-to-native handoff scenario.
4. Choose **Debug** or **Release** and **x64** or **ARM64**.
5. Build and run with **F5**.

## Key Technologies

- WinUI 3 hybrid hosting
- C# and C++/WinRT interop
- CsWinRT projections
- `WinRT.Host.dll`
- Native app container patterns
- Managed-to-native startup delegation
