# PlaygroundApp.Cs

A minimal C# WinUI 3 sample centered on showing a custom `ContentDialog` from a desktop window. It keeps the UI intentionally small so the dialog setup and lifecycle are easy to inspect.

## Solution Structure

```
PlaygroundApp.Cs.sln
├── PlaygroundApp.Cs
└── PlaygroundApp.Cs (Package)
```

`PlaygroundApp.Cs` contains the main window and dialog logic, including the `XamlRoot` setup required for desktop `ContentDialog` usage. `PlaygroundApp.Cs (Package)` wraps the app in an MSIX packaging project so it can be deployed and run as a packaged WinUI 3 desktop app.

This is a handy reference when you need a stripped-down example of button-driven dialog invocation, dialog lifetime management, and the packaging shape of a small C# WinUI 3 sample.

## Building

1. Open `PlaygroundApp.Cs.sln` in Visual Studio 2022.
2. Restore NuGet packages if needed.
3. Set **PlaygroundApp.Cs (Package)** as the startup project.
4. Choose **Debug** or **Release** and **x64** or **ARM64**.
5. Build and deploy with **F5**.

## Key Technologies

- WinUI 3 for .NET
- C# desktop apps
- `ContentDialog`
- `XamlRoot`
- MSIX packaging projects
- Minimal UI workflow samples
