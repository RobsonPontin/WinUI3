using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;

using Microsoft.UI.Xaml;

using pdftron;
using pdftron.PDF;

namespace WinUI3_WinAppSDK
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        PDFViewCtrl mPdfViewer;

        public MainWindow()
        {
            this.InitializeComponent();

            PDFNet.Initialize(); // initialize SDK in demo mode

            // Create a PDF Viewer control and add it to a parent UIElement (in this case a Border)
            mPdfViewer = new PDFViewCtrl();
            pdfViewHost.Child = mPdfViewer;
        }

        private void myButton_Click(object sender, RoutedEventArgs e)
        {
            var path = AppDomain.CurrentDomain.BaseDirectory;
            path = Path.Combine(path, @"Resources\GettingStarted.pdf");

            var doc = PDFDoc.CreateFromFilePath(path);
            mPdfViewer.SetDoc(doc);
        }
    }
}
