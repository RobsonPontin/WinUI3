// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

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
using WinRtComponent;

namespace WinUI3_GenCsProjection
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        MyWinRtClass m_myWinrtClass;

        public MainWindow()
        {
            this.InitializeComponent();
        }

        private void myButton_Click(object sender, RoutedEventArgs e)
        {
            if (m_myWinrtClass == null)
            {
                m_myWinrtClass = new MyWinRtClass();
                m_myWinrtClass.CollectionChanged += M_myWinrtClass_CollectionChanged; ;
            }

            m_myWinrtClass.TestCollection();
        }

        private async void M_myWinrtClass_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            var newItems = e.NewItems;

            try
            {
                var count = newItems.Count;
            }
            catch (Exception ex)
            {
                // exception
            }
        }
    }
}
