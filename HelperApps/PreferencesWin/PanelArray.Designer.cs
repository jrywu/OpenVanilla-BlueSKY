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
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.m_cbLearnAssociatedPhrase = new System.Windows.Forms.CheckBox();
            this.m_cbOrderWordsByFreq = new System.Windows.Forms.CheckBox();
            this.m_cbAssociatedPhrase = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.m_cbLearnAssociatedPhrase);
            this.panel1.Controls.Add(this.m_cbOrderWordsByFreq);
            this.panel1.Controls.Add(this.m_cbAssociatedPhrase);
            this.panel1.Controls.Add(this.checkBox2);
            this.panel1.Controls.Add(this.checkBox1);
            resources.ApplyResources(this.panel1, "panel1");
            this.panel1.Name = "panel1";
            // 
            // checkBox2
            // 
            resources.ApplyResources(this.checkBox2, "checkBox2");
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.UseVisualStyleBackColor = true;
            this.checkBox2.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // checkBox1
            // 
            resources.ApplyResources(this.checkBox1, "checkBox1");
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // m_cbLearnAssociatedPhrase
            // 
            resources.ApplyResources(this.m_cbLearnAssociatedPhrase, "m_cbLearnAssociatedPhrase");
            this.m_cbLearnAssociatedPhrase.Name = "m_cbLearnAssociatedPhrase";
            this.m_cbLearnAssociatedPhrase.UseVisualStyleBackColor = true;
            // 
            // m_cbOrderWordsByFreq
            // 
            resources.ApplyResources(this.m_cbOrderWordsByFreq, "m_cbOrderWordsByFreq");
            this.m_cbOrderWordsByFreq.Name = "m_cbOrderWordsByFreq";
            this.m_cbOrderWordsByFreq.UseVisualStyleBackColor = true;
            // 
            // m_cbAssociatedPhrase
            // 
            resources.ApplyResources(this.m_cbAssociatedPhrase, "m_cbAssociatedPhrase");
            this.m_cbAssociatedPhrase.Name = "m_cbAssociatedPhrase";
            this.m_cbAssociatedPhrase.UseVisualStyleBackColor = true;
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
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox m_cbLearnAssociatedPhrase;
        private System.Windows.Forms.CheckBox m_cbOrderWordsByFreq;
        private System.Windows.Forms.CheckBox m_cbAssociatedPhrase;
    }
}
