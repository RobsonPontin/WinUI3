using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace CsWinRtComponent
{
    public sealed partial class MyUserControl : UserControl
    {
        public MyUserControl()
        {
            this.InitializeComponent();
        }

        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            ContentDialog dialog = new ContentDialog();
            dialog.PrimaryButtonText = "OK";
            dialog.Content = "You clicked on the button.";
            dialog.XamlRoot = this.XamlRoot;

            await dialog.ShowAsync();
        }
    }
}
