# WasdkSelfContainedNativeApp

A C++/WinRT Windows App SDK sample focused on self-contained deployment. It shows how to ship a native WinUI 3 app without depending on the framework package while still combining native windowing details with managed component consumption.

## Solution Structure

```
WasdkSelfContainedNativeApp.sln
├── SelfContainedDeployment
├── LibXaml
├── LibXaml.Common
└── LibIdentityModel
```

`SelfContainedDeployment` is the main native app and contains the WinUI pages, window setup, and scenario code. `LibXaml` and `LibXaml.Common` support the XAML and native app infrastructure, while `LibIdentityModel` adds a managed component to demonstrate mixed native/managed composition inside the self-contained app.

Beyond deployment mode, the sample also highlights practical desktop concerns such as Win32 window handle access, icon loading, DPI-aware sizing and centering through `AppWindow`, and manifest-driven activation of C# components from native code.

## Building

1. Open `WasdkSelfContainedNativeApp.sln` in Visual Studio 2022.
2. Restore NuGet packages.
3. Set **SelfContainedDeployment** as the startup project.
4. Choose **Debug** or **Release** and **x64**, **x86**, or **ARM64**.
5. Build and run with **F5**.

## Key Technologies

- Windows App SDK self-contained deployment
- WinUI 3 with C++/WinRT
- Native and managed component interop
- `AppWindow` and DPI-aware sizing
- Win32 window handle access
- Manifest-based component activation
