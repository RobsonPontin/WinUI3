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
using Windows.Storage;
using Windows.UI;


namespace PdfJsApp
{
    public sealed partial class MainWindow : Window
    {
        private bool _isWindowInit = false;

        public MainWindow()
        {
            this.InitializeComponent();

            this.Activated += MainWindow_Activated;
        }

        private void MainWindow_Activated(object sender, WindowActivatedEventArgs args)
        {
            if (_isWindowInit)
                return;

            _isWindowInit = true;

            try
            {
                // Avoid re-entrancy fail fast issue by enqueuing the call
                this.DispatcherQueue.TryEnqueue(async () =>
                {
                    await PreviewBrowser.EnsureCoreWebView2Async();
                });
            }
            catch (Exception)
            {
                // todo: error log
            }
        }

        private async void PreviewBrowser_CoreWebView2Initialized(WebView2 sender, CoreWebView2InitializedEventArgs args)
        {
            // transparent background when loading the page
            PreviewBrowser.DefaultBackgroundColor = Color.FromArgb(0, 0, 0, 0);

            // NOTE: settings to keep in mind, so far no need to tweak them
            //PreviewBrowser.CoreWebView2.Settings.IsWebMessageEnabled = true;
            //PreviewBrowser.CoreWebView2.Settings.IsScriptEnabled = true;
            //PreviewBrowser.CoreWebView2.Settings.AreDefaultScriptDialogsEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.AreDefaultContextMenusEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.AreDevToolsEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.AreHostObjectsAllowed = false;
            //PreviewBrowser.CoreWebView2.Settings.IsGeneralAutofillEnabled = false;
            //PreviewBrowser.CoreWebView2.Settings.IsPasswordAutosaveEnabled = false;   

            PreviewBrowser.CoreWebView2.Settings.HiddenPdfToolbarItems =
                Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.ZoomIn
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.ZoomOut
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Search
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.PageLayout
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.PageSelector
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Bookmarks
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.FitPage
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.FullScreen
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Rotate
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.SaveAs
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Print
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.Save
                | Microsoft.Web.WebView2.Core.CoreWebView2PdfToolbarItems.MoreSettings;

            try
            {
                // Set the folder mapping and enable full permission with CORS to use cross-origin resource sharing
                PreviewBrowser.CoreWebView2.SetVirtualHostNameToFolderMapping(
                    "pdfViewer.example",
                    "pdf.js",
                    Microsoft.Web.WebView2.Core.CoreWebView2HostResourceAccessKind.Allow);

                PreviewBrowser.Source = new Uri("https://pdfViewer.example/web/viewer.html");

                /* NOTE: Uncomment the following line if you would like to debug JavaScript code
                 * 
                 * To make sure the console for debugging Edge works properly:
                 * 1. Once the apps runs, make sure it navigates to the "MainPage.html" above
                 * 2. In the Edge debug console the web assets should be green (online circle)
                 * 3. Open the web files in the console (not in the VS) and add breakpoints
                 * 
                 * Note that if the code navigates away from the "MainPage.html" above it might cause
                 * the web assets to be unloaded (orange color) and breakpoints won't work.
                 */
                //PreviewBrowser.CoreWebView2.OpenDevToolsWindow();
            }
            catch (Exception ex)
            {
                // todo:
            }
        }

        private void PreviewBrowser_WebMessageReceived(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2WebMessageReceivedEventArgs args)
        {
            /* 
             * In the "Viewer.js" I added an event to notify webview when a text copy event has been raised
             * which will build a json object, example: "{"CopyText":["text/plain","this is the text copied..."]}"
             */
            var msg = args.WebMessageAsJson;
        }

        private async void PreviewBrowser_NavigationCompleted(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationCompletedEventArgs args)
        {
            // empty
        }
    }
}
