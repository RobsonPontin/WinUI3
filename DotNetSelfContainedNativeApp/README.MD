## Packaged WinUI 3 C++ App as Self-contained .NET Runtime

This sample project shows how a WinUI 3 C++ App can have a dependency on a self-contained C#/WinRt component, access it's APIs and also package itself as a .NET self-contained application.

The idea of this project is to improve launch performance since the application is 100% native, and then by demand load any C#/.NET related code and UI.

![App Diagram](../doc/app_diagram.png)

There are a few benefit of this architecture:
1. Startup speed of Native app is very fast since it does not require to load .NET runtime dlls
2. The .NET runtime dlls can be loaded by demand by setting the flag `<XamlCodeGenerationControlFlags>DoNotGenerateOtherProviders</XamlCodeGenerationControlFlags>`, this speeds up startup time for the Native App.
3. All of the .NET runtime dlls are bundled with the app using a `NativeHost` with `DNNE`.

### References

 - https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting
 - https://github.com/microsoft/CsWinRT/issues/1141
  - https://github.com/AaronRobinsonMSFT/DNNE
  - https://github.com/jlaanstra/CsWinRT/commit/cd116cbc3fd93c4f7295972c454de1c610a3da59#diff-311c0d785847546fa8a82270d50fdb644cc909a736a4c508c9d7e733f21be821
