namespace OVPreferences
{
    partial class PanelGeneric
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer m_components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (m_components != null))
            {
                m_components.Dispose();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PanelGeneric));
            this.m_pnSelf = new System.Windows.Forms.Panel();
            this.m_nuMaxRadicalLength = new System.Windows.Forms.NumericUpDown();
            this.m_lbWildcardSupport = new System.Windows.Forms.Label();
            this.m_lbTypingPreference = new System.Windows.Forms.Label();
            this.m_cbLearnAssociatedPhrase = new System.Windows.Forms.CheckBox();
            this.m_cbOrderWordsByFreq = new System.Windows.Forms.CheckBox();
            this.m_cbAssociatedPhrase = new System.Windows.Forms.CheckBox();
            this.m_tbselectKey = new System.Windows.Forms.TextBox();
            this.m_tbMatchZeroOrMoreChar = new System.Windows.Forms.TextBox();
            this.m_tbMatchOneChar = new System.Windows.Forms.TextBox();
            this.m_cbWarningBeep = new System.Windows.Forms.CheckBox();
            this.m_cbHitMaxAndCompose = new System.Windows.Forms.CheckBox();
            this.m_cbAutoCompose = new System.Windows.Forms.CheckBox();
            this.m_cbShiftSelectionKey = new System.Windows.Forms.CheckBox();
            this.m_lbselectKey = new System.Windows.Forms.Label();
            this.m_lbMatchZeroOrMoreChar = new System.Windows.Forms.Label();
            this.m_lbMatchOneChar = new System.Windows.Forms.Label();
            this.m_lbMaxRadialHint2 = new System.Windows.Forms.Label();
            this.m_lbMaxRadialHint1 = new System.Windows.Forms.Label();
            this.m_lbBasicSettings = new System.Windows.Forms.Label();
            this.m_lbMaxKeySequenceLength = new System.Windows.Forms.Label();
            this.sep1 = new System.Windows.Forms.Label();
            this.sep2 = new System.Windows.Forms.Label();
            this.m_pnSelf.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_nuMaxRadicalLength)).BeginInit();
            this.SuspendLayout();
            // 
            // m_pnSelf
            // 
            this.m_pnSelf.Controls.Add(this.sep2);
            this.m_pnSelf.Controls.Add(this.sep1);
            this.m_pnSelf.Controls.Add(this.m_nuMaxRadicalLength);
            this.m_pnSelf.Controls.Add(this.m_lbWildcardSupport);
            this.m_pnSelf.Controls.Add(this.m_lbTypingPreference);
            this.m_pnSelf.Controls.Add(this.m_cbLearnAssociatedPhrase);
            this.m_pnSelf.Controls.Add(this.m_cbOrderWordsByFreq);
            this.m_pnSelf.Controls.Add(this.m_cbAssociatedPhrase);
            this.m_pnSelf.Controls.Add(this.m_tbselectKey);
            this.m_pnSelf.Controls.Add(this.m_tbMatchZeroOrMoreChar);
            this.m_pnSelf.Controls.Add(this.m_tbMatchOneChar);
            this.m_pnSelf.Controls.Add(this.m_cbWarningBeep);
            this.m_pnSelf.Controls.Add(this.m_cbHitMaxAndCompose);
            this.m_pnSelf.Controls.Add(this.m_cbAutoCompose);
            this.m_pnSelf.Controls.Add(this.m_cbShiftSelectionKey);
            this.m_pnSelf.Controls.Add(this.m_lbselectKey);
            this.m_pnSelf.Controls.Add(this.m_lbMatchZeroOrMoreChar);
            this.m_pnSelf.Controls.Add(this.m_lbMatchOneChar);
            this.m_pnSelf.Controls.Add(this.m_lbMaxRadialHint2);
            this.m_pnSelf.Controls.Add(this.m_lbMaxRadialHint1);
            this.m_pnSelf.Controls.Add(this.m_lbBasicSettings);
            this.m_pnSelf.Controls.Add(this.m_lbMaxKeySequenceLength);
            resources.ApplyResources(this.m_pnSelf, "m_pnSelf");
            this.m_pnSelf.Name = "m_pnSelf";
            // 
            // m_nuMaxRadicalLength
            // 
            resources.ApplyResources(this.m_nuMaxRadicalLength, "m_nuMaxRadicalLength");
            this.m_nuMaxRadicalLength.Maximum = new decimal(new int[] {
            32,
            0,
            0,
            0});
            this.m_nuMaxRadicalLength.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.m_nuMaxRadicalLength.Name = "m_nuMaxRadicalLength";
            this.m_nuMaxRadicalLength.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.m_nuMaxRadicalLength.ValueChanged += new System.EventHandler(this.m_nuMaxRadicalLength_ValueChanged);
            // 
            // m_lbWildcardSupport
            // 
            resources.ApplyResources(this.m_lbWildcardSupport, "m_lbWildcardSupport");
            this.m_lbWildcardSupport.Name = "m_lbWildcardSupport";
            // 
            // m_lbTypingPreference
            // 
            resources.ApplyResources(this.m_lbTypingPreference, "m_lbTypingPreference");
            this.m_lbTypingPreference.Name = "m_lbTypingPreference";
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
            // m_tbselectKey
            // 
            resources.ApplyResources(this.m_tbselectKey, "m_tbselectKey");
            this.m_tbselectKey.Name = "m_tbselectKey";
            this.m_tbselectKey.TextChanged += new System.EventHandler(this.m_tbselectKey_TextChanged);
            // 
            // m_tbMatchZeroOrMoreChar
            // 
            resources.ApplyResources(this.m_tbMatchZeroOrMoreChar, "m_tbMatchZeroOrMoreChar");
            this.m_tbMatchZeroOrMoreChar.Name = "m_tbMatchZeroOrMoreChar";
            this.m_tbMatchZeroOrMoreChar.TextChanged += new System.EventHandler(this.m_tbMatchZeroOrMoreChar_TextChanged);
            // 
            // m_tbMatchOneChar
            // 
            resources.ApplyResources(this.m_tbMatchOneChar, "m_tbMatchOneChar");
            this.m_tbMatchOneChar.Name = "m_tbMatchOneChar";
            this.m_tbMatchOneChar.TextChanged += new System.EventHandler(this.m_tbMatchOneChar_TextChanged);
            // 
            // m_cbWarningBeep
            // 
            resources.ApplyResources(this.m_cbWarningBeep, "m_cbWarningBeep");
            this.m_cbWarningBeep.Name = "m_cbWarningBeep";
            this.m_cbWarningBeep.UseVisualStyleBackColor = true;
            this.m_cbWarningBeep.CheckedChanged += new System.EventHandler(this.m_cbWarningBeep_CheckedChanged);
            // 
            // m_cbHitMaxAndCompose
            // 
            resources.ApplyResources(this.m_cbHitMaxAndCompose, "m_cbHitMaxAndCompose");
            this.m_cbHitMaxAndCompose.Name = "m_cbHitMaxAndCompose";
            this.m_cbHitMaxAndCompose.UseVisualStyleBackColor = true;
            this.m_cbHitMaxAndCompose.CheckedChanged += new System.EventHandler(this.m_cbHitMaxAndCompose_CheckedChanged);
            // 
            // m_cbAutoCompose
            // 
            resources.ApplyResources(this.m_cbAutoCompose, "m_cbAutoCompose");
            this.m_cbAutoCompose.Name = "m_cbAutoCompose";
            this.m_cbAutoCompose.UseVisualStyleBackColor = true;
            this.m_cbAutoCompose.CheckedChanged += new System.EventHandler(this.m_cbAutoCompose_CheckedChanged);
            // 
            // m_cbShiftSelectionKey
            // 
            resources.ApplyResources(this.m_cbShiftSelectionKey, "m_cbShiftSelectionKey");
            this.m_cbShiftSelectionKey.Name = "m_cbShiftSelectionKey";
            this.m_cbShiftSelectionKey.UseVisualStyleBackColor = true;
            this.m_cbShiftSelectionKey.CheckedChanged += new System.EventHandler(this.m_cbShiftSelectionKey_CheckedChanged);
            // 
            // m_lbselectKey
            // 
            resources.ApplyResources(this.m_lbselectKey, "m_lbselectKey");
            this.m_lbselectKey.Name = "m_lbselectKey";
            // 
            // m_lbMatchZeroOrMoreChar
            // 
            resources.ApplyResources(this.m_lbMatchZeroOrMoreChar, "m_lbMatchZeroOrMoreChar");
            this.m_lbMatchZeroOrMoreChar.Name = "m_lbMatchZeroOrMoreChar";
            // 
            // m_lbMatchOneChar
            // 
            resources.ApplyResources(this.m_lbMatchOneChar, "m_lbMatchOneChar");
            this.m_lbMatchOneChar.Name = "m_lbMatchOneChar";
            // 
            // m_lbMaxRadialHint2
            // 
            resources.ApplyResources(this.m_lbMaxRadialHint2, "m_lbMaxRadialHint2");
            this.m_lbMaxRadialHint2.Name = "m_lbMaxRadialHint2";
            // 
            // m_lbMaxRadialHint1
            // 
            resources.ApplyResources(this.m_lbMaxRadialHint1, "m_lbMaxRadialHint1");
            this.m_lbMaxRadialHint1.Name = "m_lbMaxRadialHint1";
            // 
            // m_lbBasicSettings
            // 
            resources.ApplyResources(this.m_lbBasicSettings, "m_lbBasicSettings");
            this.m_lbBasicSettings.Name = "m_lbBasicSettings";
            // 
            // m_lbMaxKeySequenceLength
            // 
            resources.ApplyResources(this.m_lbMaxKeySequenceLength, "m_lbMaxKeySequenceLength");
            this.m_lbMaxKeySequenceLength.Name = "m_lbMaxKeySequenceLength";
            // 
            // sep1
            // 
            this.sep1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            resources.ApplyResources(this.sep1, "sep1");
            this.sep1.Name = "sep1";
            // 
            // sep2
            // 
            this.sep2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            resources.ApplyResources(this.sep2, "sep2");
            this.sep2.Name = "sep2";
            // 
            // PanelGeneric
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.m_pnSelf);
            this.Name = "PanelGeneric";
            this.m_pnSelf.ResumeLayout(false);
            this.m_pnSelf.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.m_nuMaxRadicalLength)).EndInit();
            this.ResumeLayout(false);

        }


       
        
        
        #endregion

        private System.Windows.Forms.Panel m_pnSelf;
        private System.Windows.Forms.CheckBox m_cbWarningBeep;
        private System.Windows.Forms.CheckBox m_cbHitMaxAndCompose;
        private System.Windows.Forms.CheckBox m_cbAutoCompose;
        private System.Windows.Forms.CheckBox m_cbShiftSelectionKey;
        private System.Windows.Forms.Label m_lbMaxKeySequenceLength;
        private System.Windows.Forms.TextBox m_tbselectKey;
        private System.Windows.Forms.Label m_lbselectKey;
        private System.Windows.Forms.TextBox m_tbMatchZeroOrMoreChar;
        private System.Windows.Forms.TextBox m_tbMatchOneChar;
        private System.Windows.Forms.Label m_lbMatchZeroOrMoreChar;
        private System.Windows.Forms.Label m_lbMatchOneChar;
        private System.Windows.Forms.CheckBox m_cbOrderWordsByFreq;
        private System.Windows.Forms.CheckBox m_cbAssociatedPhrase;
        private System.Windows.Forms.CheckBox m_cbLearnAssociatedPhrase;
        private System.Windows.Forms.Label m_lbBasicSettings;
        private System.Windows.Forms.Label m_lbMaxRadialHint1;
        private System.Windows.Forms.Label m_lbTypingPreference;
        private System.Windows.Forms.Label m_lbMaxRadialHint2;
        private System.Windows.Forms.Label m_lbWildcardSupport;
        private System.Windows.Forms.NumericUpDown m_nuMaxRadicalLength;
        private System.Windows.Forms.Label sep2;
        private System.Windows.Forms.Label sep1;
        
    }
}
