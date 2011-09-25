namespace OVPreferences
{
    partial class PanelTibetan
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelTibetan));
            this.panel1 = new System.Windows.Forms.Panel();
            this.m_lbKeyboardLayout = new System.Windows.Forms.Label();
            this.m_cbKeyboardLayout = new System.Windows.Forms.ComboBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.m_cbKeyboardLayout);
            this.panel1.Controls.Add(this.m_lbKeyboardLayout);
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Name = "panel1";
            // 
            // m_lbKeyboardLayout
            // 
            resources.ApplyResources(this.m_lbKeyboardLayout, "m_lbKeyboardLayout");
            this.m_lbKeyboardLayout.Name = "m_lbKeyboardLayout";
            // 
            // m_cbKeyboardLayout
            // 
            resources.ApplyResources(this.m_cbKeyboardLayout, "m_cbKeyboardLayout");
            this.m_cbKeyboardLayout.FormattingEnabled = true;
            this.m_cbKeyboardLayout.Items.AddRange(new object[] {
            resources.GetString("m_cbKeyboardLayout.Items"),
            resources.GetString("m_cbKeyboardLayout.Items1"),
            resources.GetString("m_cbKeyboardLayout.Items2"),
            resources.GetString("m_cbKeyboardLayout.Items3")});
            this.m_cbKeyboardLayout.Name = "m_cbKeyboardLayout";
            this.m_cbKeyboardLayout.SelectedIndexChanged += new System.EventHandler(this.m_cbKeyboardLayout_SelectedIndexChanged);
            // 
            // PanelTibetan
            // 
            this.Controls.Add(this.panel1);
            resources.ApplyResources(this, "$this");
            this.Name = "PanelTibetan";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label m_lbKeyboardLayout;
        private System.Windows.Forms.ComboBox m_cbKeyboardLayout;
    }
}
