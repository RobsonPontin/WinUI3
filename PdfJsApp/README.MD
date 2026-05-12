# PDF Viewer App with PDF.js

This sample packages PDF.js inside a WinUI 3 desktop application and hosts the viewer through WebView2. The solution uses a C# host app plus a native C++/WinRT adapter layer to bridge WebView2 host objects into WinRT-friendly APIs, making it a useful reference for mixed-language WebView integration.

## Solution Structure

```text
PdfJsApp
|-- PdfJsApp/                     C# WinUI 3 host application
|-- PdfJsViewer/                  PDF.js-based viewer control and assets
|-- WvToWinRtAdapter/             C++/WinRT adapter for WebView2-to-WinRT bridging
|-- WvToWinRtAdapterCsProjection/ CsWinRT projection for adapter consumption
`-- WvToWinRtHostObjectLib/       Native host object library used by WebView2
```

## Key Details

- `PdfJsApp` is the desktop shell that initializes the viewer and hosts WebView2.
- `PdfJsViewer` carries the PDF.js viewer experience that is deployed with the app.
- `WvToWinRtAdapter` is the native C++/WinRT bridge that adapts WebView2 host object interactions into WinRT types.
- `WvToWinRtAdapterCsProjection` exposes the adapter to managed code through a generated projection.
- `WvToWinRtHostObjectLib` contains the native host object implementation used by the browser surface.
- The sample demonstrates one way to keep the app shell in C# while placing lower-level interop and host-object plumbing in native code.
- Because the viewer is deployed locally, the solution is a good starting point for offline or customized PDF.js scenarios.
- The architecture is especially helpful when experimenting with message passing, host object exposure, and WinUI 3 plus WebView2 integration.

## Building

1. Open `PdfJsApp.sln` in Visual Studio 2022.
2. Restore NuGet packages and ensure WebView2, Windows App SDK, .NET desktop, and C++/WinRT workloads are installed.
3. Build the solution for `x64` or `ARM64`.
4. Make sure the adapter, projection, and host object projects build before launching the app.
5. Run `PdfJsApp` to verify the packaged PDF.js viewer loads correctly inside WebView2.

### Useful Resources

- [Mozilla PDF.js][def]
- [WebView2 for WinUI3][def2]

[def]: https://github.com/mozilla/pdf.js
[def2]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/get-started/winui