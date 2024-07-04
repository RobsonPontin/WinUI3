using Microsoft.UI.Xaml;
using Microsoft.Graph;
using Microsoft.Graph.Beta.Models;
using Microsoft.Graph.Beta.Models.ODataErrors;
using Windows.Media.Protection.PlayReady;

namespace TrimmedApp
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
            myButton.Content = "Clicked";

            var graphServiceClient = new GraphClientOptions();
            graphServiceClient.GraphServiceTargetVersion = "beta";
        }
    }
}
