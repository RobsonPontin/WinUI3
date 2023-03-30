// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using System.Threading.Tasks;
using Windows.Storage.FileProperties;
using Windows.Storage;
using Microsoft.UI.Windowing;


namespace WinUI3App_cs
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
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
                    var myApp = App.Current as App;
                    myApp.Shutdown();
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

                    await folder.CreateFileAsync(tempFileName, CreationCollisionOption.ReplaceExisting);
                });

                this.DispatcherQueue?.TryEnqueue(async () =>
                {
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
