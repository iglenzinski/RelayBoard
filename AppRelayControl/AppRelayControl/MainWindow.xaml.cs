using System.IO.Ports;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Ribbon.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;

namespace AppRelayControl
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort? Port;
        List<Button> ButtonList = [];

        const int RELAY_NUM_PART = 0;
        const int RELAY_NAME_PART = 1;


        public MainWindow()
        {
            InitializeComponent();
            cboComPort.ItemsSource = SerialPort.GetPortNames();
            List<string> FileLines = new List<string>(); ;

            if (File.Exists("RelayDefinitions.csv"))
            {
                FileLines = [.. File.ReadAllLines("RelayDefinitions.csv")];
            }

            for (int i = 1; i <= 16; i++)
            {
                string[]? LineParts = null;
                foreach (string Line in FileLines)
                {
                    LineParts = Line.Split(',');
                    if (int.Parse(LineParts[RELAY_NUM_PART]) == i) { break; }
                }

                StackPanel tmpStackPanel = new()
                {
                    Orientation = Orientation.Vertical,
                    Margin = new Thickness(5)
                };
                Grid.SetColumn(tmpStackPanel, (i > 8) ? 0 : 1);
                Grid.SetRow(tmpStackPanel, (i > 8) ? Math.Abs(9 - i) : Math.Abs(8 - i));

                Label tmpLabel = new()
                {
                    Content = "Relay " + i.ToString()
                };

                if (LineParts != null)
                { 
                    tmpLabel.Content = LineParts[RELAY_NAME_PART] + "(" + tmpLabel.Content + ")";
                }

                Button tmpButton = new()
                {
                    Content = "OFF",
                    Tag = i.ToString(),
                    Background = Brushes.LightGray,
                    IsEnabled = false
                };
                tmpButton.Click += RelayButton_Click;
                ButtonList.Add(tmpButton);

                tmpStackPanel.Children.Add(tmpLabel);
                tmpStackPanel.Children.Add(tmpButton);

                ButtonGrid.Children.Add(tmpStackPanel);
            }
        }

        private void btnStartCom_Click(object sender, RoutedEventArgs e)
        {
            if (cboComPort.SelectedIndex >= 0)
            {
                Port = new SerialPort(cboComPort.Text, 115200, Parity.None);
                try
                {
                    Port.Open();
                    foreach (Button Btn in ButtonList)
                    {
                        Btn.IsEnabled = true;
                        Port?.Write([4], 0, 1);
                    }
                }
                catch (Exception Ex)
                {
                    MessageBox.Show("ERROR - " + Ex.Message);
                }

            }
            else
            {
                MessageBox.Show("Must select a COM port!");
            }
        }

        private void RelayButton_Click(object sender, RoutedEventArgs e)
        {
            if (sender is Button ClickedButton)
            {
                string? ButtonTag = ClickedButton.Tag.ToString();
                byte? RelayNum = ButtonTag != null ? byte.Parse(ButtonTag) : null;
                if (RelayNum != null) { Port?.Write([1, RelayNum.Value], 0, 2); }

                string? Value = ClickedButton.Content.ToString();
                if (Value != null)
                {
                    if (Value == "OFF")
                    {
                        ClickedButton.Content = "ON";
                        ClickedButton.Background = Brushes.LightGreen;
                    }
                    else
                    {
                        ClickedButton.Content = "OFF";
                        ClickedButton.Background = Brushes.LightGray;
                    }
                }
            }
        }

        ~MainWindow()
        {
            Port?.Close();
        }

        private void cboComPort_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(cboComPort.SelectedIndex >= 0)
            {
                btnStartCom.IsEnabled = true;
            }
        }
    }
}