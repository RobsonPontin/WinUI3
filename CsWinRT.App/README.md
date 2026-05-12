# CsWinRT.App

A WinUI 3 sample showing how a C# desktop app can consume a native C++/WinRT component through the CsWinRT projection pipeline. It demonstrates the full path from native implementation to managed consumption.

## Solution Structure

```
CsWinRT.App.sln
├── CsWinRT.App
├── MyCustomLibrary.CppWinRT
├── MyCustomLibrary
└── MyCustomLibrary.CsWinRT
```

`MyCustomLibrary.CppWinRT` contains the native Windows Runtime component, while `MyCustomLibrary.CsWinRT` generates the managed projection that `CsWinRT.App` references. The `MyCustomLibrary` project rounds out the native side of the sample and keeps the solution focused on the interop boundary rather than just app UI.

This makes the sample useful when validating activatable in-process server registration, CsWinRT-generated projections, and the manifest wiring required for a packaged WinUI 3 app to load a native WinRT component from C#.

## Building

1. Open `CsWinRT.App.sln` in Visual Studio 2022.
2. Restore NuGet packages.
3. Set **CsWinRT.App** as the startup project.
4. Build the solution in **Debug|x64** or **Release|x64**.
5. Run with **F5** to verify the C# app can activate the projected component.

## Key Technologies

- WinUI 3 for .NET
- C# and C++/WinRT interop
- CsWinRT projection generation
- Windows Runtime components
- In-process server manifest registration
- Packaged desktop application model
