// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Windowing;
using Microsoft.UI.Xaml;
using Microsoft.UI.Dispatching;
using System.Diagnostics;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace WinUI3App_cs
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public partial class App : Application
    {
        private Window m_window;
        private AppWindow m_appWindow;
        private DispatcherQueue m_dispatcherQueue;

        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Invoked when the application is launched.
        /// </summary>
        /// <param name="args">Details about the launch request and process.</param>
        protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            m_window = new MainWindow();
            m_window.Closed += Window_Closed;

            m_dispatcherQueue = m_window.DispatcherQueue;
            m_dispatcherQueue.ShutdownStarting += DispatcherQueue_ShutdownStarting;
            m_dispatcherQueue.ShutdownCompleted += DispatcherQueue_ShutdownCompleted;

            var hWnd = WinRT.Interop.WindowNative.GetWindowHandle(m_window);
            // Retrieve the WindowId that corresponds to hWnd.
            Microsoft.UI.WindowId windowId =
                Microsoft.UI.Win32Interop.GetWindowIdFromWindow(hWnd);

            m_appWindow = AppWindow.GetFromWindowId(windowId);
            m_appWindow.Closing += AppWindow_Closing;
            m_appWindow.Destroying += AppWindow_Destroying;

            m_window.Activate();
        }

        /*
         * A. Shutting down with Window.Close()
         *
         * 1. Window.Close()
         * 2. Window.Closed 
         * 3. AppWindow.Destroying
         * 4. Process terminated.
         * 
         * B. Shutting down with AppWindow.Destroy() and then Window.Close()
         * 
         * 1. AppWindow.Destroy()
         * 2. AppWindow.Destroying
         * 3. Window.Close() - called inside the Destroying event handler
         * 4. Window.Closed
         * 5. AppWindow.Destroying
         * 6. Process terminated.
         * 
         * C. Shutting down with Window.Close() and AppWindow.Destroy()
         * 
         * 1. Window.Close()
         * 2. Window.Closed
         * 3. AppWindow.Destroying
         * 4. AppWindow.Destroy() - called inside the AppWindow.Destroying event handler
         * 5. AppWindow.Destroying... - this event gets called a few times in a loop until process terminates.
         * 6. Process NOT terminated.
         * 
         * NOTE: calling AppWindow.Destroy() inside Window.Closed event does not cause 
         * multiple triggers of AppWindow.Destroying, but the process still get's stuck and never terminated.
         */

        private void AppWindow_Destroying(AppWindow sender, object args)
        {
            Debug.WriteLine("[Debug] AppWindow_Destroying()");
        }

        private void AppWindow_Closing(AppWindow sender, AppWindowClosingEventArgs args)
        {
            Debug.WriteLine("[Debug] AppWindow_Closing()");
        }

        private void DispatcherQueue_ShutdownCompleted(DispatcherQueue sender, object args)
        {
            Debug.WriteLine("[Debug] DispatcherQueue_ShutdownCompleted()");
        }

        private void DispatcherQueue_ShutdownStarting(DispatcherQueue sender, DispatcherQueueShutdownStartingEventArgs args)
        {
            Debug.WriteLine("[Debug] DispatcherQueue_ShutdownStarting()");
        }

        private void Window_Closed(object sender, WindowEventArgs args)
        {
            Debug.WriteLine("[Debug] Window_Closed()");
        }
    }
}
