using CustomControlsLib;
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

            var stackPanel = new StackPanel();
            stackPanel.Children.Add(new TextBlock {
                Text = "This is a dialog opened from a C#/WinRt Component", 
                Margin = new Microsoft.UI.Xaml.Thickness(0, 5, 5, 0) 
            });
            stackPanel.Children.Add(new MyUserControl());
            stackPanel.Children.Add(new MyOtherUserControl());

            contentDialog.Content = stackPanel;
            
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
