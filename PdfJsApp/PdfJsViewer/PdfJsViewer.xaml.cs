using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using System;
using Windows.UI;
using WvToWinRtHostObjectLib;

namespace PdfJsViewer
{
    public sealed partial class PdfJsViewer : UserControl
    {
        private bool _isWindowInit = false;
        private PdfJsEvents _pdfJsEvents;

        public PdfJsViewer()
        {
            this.InitializeComponent();

            this.Loaded += PdfJsViewer_Loaded;

            _pdfJsEvents = new PdfJsEvents();
        }

        private void PdfJsViewer_Loaded(object sender, RoutedEventArgs e)
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

        private void PreviewBrowser_CoreWebView2Initialized(WebView2 sender, CoreWebView2InitializedEventArgs args)
        {
            // transparent background when loading the page
            PreviewBrowser.DefaultBackgroundColor = Color.FromArgb(0, 0, 0, 0);

            try
            {
                // Setup the host objects to do the native-to-web projection and enable access
                var dispatchAdapter = new WvToWinRtAdapter.DispatchAdapter();

                sender.CoreWebView2.AddHostObjectToScript(
                    "pdfEvents",
                    dispatchAdapter.WrapObject(_pdfJsEvents, dispatchAdapter));

                string siteHostName = "pdfViewer.example";
                string siteFolderMappped = "PdfJsViewer/pdf.js";

                // Set the folder mapping and enable full permission with CORS to use cross-origin resource sharing
                PreviewBrowser.CoreWebView2.SetVirtualHostNameToFolderMapping(
                    siteHostName,
                    siteFolderMappped,
                    Microsoft.Web.WebView2.Core.CoreWebView2HostResourceAccessKind.Allow);

                PreviewBrowser.Source = new Uri("https://" + siteHostName + "/web/viewer.html");

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
            catch (Exception)
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

            // Ensure the host objecgt has also updated the data from the web-side
            var lastCopiedText = _pdfJsEvents.LastCopiedText;
        }

        private void PreviewBrowser_NavigationCompleted(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationCompletedEventArgs args)
        {
            // empty
        }

        private void PreviewBrowser_NavigationStarting(WebView2 sender, Microsoft.Web.WebView2.Core.CoreWebView2NavigationStartingEventArgs args)
        {
            // empty
        }
    }
}
