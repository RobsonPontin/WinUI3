using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Markup;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
}
