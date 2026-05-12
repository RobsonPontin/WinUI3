# AutoPlay Listener App

This sample shows a C++/WinRT-based AutoPlay experience that routes Windows AutoPlay events into a packaged WinUI 3 app. It combines a desktop UI, a simple command-line entry point, and packaging infrastructure so the app can participate in AutoPlay, activate through a custom protocol, and bridge packaged and full-trust scenarios.

## Solution Structure

```text
AutoPlayListenerApp
|-- AutoPlayApp/              WinUI 3 desktop app
|-- AutoPlayAppConsole/       CLI helper and app execution alias target
|-- ComServerExe/             Full-trust COM server executable
`-- AutoPlayAppPackage.WAP/   Packaging project and manifest declarations
```

## Key Details

- `AutoPlayApp` is the main C++/WinRT WinUI 3 application.
- `AutoPlayAppConsole` provides a lightweight command-line entry point for testing and protocol-based launch flows.
- `ComServerExe` hosts the full-trust COM server pieces needed for registration and activation scenarios.
- `AutoPlayAppPackage.WAP` packages the app, protocol, alias, and related registration metadata together.
- The sample demonstrates Windows AutoPlay handler registration and handoff into the packaged WinUI 3 experience.
- Protocol activation is used as the boundary between the AutoPlay trigger and the desktop UI.
- `App.xaml.cpp` contains the important activation logic: it uses `AppInstance` and `IProtocolActivatedEventArgs` to process protocol launches that originate from AutoPlay.
- The packaged app model also enables an app execution alias, making it easier to invoke the sample from a simple CLI command.
- Full-trust process and COM server registration are part of the end-to-end setup, which is useful when exploring richer desktop integration patterns.

## Building

1. Open `AutoPlayListenerApp.sln` in Visual Studio 2022.
2. Restore NuGet packages and ensure the Windows App SDK and C++ desktop tooling are installed.
3. Select a packaged configuration such as `Debug|x64` or `Release|x64`.
4. Build the solution so `AutoPlayApp`, `AutoPlayAppConsole`, `ComServerExe`, and `AutoPlayAppPackage.WAP` are produced together.
5. Deploy the packaging project to register the protocol, AutoPlay handler metadata, app execution alias, and COM server information.
6. Launch the packaged app or invoke the console entry point to test the protocol activation path.

This sample is most useful when you want to study how AutoPlay handlers, protocol activation, packaged desktop apps, and full-trust components can work together in a single WinUI 3 solution.