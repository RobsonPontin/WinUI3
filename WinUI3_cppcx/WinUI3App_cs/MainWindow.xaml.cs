// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using System;
using System.Diagnostics;
using System.Threading.Tasks;

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
          
        private async void btnExit_Click(object sender, RoutedEventArgs e)
        {
            // Ways of terminating the process:
            // 1. Window.Close()
            // 2. AppWindow.Destroy()
            int exitType = 0;

            switch (exitType)
            {
                case 0:
                    this.Close();
                    break;

                case 1:
                    var hWnd = WinRT.Interop.WindowNative.GetWindowHandle(this);
                    // Retrieve the WindowId that corresponds to hWnd.
                    Microsoft.UI.WindowId windowId =
                        Microsoft.UI.Win32Interop.GetWindowIdFromWindow(hWnd);

                    var appWindow = AppWindow.GetFromWindowId(windowId);

                    appWindow.Destroy();
                    break;
            }
        }

        int m_dispatcherRuns = 0;
        private void btnTestDispatcher_Click(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < 200; i++)
            {
                this.DispatcherQueue.TryEnqueue(async () =>
                {
                    m_dispatcherRuns++;

                    Random random = new();

                    await Task.Delay(random.Next(1000));

                    tbText.Text = "[Debug][Test] Running dispatcher #" + m_dispatcherRuns.ToString() + "...";                  
                });
            }
        }
    }
}
