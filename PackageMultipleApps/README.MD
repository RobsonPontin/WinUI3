# Package Multiple Apps (UWP and WinUI 3)

The idea for this sample project is to package both UWP and WinUI 3 app under the same roof (using WAP - Windows Application Packaging)  where the Uwp app will work as the main entry point and redirect all activations to the WinUI 3 app.

## Pros

In theory this should allow us to unlock a few unsupported feature on WinUI 3, like:
1. Keep ProgId when updating UWP app to WinAppSdk which is not yet supported fully (i.e. your app might display "New" on OpenWith after updating it)
2. Support AutoPlay manifest declaration in UWP since WinUI still does not support it

## Cons 
1. Startup perf impact since the UWP app always need to be launched before launching the WinUI 3 App
2. "RuntimeBroker.exe" might be running even though the UWP App redirector have exited but WinUI 3 app is still running
3. Customize the entire activation
4. Customize the "App" and "Resources" name to avoid conflict when packaging 2 apps together (i.e. App.xaml needs to be renamed to App2.xaml in one of the apps)