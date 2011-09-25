namespace OVPreferences
{
    partial class PanelArray
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelArray));
            this.panel1 = new System.Windows.Forms.Panel();
            this.m_cbLearnAssociatedPhrase = new System.Windows.Forms.CheckBox();
            this.m_cbOrderWordsByFreq = new System.Windows.Forms.CheckBox();
            this.m_cbAssociatedPhrase = new System.Windows.Forms.CheckBox();
            this.m_cbQuickMode = new System.Windows.Forms.CheckBox();
            this.m_cbSpecialCode = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.m_cbLearnAssociatedPhrase);
            this.panel1.Controls.Add(this.m_cbOrderWordsByFreq);
            this.panel1.Controls.Add(this.m_cbAssociatedPhrase);
            this.panel1.Controls.Add(this.m_cbQuickMode);
            this.panel1.Controls.Add(this.m_cbSpecialCode);
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Name = "panel1";
            // 
            // m_cbLearnAssociatedPhrase
            // 
            resources.ApplyResources(this.m_cbLearnAssociatedPhrase, "m_cbLearnAssociatedPhrase");
            this.m_cbLearnAssociatedPhrase.Name = "m_cbLearnAssociatedPhrase";
            this.m_cbLearnAssociatedPhrase.UseVisualStyleBackColor = true;
            this.m_cbLearnAssociatedPhrase.CheckedChanged += new System.EventHandler(this.m_cbLearnAssociatePhrase_CheckedChanged);
            // 
            // m_cbOrderWordsByFreq
            // 
            resources.ApplyResources(this.m_cbOrderWordsByFreq, "m_cbOrderWordsByFreq");
            this.m_cbOrderWordsByFreq.Name = "m_cbOrderWordsByFreq";
            this.m_cbOrderWordsByFreq.UseVisualStyleBackColor = true;
            this.m_cbOrderWordsByFreq.CheckedChanged += new System.EventHandler(this.m_cbOrderWordsByFreq_CheckedChanged);
            // 
            // m_cbAssociatedPhrase
            // 
            resources.ApplyResources(this.m_cbAssociatedPhrase, "m_cbAssociatedPhrase");
            this.m_cbAssociatedPhrase.Name = "m_cbAssociatedPhrase";
            this.m_cbAssociatedPhrase.UseVisualStyleBackColor = true;
            this.m_cbAssociatedPhrase.CheckedChanged += new System.EventHandler(this.m_cbAssociatedPhrase_CheckedChanged);
            // 
            // m_cbQuickMode
            // 
            resources.ApplyResources(this.m_cbQuickMode, "m_cbQuickMode");
            this.m_cbQuickMode.Name = "m_cbQuickMode";
            this.m_cbQuickMode.UseVisualStyleBackColor = true;
            this.m_cbQuickMode.CheckedChanged += new System.EventHandler(this.m_cbQuickMode_CheckedChanged);
            // 
            // m_cbSpecialCode
            // 
            resources.ApplyResources(this.m_cbSpecialCode, "m_cbSpecialCode");
            this.m_cbSpecialCode.Name = "m_cbSpecialCode";
            this.m_cbSpecialCode.UseVisualStyleBackColor = true;
            this.m_cbSpecialCode.CheckedChanged += new System.EventHandler(this.m_cbSpecialCode_CheckedChanged);
            // 
            // PanelArray
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PanelArray";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.CheckBox m_cbQuickMode;
        private System.Windows.Forms.CheckBox m_cbSpecialCode;
        private System.Windows.Forms.CheckBox m_cbLearnAssociatedPhrase;
        private System.Windows.Forms.CheckBox m_cbOrderWordsByFreq;
        private System.Windows.Forms.CheckBox m_cbAssociatedPhrase;
    }
}
