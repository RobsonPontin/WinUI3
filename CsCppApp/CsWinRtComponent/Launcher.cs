using Microsoft.UI.Xaml.Controls;
using System.Diagnostics;

namespace CsWinRtComponent
{
    /// <summary>
    /// Exported type which can be used to activate C#/WinRt managed code from C++/WinRT Native App.
    /// </summary>
    public sealed class Launcher
    {
        static public void OpenDialogAsync(Microsoft.UI.Xaml.Window window)
        {
            ContentDialog contentDialog = new();
            contentDialog.XamlRoot = window.Content.XamlRoot;
            contentDialog.PrimaryButtonText = "Ok";
            contentDialog.Content = "This is a dialog opened from a C#/WinRt Component";
            
            _ = contentDialog.ShowAsync();
        }
    }

    /// <summary>
    /// The only reason this exist is to satisfy the WinRT.Host.dll which will insist
    /// to declare an InProcessServer in the AppxManifest.xml.
    /// </summary>
    public sealed class LauncherDummy
    {
        static public void Run()
        {
            Debug.WriteLine("Running " + nameof(LauncherDummy));
        }
    }
}
