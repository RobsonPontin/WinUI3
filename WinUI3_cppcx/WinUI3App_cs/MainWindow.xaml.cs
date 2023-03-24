// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.FileProperties;

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
                    this.Content = null;
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
        BasicProperties m_basicProps;
        private async void btnTestDispatcher_Click(object sender, RoutedEventArgs e)
        {
            Windows.Storage.StorageFolder installedLocation = Windows.ApplicationModel.Package.Current.InstalledLocation;
            var folder = await StorageFolder.GetFolderFromPathAsync(installedLocation.Path);
            var file = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///Assets/Wide310x150Logo.scale-200.png"));

            string tempFileName = "myFile";

            for (int i = 0; i < 20000; i++)
            {
                await Task.Delay(1);

                this.DispatcherQueue?.TryEnqueue(async () =>
                {
                    tbText.Text = "[Debug][Test] Tasks left: " + m_dispatcherRuns.ToString();

                    if (m_basicProps == null)
                    {
                       await Task.Delay(10);
                        m_basicProps = await file.GetBasicPropertiesAsync();
                    }

                    await folder.CreateFileAsync(tempFileName, CreationCollisionOption.ReplaceExisting);
                });

                this.DispatcherQueue?.TryEnqueue(async () =>
                {
                    if (m_basicProps != null)
                    {
                        tbText2.Text = "[Debug][Test] Tasks left: " + m_basicProps.DateModified.ToString();
                        m_basicProps = null;
                    }

                    try
                    {
                        var myfile = await folder.GetFileAsync(tempFileName);
                        await myfile.DeleteAsync();
                    }
                    catch (Exception ex)
                    {
                    }
                });

                m_dispatcherRuns++;
            }
        }
    }
}
