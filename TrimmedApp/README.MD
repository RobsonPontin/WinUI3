# WinUI 3 C# App with Trimming Enabled

This sample is a single-project C# WinUI 3 application created to test trimming behavior in a desktop app. It references Microsoft Graph and Microsoft Graph Beta types so you can inspect what survives the trimmer, what warnings are emitted during publish, and which dependencies need special handling when aggressive size reduction is enabled.

## Solution Structure

```text
TrimmedApp
`-- TrimmedApp/   C# WinUI 3 desktop application
```

## Key Details

- The solution contains one WinUI 3 project: `TrimmedApp`.
- The project is focused on trimming analysis rather than on app features.
- Microsoft Graph and Microsoft Graph Beta types are referenced specifically to test how reflection-heavy or metadata-dependent libraries behave under trimming.
- This makes the sample useful for validating linker warnings, checking runtime behavior after publish, and experimenting with preservation strategies.
- Because the app is intentionally small, it is easier to isolate trimming-related changes from unrelated application code.

## Building

1. Open `TrimmedApp.sln` in Visual Studio 2022.
2. Restore NuGet packages and confirm the .NET desktop and Windows App SDK tooling are installed.
3. Build the project for a supported architecture such as `x64`, `x86`, or `ARM64`.
4. Use a publish configuration with trimming enabled to inspect linker output and generated artifacts.

## Running

1. Run the app from Visual Studio to verify the baseline debug experience.
2. Publish the app with trimming enabled and compare behavior with the non-trimmed build.
3. Review warnings produced during publish to identify APIs or dependencies that are not trim-friendly.
4. Use the sample to validate which Microsoft Graph and Beta API shapes remain available after trimming.

This project is best suited for dependency-compatibility checks and for learning how trimming affects a WinUI 3 desktop application.