# MigrationProgIdDehApp

A compact C++/WinRT WinUI 3 sample focused on ProgID migration for packaged desktop apps. It exists to test how legacy application ProgIDs can be migrated during upgrades by using the Default Experience Handler-related manifest extension.

## Solution Structure

```
MigrationProgIdDehApp.sln
└── MigrationProgIdDehApp
```

The sample is intentionally small: one WinUI 3 application project with the manifest and registration data needed to exercise `desktop11:MigrateApplicationProgIds`. That makes it easy to isolate upgrade and association behavior without extra packaging or library projects getting in the way.

This is particularly useful for validating DEH migration scenarios where a newer packaged app version needs to preserve or move existing file association identity cleanly.

## Building

1. Open `MigrationProgIdDehApp.sln` in Visual Studio 2022.
2. Restore NuGet packages if prompted.
3. Set **MigrationProgIdDehApp** as the startup project.
4. Choose **Debug** or **Release** and **x64**, **x86**, or **ARM64**.
5. Build and deploy with **F5**.

## Key Technologies

- WinUI 3 desktop apps
- C++/WinRT
- File type associations
- ProgID migration
- `desktop11:MigrateApplicationProgIds`
- Default Experience Handler testing
