using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Media.Animation;
using System.Windows.Threading;
using System.Threading;
using System.Xml;
namespace QQMusic
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>

    public partial class MainWindow : NavigationWindow
    {
        public static String config;
        public static String skins;
        public MainWindow()
        {
            try
            {
                config = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Tencent\\QQMusic";
                skins = "未找到！";
                XmlReader rdr = XmlReader.Create(config + "\\Option2.xml");
                while (rdr.Read())
                {
                    if (rdr.Name == "Skins")
                    {
                        skins = rdr.GetAttribute("currentPath");
                        rdr.Close();
                        break;
                    }
                }
                InitializeComponent();
            }
            catch (Exception e)
            {
                MessageBox.Show("打开皮肤文件失败！", "QQ音乐去广告工具", MessageBoxButton.OK, MessageBoxImage.Warning);
                System.Environment.Exit(0);
            }
        }
        private void NavigationWindow_Navigating(object sender, NavigatingCancelEventArgs e)
        {
            if (Content != null && !_allowDirectNavigation)
            {
                e.Cancel = true;
                _navArgs = e;
                this.IsHitTestVisible = false;
                DoubleAnimation da = new DoubleAnimation(0.2d, new Duration(TimeSpan.FromMilliseconds(300)));
                da.Completed += FadeOutCompleted;
                this.BeginAnimation(OpacityProperty, da);
            }
            _allowDirectNavigation = false;
        }

        private void FadeOutCompleted(object sender, EventArgs e)
        {
            (sender as AnimationClock).Completed -= FadeOutCompleted;

            this.IsHitTestVisible = true;

            _allowDirectNavigation = true;
            switch (_navArgs.NavigationMode)
            {
                case NavigationMode.New:
                    if (_navArgs.Uri == null)
                    {
                        NavigationService.Navigate(_navArgs.Content);
                    }
                    else
                    {
                        NavigationService.Navigate(_navArgs.Uri);
                    }
                    break;
                case NavigationMode.Back:
                    NavigationService.GoBack();
                    break;

                case NavigationMode.Forward:
                    NavigationService.GoForward();
                    break;
                case NavigationMode.Refresh:
                    NavigationService.Refresh();
                    break;
            }

            Dispatcher.BeginInvoke(DispatcherPriority.Loaded,
                (ThreadStart)delegate()
                {
                    DoubleAnimation da = new DoubleAnimation(1.0d, new Duration(TimeSpan.FromMilliseconds(200)));
                    this.BeginAnimation(OpacityProperty, da);
                });
        }

        private bool _allowDirectNavigation = false;
        private NavigatingCancelEventArgs _navArgs = null;
        
        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.DragMove();
            //this.WindowState = WindowState.Minimized;
        }

        bool Completed = false;
        public void AnimationCompleted(object sender, EventArgs e)
        {
            Completed = true;
            System.Environment.Exit(0);
        }
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            this.ShowInTaskbar = false;
            this.Opacity = 1;
            DoubleAnimation OpercityAnimation = new DoubleAnimation(1.00, 0.00, new Duration(TimeSpan.FromMilliseconds(600)));
            OpercityAnimation.Completed += AnimationCompleted;
            this.BeginAnimation(Window.OpacityProperty, OpercityAnimation);
            if (!Completed) e.Cancel = true;
            base.OnClosing(e);
        }
    }
}
