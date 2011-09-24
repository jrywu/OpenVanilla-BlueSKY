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
        private bool m_doAssociatedPhrase;
        private bool m_doOrderWordsByFreq;
        private bool m_doLearnAssociatedPhrase;

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
            checkBox1.Checked =
                Convert.ToBoolean(Convert.ToInt32(
                   m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "specialCode", "1")));
                //|| Convert.ToBoolean(Convert.ToInt32(
                //   m_ovConfDomHandler.GetAttribute(
                //      m_ovConf.moduleName, "特別碼提示", "1")));
            checkBox2.Checked =
                Convert.ToBoolean(Convert.ToInt32(
                   m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "quickMode", "0")));
                 //|| Convert.ToBoolean(Convert.ToInt32(
                 //  m_ovConfDomHandler.GetAttribute(
                 //      m_ovConf.moduleName, "快打模式", "1")));

            m_doAssociatedPhrase =
               Convert.ToBoolean(Convert.ToInt32(
               m_ovConfDomHandler.GetAttribute(
                       m_ovConf.moduleName, "associatedPhrase", "1")));

            m_doLearnAssociatedPhrase =
                Convert.ToBoolean(Convert.ToInt32(
                m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "learnAssociatedPhrase", "1")));

            m_doOrderWordsByFreq =
                Convert.ToBoolean(Convert.ToInt32(
                m_ovConfDomHandler.GetAttribute(
                        m_ovConf.moduleName, "orderWordsByFreq", "0")));

            m_cbAssociatedPhrase.Checked = m_doAssociatedPhrase;
            m_cbOrderWordsByFreq.Checked = m_doOrderWordsByFreq;
            m_cbLearnAssociatedPhrase.Checked = m_doLearnAssociatedPhrase;

        }
            
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            m_ovConfDomHandler.SetAttribute(
               m_ovConf.moduleName,
               "specialCode",
               checkBox1.Checked ? "1" : "0");
            /*m_ovConfDomHandler.SetAttribute(
              m_ovConf.moduleName,
              "特別碼提示",
              checkBox1.Checked ? "1" : "0");
             */
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            m_ovConfDomHandler.SetAttribute(
               m_ovConf.moduleName,
               "quickMode",
               checkBox2.Checked ? "1" : "0");
            /*m_ovConfDomHandler.SetAttribute(
              m_ovConf.moduleName,
              "快打模式",
              checkBox1.Checked ? "1" : "0");
             */
        }
        private void m_cbAssociatedPhrase_CheckedChanged(object sender, EventArgs e)
        {
            m_doAssociatedPhrase = m_cbAssociatedPhrase.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "associatedPhrase",
                m_doAssociatedPhrase ? "1" : "0");

        }


        private void m_cbOrderWordsByFreq_CheckedChanged(object sender, EventArgs e)
        {
            m_doOrderWordsByFreq = m_cbOrderWordsByFreq.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "orderWordsByFreq",
                m_doOrderWordsByFreq ? "1" : "0");

        }

        private void m_cbLearnAssociatePhrase_CheckedChanged(object sender, EventArgs e)
        {
            m_doLearnAssociatedPhrase = m_cbLearnAssociatedPhrase.Checked;
            m_ovConfDomHandler.SetAttribute(
                m_ovConf.moduleName,
                "learnAssociatedPhrase",
                m_doLearnAssociatedPhrase ? "1" : "0");

        }

       
    }
}
