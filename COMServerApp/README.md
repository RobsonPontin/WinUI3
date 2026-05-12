# COMServerApp

A WinUI 3 sample demonstrating packaged out-of-process COM activation with C++/WinRT. The solution pairs a COM server and client inside an MSIX deployment, and includes the supporting proxy/stub pieces needed to validate cross-process calls.

## Solution Structure

```
COMServerApp.sln
├── Client
│   └── COM.ClientApp
├── Server
│   └── COM.ServerApp
├── Proxy
│   ├── COM.ServerLib
│   └── COM.Server.Proxy
└── COM.PackageApp (Package)
```

`COM.ServerApp` registers COM classes at runtime with `CoRegisterClassObject`, while `COM.ClientApp` activates those classes from a separate process and exercises the marshaled interface boundary. `COM.ServerLib` holds the shared COM types and implementation contracts, and `COM.Server.Proxy` provides the proxy/stub plumbing for custom interface marshaling.

Because everything is deployed through `COM.PackageApp (Package)`, the sample is useful for testing packaged COM registration behavior, activation identities, and how proxy/stub binaries participate in an MSIX-based desktop deployment.

## Building

1. Open `COMServerApp.sln` in Visual Studio 2022.
2. Restore NuGet packages if Visual Studio does not do it automatically.
3. Set **COM.PackageApp (Package)** as the startup project.
4. Choose **Debug** or **Release** and **x64** or **ARM64**.
5. Build and deploy with **F5**.

## Key Technologies

- WinUI 3 desktop packaging
- C++/WinRT
- Out-of-process COM activation
- `CoRegisterClassObject`
- COM proxy/stub projects
- MSIX packaged desktop apps
