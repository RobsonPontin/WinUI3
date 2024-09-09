
using System.Diagnostics;

namespace PlaygroundApp.Views.Dialogs
{
    public sealed partial class CustomContentDialog : BaseContentDialog
    {
        public CustomContentDialog()
        {
            this.InitializeComponent();

            this.Unloaded += CustomContentDialog_Unloaded;
        }

        private void CustomContentDialog_Unloaded(object sender, Microsoft.UI.Xaml.RoutedEventArgs e)
        {
            if (this.IsLoaded)
            {
                Debug.Assert(false, nameof(CustomContentDialog) + ": Unloaded event raised during an unexpected time");
            }
        }
    }
}
