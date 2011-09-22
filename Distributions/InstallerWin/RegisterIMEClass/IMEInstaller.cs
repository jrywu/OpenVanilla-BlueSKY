using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using System.Text;


namespace RegisterIMEClass
{
    [RunInstaller(true)]
    public partial class IMEInstaller : System.Configuration.Install.Installer
    {
        [DllImport("Imm32.dll", EntryPoint = "ImmInstallIME")]
        public static extern IntPtr ImmInstallIMEA(string image, string name);

        public IMEInstaller()
        {
            InitializeComponent();
        }
        public override void Install(System.Collections.IDictionary stateSaver)
        {
            ImmInstallIMEA(Environment.GetFolderPath(Environment.SpecialFolder.System) + "\\OVIME.IME", "OpenVanilla 0.9b");
            base.Install(stateSaver);
        }
        public override void Uninstall(System.Collections.IDictionary stateSaver)
        {
            RegistryKey HKLM = Registry.LocalMachine;
            RegistryKey HKUSER = Registry.CurrentUser;

            RegistryKey IMEKey = HKLM.OpenSubKey(
                "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts", true);
            RegistryKey IMEUserKey = HKUSER.OpenSubKey(
                "Keyboard Layout\\Preload", true);

            string[] subKeys = IMEKey.GetSubKeyNames();
            string OVIMEKey = "";

            foreach (string curKey in subKeys)
            {
                RegistryKey rk = IMEKey.OpenSubKey(curKey, false);
                if (rk != null)
                {
                    String imeName = (String)rk.GetValue("IME File");
                    if (imeName == null)
                        imeName = "";
                    //else 
                    //    Console.WriteLine(curKey + ":IME Name: " + imeName);

                    if (imeName.Equals("OVIME.IME"))
                    {
                        OVIMEKey = curKey;
                        Console.WriteLine(" OVIME Regiester as " + OVIMEKey);
                        IMEKey.DeleteSubKeyTree(curKey);
                        Console.WriteLine(" OVIME unregistered ");
                    }
                }
            }
            String[] valueNames = IMEUserKey.GetValueNames();
            foreach (string curValueName in valueNames)
            {
                String keyLayout = (String)IMEUserKey.GetValue(curValueName);
                if (keyLayout.ToUpper().Equals(OVIMEKey.ToUpper()))
                {
                    Console.WriteLine(" OVIME Preload found at " + curValueName);
                    IMEUserKey.DeleteValue(curValueName);
                    Console.WriteLine(" OVIME user preload settings removed");
                }
                //else
                //    Console.WriteLine(" Preload keyboard : " + curValueName);
            } 
            base.Uninstall(stateSaver);
        }
    }
}
