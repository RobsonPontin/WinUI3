# WinUI 3 App with CsWinRT Projection

This sample demonstrates the full C++/WinRT-to-C# consumption pipeline for a WinUI 3 desktop app. A native Windows Runtime component is authored in C++/WinRT, projected into a .NET-friendly API surface with CsWinRT, and then consumed by a C# WinUI 3 app that instantiates `MyWinRtClass` and listens for `CollectionChanged` notifications.

## Solution Structure

```text
WinUI3_GenCsProjection
|-- WinUI3_GenCsProjection/   C# WinUI 3 app
|-- WinRtComponent/           Native C++/WinRT runtime component
`-- WinRtComponentProjection/ Generated CsWinRT projection assembly
```

## Key Details

- `WinUI3_GenCsProjection` is the C# desktop app that exercises the projected API surface.
- The app creates `MyWinRtClass` from the native component and handles `CollectionChanged` events from managed code.
- `WinRtComponent` contains the actual Windows Runtime implementation authored in C++/WinRT.
- `WinRtComponentProjection` is the generated projection layer that makes the native WinRT types easy to consume from C#.
- The sample is useful for understanding where native implementation ends and generated interop begins.
- It demonstrates a realistic authoring flow: define WinRT types in C++/WinRT, generate the CsWinRT projection, reference the projection from the app, then consume the types like regular .NET objects.
- Event flow is an important part of the sample, because it shows that projected collections and change notifications can move cleanly across the native-to-managed boundary.
- The solution is intentionally small, making it easier to inspect the component, projection, and app side-by-side.

## Building

1. Open `WinUI3_GenCsProjection.sln` in Visual Studio 2022.
2. Restore dependencies and confirm both .NET desktop and C++/WinRT tooling are available.
3. Build the solution for a supported architecture such as `x64` or `ARM64`.
4. Ensure the projection project builds before or alongside the C# app so the generated assembly is available as a reference.
5. Run `WinUI3_GenCsProjection` and exercise the UI to verify `MyWinRtClass` creation and `CollectionChanged` event handling.

Use this sample when you want a compact reference for how C++/WinRT authoring, CsWinRT projection generation, and C# WinUI 3 consumption fit together.