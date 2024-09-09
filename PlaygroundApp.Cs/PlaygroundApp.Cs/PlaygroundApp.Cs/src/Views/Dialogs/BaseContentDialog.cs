using Microsoft.UI.Xaml.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PlaygroundApp.Views.Dialogs
{
    public partial class BaseContentDialog : ContentDialog
    {
        public BaseContentDialog() 
        {
            DefaultButton = ContentDialogButton.Primary;
        }
    }
}
