## WinUI 3 C++ App consuming a .NET library that was published using NativeAOT

The idea for this sample is to try to load and consume APIs from libraries that have been published as NativeAOT, which usually are meant to be consumed from a .NET App.

### Running the sample

Before running the app, publish the .NET library as NativeAOT using dotnet CLI tool:

1. Navigate to the library folder `..\MyDotNetLib`
2. Open PowerShell and run `dotnet publish -r win-x64 -c Release`
3. Run the NativeAotApp

### References
- https://learn.microsoft.com/en-us/dotnet/core/deploying/native-aot
- https://github.com/dotnet/samples/tree/main/core/nativeaot/HelloWorld
