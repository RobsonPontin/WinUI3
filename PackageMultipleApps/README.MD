# Package Multiple Apps (UWP and WinUI 3)

This sample packages a UWP app and a WinUI 3 desktop app together by using a Windows Application Packaging (WAP) project. The UWP app acts as the entry point and can redirect activation to the WinUI 3 app, which makes the solution useful for exploring migration strategies and packaged scenarios that still rely on capabilities more readily available in UWP.

## Solution Structure

```text
PackageMultipleApps
|-- WinUi3App/          C++/WinRT WinUI 3 desktop app
|-- UwpApp/             C++/WinRT UWP redirector app
|-- AppPackaging/       WAP packaging project
|-- ControlsLib/        Native shared controls library
|-- ControlsLib.CS/     Managed shared controls library (ControlsLibCs)
`-- ControlsLibCs_Dep/  Supporting managed dependency project
```

## Key Details

- The solution mixes C++/WinRT desktop and UWP app models in a single packaged deployment.
- `WinUi3App` is the WinUI 3 desktop experience that ultimately handles the redirected activation.
- `UwpApp` serves as the packaged entry point and can front-load activation scenarios before handing off to the desktop app.
- `AppPackaging` is the project that ties the applications and libraries into one deployable package.
- `ControlsLib`, `ControlsLibCs`, and `ControlsLibCs_Dep` show how shared native and managed assets can live alongside the two app models.
- This setup is primarily interesting for compatibility and migration experiments rather than for a minimal production architecture.

## Pros

In theory this should allow us to unlock a few unsupported features on WinUI 3, like:
1. Keep ProgId when updating UWP app to WinAppSdk which is not yet supported fully (i.e. your app might display "New" on OpenWith after updating it)
2. Support AutoPlay manifest declaration in UWP since WinUI still does not support it

## Cons

1. Startup perf impact since the UWP app always need to be launched before launching the WinUI 3 App
2. `RuntimeBroker.exe` might be running even though the UWP App redirector have exited but WinUI 3 app is still running
3. Customize the entire activation
4. Customize the `App` and `Resources` name to avoid conflict when packaging 2 apps together (i.e. `App.xaml` needs to be renamed to `App2.xaml` in one of the apps)

## Building

1. Open `PackageMultipleApps.sln` in Visual Studio 2022.
2. Restore dependencies and ensure the UWP, C++ desktop, and Windows App SDK workloads are installed.
3. Choose a packaged configuration such as `Debug|x64`, `Debug|ARM64`, or the matching release configuration.
4. Build the shared libraries first or build the full solution so all app and library dependencies are produced together.
5. Deploy `AppPackaging` to test the combined package and activation redirection flow.

Use this sample when you want to study tradeoffs in packaging multiple Windows app models together during a migration to WinUI 3.