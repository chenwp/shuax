using System;
using System.Collections.Generic;
using System.Linq;
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
using System.Xml;
using System.Collections;
namespace QQMusic
{
    /// <summary>
    /// Page2.xaml 的交互逻辑
    /// </summary>
    public partial class Page2 : Page
    {
        public Page2()
        {
            InitializeComponent();
            XmlDocument xd = new XmlDocument();
            xd.Load(QQMusic.MainWindow.skins+"\\List\\config.xml");
            xd.Save(QQMusic.MainWindow.skins + "\\List\\config.xml.bak");
            XmlNodeList node = xd.GetElementsByTagName("DlgItem");
            int height=0;
            int temp;
            foreach (XmlNode xn in node)
            {
                XmlElement xe = (XmlElement)xn;
                if (xe.GetAttribute("id").Equals("DlgItem_Advertisement"))
                {
                    height = int.Parse(xe.GetAttribute("height"));
                    xe.SetAttribute("height","0");
                    textBlock1.Inlines.Add("广告高度：" + height+" 像素\n");
                }
            }
            foreach (XmlNode xn in node)
            {
                XmlElement xe = (XmlElement)xn;
                if (xe.GetAttribute("id").IndexOf("List")!=-1)
                {
                    temp = height + int.Parse(xe.GetAttribute("height"));
                    xe.SetAttribute("height", ""+temp);
                    textBlock1.Inlines.Add(xe.GetAttribute("id") + " 高度改变为：" + temp + " 像素\n");
                }
            }
            xd.Save(QQMusic.MainWindow.skins + "\\List\\config.xml");
            textBlock1.Inlines.Add("皮肤配置文件修改完成，已经自动备份。");
        }

        private void close_Click(object sender, RoutedEventArgs e)
        {
            App.Current.MainWindow.Close();
        }

        private void image1_MouseDown(object sender, MouseButtonEventArgs e)
        {
            NavigationService.Navigate(new Uri("Page1.xaml", UriKind.Relative));
        }
    }
}
