# WinUI 3 C++ App Consuming a .NET Library Published with NativeAOT

This sample explores how a native WinUI 3 desktop application can call into a C# library that has been published using NativeAOT. The important idea is that `MyDotNetLib` is compiled into a native DLL, allowing the C++/WinRT app to consume exported functionality without requiring the full .NET runtime to be present at execution time.

## Solution Structure

```text
NativeAotApp
|-- NativeAotApp/   C++/WinRT WinUI 3 desktop application
`-- MyDotNetLib/    C# library published as a NativeAOT native DLL
```

## Key Details

- `NativeAotApp` is the native desktop front end built with C++/WinRT.
- `MyDotNetLib` is authored in C# but published as a NativeAOT binary.
- Publishing with NativeAOT produces a native DLL that can be consumed like a native dependency rather than a typical managed assembly.
- This makes the sample useful for experimenting with native interop patterns where the producer is written in C# but the consumer is not a .NET application.
- The solution is intentionally small so you can focus on the publish step, the produced binary, and the consumption path from the WinUI 3 app.

## Building

1. Open `NativeAotApp.sln` in Visual Studio 2022.
2. Restore packages and confirm both .NET SDK support and C++ desktop tooling are installed.
3. Build the solution for a supported platform such as `x64` or `ARM64`.
4. Publish `MyDotNetLib` as NativeAOT before launching the app so the expected native output exists.

### Running the sample

Before running the app, publish the .NET library as NativeAOT using dotnet CLI tool:

1. Navigate to the library folder `..\MyDotNetLib`
2. Open PowerShell and run `dotnet publish -r win-x64 -c Release`
3. Run the NativeAotApp

### References
- https://learn.microsoft.com/en-us/dotnet/core/deploying/native-aot
- https://github.com/dotnet/samples/tree/main/core/nativeaot/HelloWorld
