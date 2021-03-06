using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Resources;

namespace OVUIServer
{
	/// <summary>
	/// Summary description for IMEStatusForm.
	/// </summary>
	public class IMEStatusForm : System.Windows.Forms.Form
	{
        private static IMEStatusForm m_instance=null;
        private const int MA_ACTIVATE = 1;
        private const int MA_ACTIVATEANDEAT = 2;
        private const int MA_NOACTIVATE = 0x0003;
        private const int MA_NOACTIVATEANDEAT = 0x0004;        
        private IntPtr m_AppHWnd;

        //@warning OV-specific paths here!
        private static string m_baseDir =
            "C:" +
            //Environment.GetEnvironmentVariable("WINDIR") +
            System.IO.Path.DirectorySeparatorChar +
            "OpenVanilla";
        private static string m_moduleDir =
            m_baseDir +
            System.IO.Path.DirectorySeparatorChar +
            "Modules";
        private static string m_userDir =
            Environment.GetFolderPath(
                Environment.SpecialFolder.ApplicationData) +
            System.IO.Path.DirectorySeparatorChar +
            "OpenVanilla";
        private static string m_confingPath =
            m_userDir +
            System.IO.Path.DirectorySeparatorChar +
            "config.xml";
        
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container m_components = null;
        private IMEButton m_btMenu;
        private IMEButton m_btZhEn;
        private IMEButton m_btHantHans;
        private IMEButton m_btPref;
        private IMEButton m_btHalfFull;
        private IMEButton m_btSymbol;
        private ContextMenu m_cmModule;
        private Panel m_pnHead;
		private Panel m_pnBody;
        private bool m_isHanji = true;
        private bool m_halfFull = false;
        private bool m_docked = false;

        private OVAbout m_aboutForm = null;
        //private IMESymbolForm m_symbolForm = null;
        //private IMEFilterForm m_symbolForm = null;
        private IMEButton m_btDocking;
        private System.Windows.Forms.Timer m_timer;
        private IContainer components;

        private ToolTip tt= new ToolTip();
        
		public IMEStatusForm()
		{
			//
			// Required for Windows Form Designer support
			//
            InitializeComponent();
			this.SetStyle(ControlStyles.StandardClick, false);
			this.SetStyle(ControlStyles.EnableNotifyMessage, false);
			this.SetStyle(ControlStyles.UserMouse, false);

            this.SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer, true);
            Application.EnableVisualStyles();
            
            tt.AutoPopDelay = 5000;
            tt.InitialDelay = 1000;
            tt.ReshowDelay = 500;
            tt.ShowAlways = true;
            tt.SetToolTip(this.m_btMenu, "IM");
            tt.SetToolTip(this.m_btHalfFull, "半型/全形 - Shift+Space");
            tt.SetToolTip(this.m_btZhEn, "中文/英文 - Shift");


            

        }
        public IMEStatusForm open()
        {
            if (m_instance==null)
            {
                m_instance = new IMEStatusForm();
            }
            return m_instance;
        }

