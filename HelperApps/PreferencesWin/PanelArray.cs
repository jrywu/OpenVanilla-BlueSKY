using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OVPreferences
{
    public partial class PanelArray : UserControl
    {
        private OVConfig m_ovConf = null;
        private XmlDocument m_ovConfDOM = null;
        private OVConfigDom m_ovConfDomHandler = null;

        public PanelArray()
        {
            InitializeComponent();
        }

        public virtual void Init(OVConfig conf, XmlDocument confDOM)
        {
            m_ovConf = conf;
            m_ovConfDOM = confDOM;
            m_ovConfDomHandler = new OVConfigDom(m_ovConfDOM);

            LoadSettings();
        }

        protected virtual void LoadSettings()
        {
            checkBox1.Checked=
                Convert.ToBoolean(Convert.ToInt32(
                   m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "specialCode", "1")))
                || Convert.ToBoolean(Convert.ToInt32(
                   m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "特別碼提示", "1")));
            checkBox2.Checked =
                Convert.ToBoolean(Convert.ToInt32(
                   m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "quickMode", "0")))
                 || Convert.ToBoolean(Convert.ToInt32(
                   m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "快打模式", "1")));

        }
            
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            m_ovConfDomHandler.SetAttribute(
               m_ovConf.moduleName,
               "specialCode",
               checkBox1.Checked ? "1" : "0");
            m_ovConfDomHandler.SetAttribute(
              m_ovConf.moduleName,
              "特別碼提示",
              checkBox1.Checked ? "1" : "0");
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            m_ovConfDomHandler.SetAttribute(
               m_ovConf.moduleName,
               "quickMode",
               checkBox2.Checked ? "1" : "0");
            m_ovConfDomHandler.SetAttribute(
              m_ovConf.moduleName,
              "快打模式",
              checkBox1.Checked ? "1" : "0");
        }
    }
}
