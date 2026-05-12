# MultiprocessApp

A multi-process WinUI 3 sample that combines packaged desktop activation, a full-trust service process, and alternate app entry points. The solution includes both WinUI 3 and UWP fronts ends so the same activation and redirection ideas can be compared across app models.

## Solution Structure

```
Multiprocess.Sample.sln
├── Multiprocess.Service
├── WinUI3
│   ├── Multiprocess.App
│   └── Multiprocess.App (Package)
└── UWP
    ├── Multiprocess.App.uwp
    └── Multiprocess.App.uwp (Package)
```

`Multiprocess.App` is the WinUI 3 packaged application, `Multiprocess.Service` is the full-trust companion process, and the UWP projects provide the parallel UWP-based version of the scenario. Together they demonstrate launching a secondary process from a package, redirecting activation to the right process, and handling protocol or file-type based entry across process boundaries.

This sample is helpful when exploring the `fullTrustProcess` manifest extension, app-to-service coordination, and how packaged Windows apps can split UI and background responsibilities while preserving activation behavior.

## Building

1. Open `Multiprocess.Sample.sln` in Visual Studio 2022.
2. Restore NuGet packages if needed.
3. Choose the startup package you want to test: **Multiprocess.App (Package)** or **Multiprocess.App.uwp (Package)**.
4. Select **Debug** or **Release** and **x64** or **ARM64**.
5. Build and deploy with **F5**.

## Key Technologies

- WinUI 3 and UWP comparison
- C++/WinRT
- Full-trust packaged processes
- `fullTrustProcess` manifest extension
- Protocol and file activation
- Multi-process app coordination
