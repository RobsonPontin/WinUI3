using Microsoft.UI.Dispatching;
using Microsoft.UI.Xaml;
using System;
using System.Threading;

namespace ContainerForCs.AppCs
{
    public static class Program
    {
        // Replaces the standard App.g.i.cs.
        // Note: We can't declare Main to be async because in a WinUI app
        // this prevents Narrator from reading XAML elements.
        [STAThread]
        private static void Main(string[] args)
        {
            WinRT.ComWrappersSupport.InitializeComWrappers();

            Application.Start((p) =>
            {
                var dq = DispatcherQueue.GetForCurrentThread();
                var context = new DispatcherQueueSynchronizationContext(dq);
                SynchronizationContext.SetSynchronizationContext(context);

                ContainerForCs.AppLib.AppFactory.CreateApp();
            });
        }
    }
}
