// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;


namespace WinUI3App_cs
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        public MainWindow()
        {
            this.InitializeComponent();
        }

        private void myButton_Click(object sender, RoutedEventArgs e)
        {
            WinRt_cppcx.ClassCppCx test = new WinRt_cppcx.ClassCppCx();
            var str = test.GetStringValue;

            myButton.Content = "Clicked";
        }
    }
}