        #region IME-specific overridden methods
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                //System.Diagnostics.Debug.WriteLine(
                //    "Form: CreateParams.ExStyle (before) =" + cp.ExStyle);
                cp.ExStyle |= 0x00000004; //<WS_EX_NOPARENTNOTIFY
                cp.ExStyle |= 0x80; //<WS_EX_TOOLWINDOW to hide from alt-tab
                //System.Diagnostics.Debug.WriteLine(
                //    "Form: CreateParams.ExStyle (after) =" + cp.ExStyle);
                return cp;
            }
        }

		protected override void WndProc(ref Message m) 
		{
            /*
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Form[" + msgCounter + "]: (before base) Msg->\t" +
                Enum.GetName(typeof(UtilFuncs.WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");
            */

            base.WndProc(ref m);

            /*
            msgCounter++;
            System.Diagnostics.Debug.WriteLine(
                "Form[" + msgCounter + "]: (after base) Msg->\t" +
                Enum.GetName(typeof(UtilFuncs.WindowsMessage), m.Msg) +
                "(0x" + m.Msg.ToString("X") + ")");
             */

            /*
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_PARENTNOTIFY)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_PARENTNOTIFY! ==");
            }
            else*/
            if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_MOUSEACTIVATE)
            {
                //System.Diagnostics.Debug.WriteLine("== Damn WM_MOUSEACTIVATE! ==");

                //System.Diagnostics.Debug.WriteLine(
                //    "Form[" + msgCounter + "]: Result (before)->\t0x" + m.Result.ToString("X"));

                //m.Result = (IntPtr)MA_NOACTIVATEANDEAT;
                m.Result = (IntPtr)MA_NOACTIVATE;

                //System.Diagnostics.Debug.WriteLine(
                //    "Form[" + msgCounter + "]: Result (after)->\t0x" + m.Result.ToString("X"));
                
            }
            /*
            else if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_NCACTIVATE)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_NCACTIVATE! ==");

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: LParam->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (before)->\t0x" + m.WParam.ToString("X"));
                m.WParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (after)->\t0x" + m.WParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (before)->\t0x" + m.Result.ToString("X"));
                m.Result = new IntPtr(1);
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (after)->\t0x" + m.Result.ToString("X"));
            }
            else if (m.Msg == (Int32)UtilFuncs.WindowsMessage.WM_ACTIVATE)
            {
                System.Diagnostics.Debug.WriteLine("== Damn WM_ACTIVATE! ==");
                //ShowNoActive();
                /*
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: LParam (before)->\t0x" + m.LParam.ToString("X"));
                m.LParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: LParam (after)->\t0x" + m.LParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (before)->\t0x" + m.WParam.ToString("X"));
                m.WParam = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: WParam (after)->\t0x" + m.WParam.ToString("X"));

                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (before)->\t0x" + m.Result.ToString("X"));
                m.Result = IntPtr.Zero;
                System.Diagnostics.Debug.WriteLine(
                    "Form[" + msgCounter + "]: Result (after)->\t0x" + m.Result.ToString("X"));
            }
             */
        }
        #endregion

        /// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				if (m_components != null)
				{
					m_components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            this.m_cmModule = new System.Windows.Forms.ContextMenu();
            this.m_pnHead = new System.Windows.Forms.Panel();
            this.m_pnBody = new System.Windows.Forms.Panel();
            this.m_btMenu = new OVUIServer.IMEButton();
            this.m_btZhEn = new OVUIServer.IMEButton();
            this.m_btPref = new OVUIServer.IMEButton();
            this.m_btHalfFull = new OVUIServer.IMEButton();
            this.m_timer = new System.Windows.Forms.Timer(this.components);
            this.m_btDocking = new OVUIServer.IMEButton();
            this.m_btHantHans = new OVUIServer.IMEButton();
            this.m_btSymbol = new OVUIServer.IMEButton();
            this.m_pnBody.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_pnHead
            // 
            this.m_pnHead.BackColor = System.Drawing.Color.DimGray;
            this.m_pnHead.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_pnHead.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.m_pnHead.Location = new System.Drawing.Point(0, 0);
            this.m_pnHead.Margin = new System.Windows.Forms.Padding(0);
            this.m_pnHead.Name = "m_pnHead";
            this.m_pnHead.Size = new System.Drawing.Size(8, 28);
            this.m_pnHead.TabIndex = 1;
            this.m_pnHead.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.m_pnHead.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.m_pnHead.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            // 
            // m_pnBody
            // 
            this.m_pnBody.AutoSize = true;
            this.m_pnBody.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.m_pnBody.BackColor = System.Drawing.SystemColors.ControlDark;
            this.m_pnBody.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.m_pnBody.CausesValidation = false;
            this.m_pnBody.Controls.Add(this.m_btMenu);
            this.m_pnBody.Controls.Add(this.m_btZhEn);
            this.m_pnBody.Controls.Add(this.m_btPref);
            this.m_pnBody.Controls.Add(this.m_btHalfFull);
            this.m_pnBody.Location = new System.Drawing.Point(8, 0);
            this.m_pnBody.Margin = new System.Windows.Forms.Padding(0);
            this.m_pnBody.Name = "m_pnBody";
            this.m_pnBody.Size = new System.Drawing.Size(185, 28);
            this.m_pnBody.TabIndex = 0;
            // 
            // m_btMenu
            // 
            this.m_btMenu.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.m_btMenu.BackColor = System.Drawing.Color.Transparent;
            this.m_btMenu.ContextMenu = this.m_cmModule;
            this.m_btMenu.Font = new System.Drawing.Font("MS PGothic", 12F);
            this.m_btMenu.ForeColor = System.Drawing.Color.White;
            this.m_btMenu.Location = new System.Drawing.Point(0, 0);
            this.m_btMenu.Margin = new System.Windows.Forms.Padding(0);
            this.m_btMenu.Name = "m_btMenu";
            this.m_btMenu.Size = new System.Drawing.Size(26, 26);
            this.m_btMenu.TabIndex = 0;
            this.m_btMenu.UseVisualStyleBackColor = false;
            this.m_btMenu.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonMenu_MouseUp);
            // 
            // m_btZhEn
            // 
            this.m_btZhEn.BackColor = System.Drawing.Color.Transparent;
            this.m_btZhEn.Font = new System.Drawing.Font("MS PGothic", 12F);
            this.m_btZhEn.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btZhEn.Location = new System.Drawing.Point(24, 0);
            this.m_btZhEn.Margin = new System.Windows.Forms.Padding(0);
            this.m_btZhEn.Name = "m_btZhEn";
            this.m_btZhEn.Size = new System.Drawing.Size(26, 26);
            this.m_btZhEn.TabIndex = 2;
            this.m_btZhEn.Text = "字";
            this.m_btZhEn.UseVisualStyleBackColor = false;
            this.m_btZhEn.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonZhEn_MouseUp);
            // 
            // m_btPref
            // 
            this.m_btPref.BackColor = System.Drawing.Color.Transparent;
            this.m_btPref.Font = new System.Drawing.Font("MS PGothic", 12F);
            this.m_btPref.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btPref.Location = new System.Drawing.Point(139, 0);
            this.m_btPref.Margin = new System.Windows.Forms.Padding(0);
            this.m_btPref.Name = "m_btPref";
            this.m_btPref.Size = new System.Drawing.Size(44, 26);
            this.m_btPref.TabIndex = 3;
            this.m_btPref.Text = "設定";
            this.m_btPref.UseVisualStyleBackColor = false;
            this.m_btPref.Visible = false;
            this.m_btPref.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonPref_MouseUp);
            // 
            // m_btHalfFull
            // 
            this.m_btHalfFull.BackColor = System.Drawing.Color.Transparent;
            this.m_btHalfFull.Font = new System.Drawing.Font("PMingLiU", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_btHalfFull.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btHalfFull.Location = new System.Drawing.Point(48, 0);
            this.m_btHalfFull.Margin = new System.Windows.Forms.Padding(0);
            this.m_btHalfFull.Name = "m_btHalfFull";
            this.m_btHalfFull.Size = new System.Drawing.Size(26, 26);
            this.m_btHalfFull.TabIndex = 3;
            this.m_btHalfFull.Text = "▌";
            this.m_btHalfFull.UseVisualStyleBackColor = false;
            this.m_btHalfFull.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonHalfFull_MouseUp);
            // 
            // m_timer
            // 
            this.m_timer.Enabled = true;
            this.m_timer.Interval = 200;
            this.m_timer.Tick += new System.EventHandler(this.m_timer_Tick);
            // 
            // m_btDocking
            // 
            this.m_btDocking.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.m_btDocking.BackColor = System.Drawing.Color.Transparent;
            this.m_btDocking.ContextMenu = this.m_cmModule;
            this.m_btDocking.Font = new System.Drawing.Font("Times New Roman", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.m_btDocking.ForeColor = System.Drawing.Color.White;
            this.m_btDocking.Location = new System.Drawing.Point(1, 2);
            this.m_btDocking.Margin = new System.Windows.Forms.Padding(0);
            this.m_btDocking.Name = "m_btDocking";
            this.m_btDocking.Size = new System.Drawing.Size(8, 10);
            this.m_btDocking.TabIndex = 2;
            this.m_btDocking.Text = "▬";
            this.m_btDocking.UseVisualStyleBackColor = false;
            this.m_btDocking.Visible = false;
            this.m_btDocking.MouseUp += new System.Windows.Forms.MouseEventHandler(this.m_btDocking_MouseUp);
            // 
            // m_btHantHans
            // 
            this.m_btHantHans.BackColor = System.Drawing.Color.Transparent;
            this.m_btHantHans.ForeColor = System.Drawing.Color.White;
            this.m_btHantHans.Location = new System.Drawing.Point(154, 0);
            this.m_btHantHans.Margin = new System.Windows.Forms.Padding(0);
            this.m_btHantHans.Name = "m_btHantHans";
            this.m_btHantHans.Size = new System.Drawing.Size(34, 34);
            this.m_btHantHans.TabIndex = 1;
            this.m_btHantHans.UseVisualStyleBackColor = false;
            this.m_btHantHans.Visible = false;
            this.m_btHantHans.MouseUp += new System.Windows.Forms.MouseEventHandler(this.buttonHantHans_MouseUp);
            // 
            // m_btSymbol
            // 
            this.m_btSymbol.BackColor = System.Drawing.Color.Transparent;
            this.m_btSymbol.ForeColor = System.Drawing.SystemColors.WindowText;
            this.m_btSymbol.Location = new System.Drawing.Point(188, 0);
            this.m_btSymbol.Margin = new System.Windows.Forms.Padding(0);
            this.m_btSymbol.Name = "m_btSymbol";
            this.m_btSymbol.Size = new System.Drawing.Size(34, 34);
            this.m_btSymbol.TabIndex = 4;
            this.m_btSymbol.Text = "€";
            this.m_btSymbol.UseVisualStyleBackColor = false;
            this.m_btSymbol.Visible = false;
            // 
            // IMEStatusForm
            // 
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.Color.Gray;
            this.ClientSize = new System.Drawing.Size(85, 28);
            this.ControlBox = false;
            this.Controls.Add(this.m_btDocking);
            this.Controls.Add(this.m_pnHead);
            this.Controls.Add(this.m_pnBody);
            this.ForeColor = System.Drawing.Color.Transparent;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.Name = "IMEStatusForm";
            this.ShowInTaskbar = false;
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseMove);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.IMEStatusForm_MouseUp);
            this.m_pnBody.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

        
		#endregion

        #region public methods
        public void SetAppHWnd(IntPtr HWND)
        {
            m_AppHWnd = HWND;
            Debug.WriteLine("m_AppHWnd (status) =" + HWND);
        }

        public IntPtr GetHandle()
        {
            return this.Handle;
        }
        public bool getIsDocked()
        {
            return this.m_docked;
        }

        public void setIsDocked(bool t)
        {
      /*      BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetIsDocked(t);
            worker.RunWorkerAsync();

        }
        private void doSetIsDocked(bool t)
        {*/
            if (t)
            {

                this.Invoke((MethodInvoker)delegate { m_btDocking.Text = "□"; });
                m_docked = true;
            }
            else
            {

                this.Invoke((MethodInvoker)delegate { m_btDocking.Text = "▬"; });
                m_docked = false;
            }
        }

		public void ShowNoActive()
		{
            if (!this.Visible)
            {
                //showDeskBand(true);
                
                UtilFuncs.SetVisibleNoActivate(this, true); // true to show. 
                

            }

		}

        public void HideNoActive()
		{
            if (this.Visible)
            {
                
                //showDeskBand(false);
                UtilFuncs.SetVisibleNoActivate(this, false); // false to hide.  
            }
		}

        public void SetLocation(int x, int y)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetLocation(x, y);
            worker.RunWorkerAsync();

        }
        private void doSetLocation(int x, int y)
		{*/
            this.Invoke((MethodInvoker)delegate { Location = new Point(x, y); });
            m_docked = false;
            this.Invoke((MethodInvoker)delegate { m_pnHead.Cursor = System.Windows.Forms.Cursors.SizeAll; });
            
		}
        public void DockedLocation(int x, int y)
        {
            this.Invoke((MethodInvoker)delegate { Location = new Point(x, y); });
            m_docked = true;
            this.Invoke((MethodInvoker)delegate { m_pnHead.Cursor = System.Windows.Forms.Cursors.Default; });

        }

        public void SetHalfFull()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetHalfFull();
            worker.RunWorkerAsync();

        }
        private void doSetHalfFull()
        {*/
            m_halfFull = !m_halfFull;
            if (m_halfFull)
            {
                this.Invoke((MethodInvoker)delegate {m_cmModule.MenuItems[imCount + kpCount + 2].Checked = true; });
                this.Invoke((MethodInvoker)delegate {m_btHalfFull.Text = "█"; });
                //showDeskBand(true);
            }
            else
            {
                this.Invoke((MethodInvoker)delegate {m_cmModule.MenuItems[imCount + kpCount + 2].Checked = false; });
                this.Invoke((MethodInvoker)delegate { m_btHalfFull.Text = "▌"; });
                //showDeskBand(false);
            }

        }

        public void SetHalf()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetHalf();
            worker.RunWorkerAsync();

        }
        private void doSetHalf()
        {*/
            m_halfFull = false;
            this.Invoke((MethodInvoker)delegate {m_cmModule.MenuItems[imCount + kpCount + 2].Checked = false;});
            this.Invoke((MethodInvoker)delegate {m_btHalfFull.Text = "▌";});
            
        }
        public void SetFull()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetFull();
            worker.RunWorkerAsync();
        }
        private void doSetFull()
        {*/
            m_halfFull = true;
            this.Invoke((MethodInvoker)delegate {m_cmModule.MenuItems[imCount + kpCount + 2].Checked = true;});
            this.Invoke((MethodInvoker)delegate { m_btHalfFull.Text = "█"; });

        }

		public void SetChiEng()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetCHiEng();
            worker.RunWorkerAsync();

        }
        private void doSetCHiEng()
		{*/
            m_isHanji = !m_isHanji;
            if (m_isHanji)
                this.Invoke((MethodInvoker)delegate {m_btZhEn.Text = "字";});
			else
                this.Invoke((MethodInvoker)delegate { m_btZhEn.Text = "A"; });
		}
        public void SetChinese()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetChinese();
            worker.RunWorkerAsync();

        }
        private void doSetChinese()
        {*/
            m_isHanji = true;
            this.Invoke((MethodInvoker)delegate {m_btZhEn.Text = "字"; });
        }

        public void SetEnglish()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetEnglish();
            worker.RunWorkerAsync();

        }
        private void doSetEnglish()
        {*/
            m_isHanji = false;
            this.Invoke((MethodInvoker)delegate{ m_btZhEn.Text = "A"; });
        }

        public void SetSimpifiedOrTraditional(bool isTraditional)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetSimpifiedOrTraditional(isTraditional);
            worker.RunWorkerAsync();

        }
        private void doSetSimpifiedOrTraditional(bool isTraditional)
        {*/
            if (isTraditional)
                this.Invoke((MethodInvoker)delegate { m_btHantHans.Text = "繁"; });
            else
                this.Invoke((MethodInvoker)delegate { m_btHantHans.Text = "簡"; });
        }

        public void SetModString(int index)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetModString(index);
            worker.RunWorkerAsync();

        }
        public void doSetModString(int index)
        {*/
            if (index > imCount) return;
            
            this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[m_selectedModuleIndex].RadioCheck = false; });
            this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[m_selectedModuleIndex].Checked = false; });
            m_selectedModuleIndex = index;
            this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[m_selectedModuleIndex].RadioCheck = true; });
            this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[m_selectedModuleIndex].Checked = true; });
            
            tt.SetToolTip(m_btMenu, this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.ToString());
            if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("倉頡"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text="頡";});
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("許氏"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "許";});
            else if(this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("注音"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text="ㄅ";});
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("英數"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "英";});
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("符號"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "符";});            
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("大易"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "易";});
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("行列"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "行";});
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("拼音"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "行";});
            else if (this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Contains("五筆"))
                this.Invoke((MethodInvoker)delegate{m_btMenu.Text = "五";});

            else
            {
                //this.m_btMenu.AutoSize = true;
                this.Invoke((MethodInvoker)delegate { m_btMenu.Text = this.m_cmModule.MenuItems[m_selectedModuleIndex].Text.Substring(0, 1); });
            }
            ResizeAll();            
        }

        
        public void ClearMenuModString()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doClearMenuModString();
            worker.RunWorkerAsync();
        }
        private void doClearMenuModString()
        {*/
            this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems.Clear(); });
        }


        public void SetMenuModString(string inputs)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetMenuModString(inputs);
            worker.RunWorkerAsync();
        }
        private void doSetMenuModString(string inputs)
		{*/
			if(inputs == null) return;


            if (this.m_cmModule.MenuItems.Count == 0)
            {
                this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems.Add("-");});
                this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems.Add("-");});
                this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems.Add("-");});
                this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems.Add(
                 "偏好設定 ...", new EventHandler(menuItem_Click));});
                this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems.Add(
                 "安裝通用表格", new EventHandler(menuItem_Click));});
                this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems.Add(
                 "關於 OpenVanilla", new EventHandler(menuItem_Click));
                });
            }

            int imOffset = 6;
            int kpOffset = imOffset - 1;
            int ofOffset = kpOffset - 1;
            
            System.Diagnostics.Debug.WriteLine("\tstatus: set menu=\t" + inputs);
            
            MenuItem mi = new MenuItem(inputs, new EventHandler(menuItem_Click));
            if (this.m_cmModule.MenuItems.Count - imOffset < imCount)
            {
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems.Add(this.m_cmModule.MenuItems.Count - imOffset, mi); });// IM at top
                //if (this.m_btMenu.Text == "") SetModString(0);       // First IM
                //this.m_cmModule.MenuItems[0].RadioCheck = true;
                //this.m_cmModule.MenuItems[0].Checked = true;
            }
            else if (this.m_cmModule.MenuItems.Count - imOffset < imCount + kpCount)// KP at middle
            {
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems.Add(this.m_cmModule.MenuItems.Count - kpOffset, mi); });
            }
            else  //OF at bottom
            {
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems.Add(this.m_cmModule.MenuItems.Count - ofOffset, mi); });

                if (m_halfFull) // restore OVOFFullwidth checked status
                    this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[imCount + kpCount + ofOffset].Checked = true; });
                else
                    this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[imCount + kpCount + ofOffset].Checked = false; });
                
            }
                     
                
        }

        public void SetDir(string baseDir, string moduleDir , string userDir)
		{
            m_baseDir = baseDir;
            m_moduleDir = moduleDir;
            m_userDir = userDir;
            Debug.WriteLine(m_baseDir);
            Debug.WriteLine(m_moduleDir);
            Debug.WriteLine(m_userDir);
        }

        public int GetSelectedModuleIndex()
        {
            return m_selectedModuleIndex;// +kpCount;
        }

        public void RotateModule()
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doRotateModule();
            worker.RunWorkerAsync();
        }
        private void doRotateModule()
        {*/
            this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems[m_selectedModuleIndex].RadioCheck = false;});
            this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems[m_selectedModuleIndex].Checked = false;});
            m_selectedModuleIndex++;
            
            if (m_selectedModuleIndex == imCount)
                m_selectedModuleIndex = 0;
            this.Invoke((MethodInvoker)delegate{m_btMenu.Text =
                m_cmModule.MenuItems[m_selectedModuleIndex].Text;});

            this.Invoke((MethodInvoker)delegate{m_cmModule.MenuItems[m_selectedModuleIndex].RadioCheck = true;});
            this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[m_selectedModuleIndex].Checked = true; });
        }

        public bool getItemChecked(int index)
        {
            if ((kpCount + ofCount==0)||(index > kpCount + ofCount)) return true;
            if(index < kpCount) 
                return this.m_cmModule.MenuItems[index + imCount + 1].Checked;  //KP modules
            else
                return this.m_cmModule.MenuItems[index + imCount + 2].Checked;  //OF modules
        }

        public void setItemChecked(int index, bool c)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetItemCheck(index, c);
            worker.RunWorkerAsync();

        }
        private void doSetItemCheck(int index, bool c)
        {*/
            if ((kpCount + ofCount == 0) || (index > kpCount + ofCount)) return;
            if (index < kpCount)
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[index + imCount + 1].Checked = c; });  //KP modules
            else
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[index + imCount + 2].Checked = c; });  //OF modules

        }

        public void setItemEnabled(int index, bool c)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetItemEnabled(index, c);
            worker.RunWorkerAsync();
        }
        private void doSetItemEnabled(int index, bool c)
        {*/
            if ((kpCount + ofCount == 0) || (index > kpCount + ofCount)) return;
         
            if (index < kpCount)
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[index + imCount + 1].Enabled = c; });  //KP modules
            else
                this.Invoke((MethodInvoker)delegate { m_cmModule.MenuItems[index + imCount + 2].Enabled = c; });  //OF modules

        }


        #endregion

        #region private memebers

        private Point mouseOffset;
		private bool isMouseDown = false;
        private int m_selectedModuleIndex = 0;
        private int kpCount=0;
        private int ofCount=0;
        private int imCount = 0;

        public void setkpCount(int count)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetkpCount(count);
            worker.RunWorkerAsync();
        }
        private void doSetkpCount(int count)
        {*/
            System.Diagnostics.Debug.WriteLine("\tm_selectidx:"+m_selectedModuleIndex+";kpcount:"+count);
            kpCount = count;
          
        }

        public void setofCount(int count)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetofCount(count);
            worker.RunWorkerAsync();
        }
        private void doSetofCount(int count)
        {*/
            System.Diagnostics.Debug.WriteLine("\tofcount:" + count);
            ofCount = count;
        }
        public void setimCount(int count)
        {/*
            BackgroundWorker worker = new BackgroundWorker();
            worker.DoWork += (obj, e) => doSetimCount(count);
            worker.RunWorkerAsync();
        }
        private void doSetimCount(int count)
        {*/
            System.Diagnostics.Debug.WriteLine("\timcount:" + count);
            imCount = count;
        }

        private void menuItem_Click(object sender, EventArgs e)
        {
            if (((MenuItem)sender).Index == this.m_cmModule.MenuItems.Count - 1)
            {
                m_aboutForm = new OVAbout();
                m_aboutForm.Show(this);
            }
            else if (((MenuItem)sender).Index == this.m_cmModule.MenuItems.Count - 2)
            {

                //Create a new instance of the OpenFileDialog because it's an object.
                OpenFileDialog dialog = new OpenFileDialog();
                //Now set the file type
                dialog.Filter = "cin files (*.cin)|*.cin";
                //Set the starting directory and the title.
                dialog.InitialDirectory = Environment.GetFolderPath (Environment.SpecialFolder.MyDocuments);
                dialog.Multiselect = true;
                dialog.Title = "Select a .cin file";
                //Present to the user.
                if (dialog.ShowDialog() == DialogResult.OK && dialog.FileNames.Length>0)
                {
                    foreach (String filename in dialog.FileNames)
                    {
                        String targetFileName =  m_userDir 
                            + Path.DirectorySeparatorChar + "OVIMGeneric" 
                            + Path.DirectorySeparatorChar + Path.GetFileName(filename);
                        //MessageBox.Show(" source file:" + filename + "\ntarget file:" + targetFileName);
                        File.Copy(filename, targetFileName,true);
                    }
                }
                    
            }
            else if (((MenuItem)sender).Index == this.m_cmModule.MenuItems.Count-3)
            {
                callovpreference();
            }
            else if (((MenuItem)sender).Index < imCount )
            {
                //this.m_cmModule.MenuItems.IndexOf(((MenuItem)sender).Index)
                this.m_cmModule.MenuItems[m_selectedModuleIndex].RadioCheck = false;
                this.m_cmModule.MenuItems[m_selectedModuleIndex].Checked = false;
                m_selectedModuleIndex = ((MenuItem)sender).Index;
                this.m_cmModule.MenuItems[m_selectedModuleIndex].RadioCheck = true;
                this.m_cmModule.MenuItems[m_selectedModuleIndex].Checked = true;
                //System.Diagnostics.Debug.WriteLine("Menu[xxx]: Send message to handle " +
                //    m_AppHWnd.ToString("X"));
                int ret = UtilFuncs.SendMessage(
                    m_AppHWnd,
                    (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                    0xE, //IMN_PRIVATE
                    3);  //Switch modules by Mouse
                //System.Diagnostics.Debug.WriteLine("Menu[xxx]: Message ret=" + ret);
                //System.Diagnostics.Debug.WriteLine("Menu[xxx]: GetLastError=" + UtilFuncs.GetLastError());
            }
            else if(((MenuItem)sender).Index == imCount+kpCount+2) // OVFullwidth
            {
                //this.m_cmModule.MenuItems[imCount + kpCount + 2].Checked =
                //    !this.m_cmModule.MenuItems[imCount + kpCount + 2].Checked;
                SendKeys.SendWait("+ ");
            }
            else // KP OF checked status updated.
            {
                this.m_cmModule.MenuItems[((MenuItem)sender).Index].Checked = 
                    !this.m_cmModule.MenuItems[((MenuItem)sender).Index].Checked;
                int ret = UtilFuncs.SendMessage(
                    m_AppHWnd,
                    (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                    0xE, //IMN_PRIVATE
                    13);
            }
        }

		private void IMEStatusForm_MouseDown(object sender,System.Windows.Forms.MouseEventArgs e)
		{
			int xOffset;
			int yOffset;
			if (e.Button == MouseButtons.Left && !m_docked)
			{
				xOffset = -e.X - SystemInformation.FrameBorderSize.Width;
				yOffset = -e.Y;
				mouseOffset = new Point(xOffset, yOffset);
				isMouseDown = true;
			}
		}

		private void IMEStatusForm_MouseMove(object sender,
			System.Windows.Forms.MouseEventArgs e)
		{
			if (isMouseDown && !m_docked)
			{				
				Point mousePos = Control.MousePosition;
				mousePos.Offset(mouseOffset.X, mouseOffset.Y);
				Location = mousePos;
			}
		}

		private void IMEStatusForm_MouseUp(object sender,
			System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left && !m_docked)
			{
				isMouseDown = false;
                // location changed. Notify loader to save new location
                int ret = UtilFuncs.SendMessage(
                        m_AppHWnd,
                        (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                        0xE, //IMN_PRIVATE
                        11);
            }
		}
            

        private void buttonMenu_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.Bottom < Screen.PrimaryScreen.Bounds.Height / 2)
                this.m_btMenu.ContextMenu.Show(m_btMenu, new System.Drawing.Point(0, m_btMenu.Bottom));
            else
                this.m_btMenu.ContextMenu.Show(m_btMenu, new System.Drawing.Point(0, m_btMenu.Top));
            //System.Diagnostics.Debug.WriteLine("status: context menu shown.");
        }

        private void buttonZhEn_MouseUp(object sender, MouseEventArgs e) //中英
        {
            Debug.WriteLine("中英-> m_AppHWnd=" + m_AppHWnd);
            //            showDeskBand(true);
            SendKeys.SendWait("+"); //send shift
            //comment below because handing over the job to modules
            /* int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                2);*/
        }

        private void buttonHantHans_MouseUp(object sender, MouseEventArgs e) //繁簡
        {
            //Debug.WriteLine("繁簡-> m_AppHWnd=" + m_AppHWnd);
            int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                4);
        }
        private  void buttonHalfFull_MouseUp(object sender, MouseEventArgs e) // Full/half shape
        {
            
            //SetHalfFull();
            //this.m_cmModule.MenuItems[imCount + kpCount + 2].Checked =
            //        !this.m_cmModule.MenuItems[imCount + kpCount + 2].Checked;
            SendKeys.SendWait("+ "); //send shift+space
            /*int ret = UtilFuncs.SendMessage(
                new IntPtr((long)m_AppHWnd),
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                12);*/
            
        }

        void m_btDocking_MouseUp(object sender, MouseEventArgs e)
        {
            int ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE
                14);
          
        }
        



        private void proc_Exited(object sender, EventArgs e) //
        {
            //System.Threading.Thread.Sleep(1000);
            int ret;
            
            ret = UtilFuncs.SendMessage(m_AppHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                   0, 0);

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_CREATE,
                0, 0);

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0x0002, //IMN_OPENSTATUSWINDOW
                0);

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_RELOADCONFIG,
                0, 0);
        }

        private void MessageBoxDefaultButton()
        {
            throw new Exception("The method or operation is not implemented.");
        } 

        private void buttonPref_MouseUp(object sender, MouseEventArgs e) //設定
        {
            callovpreference();
        }

        private void callovpreference()
        {

            int ret;
            /*
            ret    = UtilFuncs.SendMessage(m_AppHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                   0, 0);

            /*
            Process proc = new System.Diagnostics.Process();
            proc.StartInfo.FileName =
                m_baseDir + System.IO.Path.DirectorySeparatorChar +
                "OVPreferences.exe";
            proc.StartInfo.WorkingDirectory = m_baseDir;
            System.IO.FileInfo configFile =
                new System.IO.FileInfo(m_confingPath);

          /*  int ret = UtilFuncs.SendMessage(m_AppHWnd,
                   (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                   0, 0);
            
            //proc.Exited += new EventHandler(proc_Exited);           
            //int ret;
       
            proc.Start();
            proc.WaitForExit();
            */



            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0x01, //IMN_CLOSESTATUSWINDOW
                10);



            // Private 10 sends WM_RELOAD_CONFIG to all OVIMEUI instances
            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0xE, //IMN_PRIVATE 
                10); // Call OVpreference and send reload to all OV instances after process ended.


            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_SETCONTEXT,// .WM_IME_NOTIFY,
                0x01, //
                0x0f);
            
            /*

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_CREATE,
                0, 0);

            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_NOTIFY,
                0x0002, //IMN_OPENSTATUSWINDOW
                0);
            
            ret = UtilFuncs.SendMessage(
                m_AppHWnd,
                (uint)UtilFuncs.WindowsMessage.WM_IME_RELOADCONFIG,
                0, 0);
             */
    }

        /*
       private void buttonSymbol_MouseUp(object sender, MouseEventArgs e) //符號表
       {            
          int ret = UtilFuncs.SendMessage(m_AppHWnd,
                  (uint)UtilFuncs.WindowsMessage.WM_DESTROY,
                  0, 0);
           if (m_symbolForm == null)
           {
               //m_symbolForm = new IMESymbolForm();
               m_symbolForm = new IMEFilterForm();
                
               string[] fakeInput = new string[4];
               fakeInput[0] = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
               fakeInput[1] = "1 2 3 4 5 6 7";
               fakeInput[2] = "ㄅ ㄆ ㄇ ㄈ";
               fakeInput[3] = "阿 ㄇ ㄌ";
               m_symbolForm.SetSymbols(fakeInput);
               //m_symbolForm.SetSymbols("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.10 11.11 12.1/2");                                                
               m_symbolForm.SetLocation(800, 600);
               m_symbolForm.ShowNoActive();
               
               string[] fakeInput = new string[4];
               fakeInput[0] = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
               fakeInput[1] = "1 2 3 4 5 6 7";
               fakeInput[2] = "ㄅ ㄆ ㄇ ㄈ";
               fakeInput[3] = "阿 ㄇ ㄌ";
               m_symbolForm.SetPossibleList("ㄗㄢˇ ㄗㄤ");//ㄓㄢ ㄓㄢˊ 
               m_symbolForm.SetSymbols(fakeInput);
               //m_symbolForm.SetSymbols("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.10 11.11 12.1/2");                                                
               // m_symbolForm.SetLocation(800, 600);
               m_symbolForm.ShowNoActive();
           }
           else
           {
               if (m_symbolForm.IsDisposed)
               {
                   m_symbolForm = null;
                   m_symbolForm = new IMESymbolForm();6
                   string[] fakeInput = new string[4];
                   fakeInput[0] = "， 。 、 ； ！ ？ ﹛ 『 （ 【 ＃ ％ ＆ ＊ ※ ○ ◎ □ ㊣ ＋ ＊ ※ ○ ◎ ＜ ＞ ＝ ＝ ＄ ￥ ㏄ € ℃ ㎜ ㎞ ㎝ ㎏ 】 ） 』 ﹜";
                   fakeInput[1] = "1 2 3 4 5 6 7";
                   fakeInput[2] = "ㄅ ㄆ ㄇ ㄈ";
                   fakeInput[3] = "阿 ㄇ ㄌ";
                   m_symbolForm.SetSymbols(fakeInput);
                   //m_symbolForm.SetSymbols("1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8 9.9 10.10 11.11 12.1/2");                                                
                   m_symbolForm.SetLocation(800, 600);
                   m_symbolForm.ShowNoActive();
               }
               else
               {
                   m_symbolForm.Dispose();
                   m_symbolForm = null;
               }
           }
       }
       */
        private void ResizeAll()
        {
            Debug.WriteLine("ReSize");
            this.m_btDocking.Location = new Point(1, 1);
            this.m_btMenu.Location = new Point(0, 0);
            //this.m_btMenu.Size = new Size(this.m_btMenu.Height * this.m_btMenu.Text.Length, this.m_btMenu.Height);
            this.m_btZhEn.Location = new Point(this.m_btMenu.Bounds.Right, 0);
            //this.m_btPref.Location = new Point(this.m_btZhEn.Bounds.Right, 0);
            this.m_btHalfFull.Location = new Point(this.m_btZhEn.Bounds.Right, 0);

            this.m_pnBody.Width =
                this.m_btMenu.Width +
                (this.m_btHantHans.Visible ? this.m_btHantHans.Width : 0) +
                this.m_btZhEn.Width + this.m_btHalfFull.Width +
                (this.m_btSymbol.Visible ? this.m_btSymbol.Width : 0);
            this.Width = this.m_pnBody.Width + this.m_pnHead.Width + 2;
        }


        /*
        IBandSite spBandsite=null;
        OVDeskBand spBand = null;
        private UInt32 deskBandID;
        private void showDeskBand(bool t)
        {
            if (spBandsite==null)
            {
                try
                {
                //cocreateinstatnce on traybandservice
                OVUIServer.trayBandservice spBandService = new OVUIServer.trayBandservice();
                //query interface of Ibandsite
                spBandsite = (OVUIServer.IBandSite)spBandService;
                }
                catch (Exception ex)
                {
                    spBandsite = null;
                    System.Diagnostics.Debug.WriteLine("COM Exception" + ex.Message);
                }
            }
            if (spBandsite!=null && spBand==null)
            {
                try
                {
                //cocreateinstance on OvDeskband
                spBand = new OVUIServer.OVDeskBand();
                // query interface of IObjectwithsite
                OVUIServer.IObjectWithSite spSite = (OVUIServer.IObjectWithSite)spBand;
                spSite.SetSite(spBandsite);
                }
                catch (Exception ex)
                {
                    spBand = null;
                    System.Diagnostics.Debug.WriteLine("COM Exception" + ex.Message);
                }

            }

            if (spBand!=null && spBandsite!=null)
            {
                if (t)
                {
                    try
                    {   
                        deskBandID = spBandsite.AddBand(spBand);
                        System.Diagnostics.Debug.WriteLine("DeskbandID:" + deskBandID.ToString());
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine("COM Exception" + ex.Message);
                        spBand = null;
                        spBandsite = null;
                    }
                }
                else
                {
                    try
                    {
                        System.Diagnostics.Debug.WriteLine("DeskbandID:" + deskBandID.ToString());
                        spBandsite.RemoveBand(deskBandID);
                        spBand = null;
                        spBandsite = null;
                        
                    }
                    catch (Exception ex)
                    {
                        System.Diagnostics.Debug.WriteLine("COM Exception" + ex.Message);
                        spBand = null;
                        spBandsite = null;
                    }

                }

                    
            
                
            }
        }
        */
        #endregion

        private void m_timer_Tick(object sender, EventArgs e)
        {
            if (this.Visible)
            {

                UtilFuncs.SetVisibleNoActivate(this, true);
            }
        }

        

        
     
    }

    #region Deprecated MessageFilter implementation
    /*
    public class MessageFilter : IMessageFilter
    {
        public bool PreFilterMessage(ref Message objMessage)
        {
            System.Diagnostics.Debug.WriteLine("PreFilterMessage");
            if (objMessage.Msg == Convert.ToInt32(UtilFuncs.WindowsMessages.WM_MOUSEACTIVATE))
            {
                System.Diagnostics.Debug.WriteLine("WM_MOUSEACTIVATE");
                return true;
            }
            return false;
        }
    }
    */
    #endregion
}


