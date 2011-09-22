using System;
using System.EnterpriseServices.Internal;
using System.Runtime.InteropServices;
using Microsoft.Win32;
using System.Text;


namespace OVUtil
{
    
    
    public class Program
    {
        [DllImport("Imm32.dll", EntryPoint = "ImmInstallIME")]
        public static extern IntPtr ImmInstallIMEA(string image, string name);

        static void Main(string[] args) 
        {

            Publish p = new Publish();

            if (args.Length > 0)
            {
                if (args[0] == "gac" || args[0] == "install")
                {
                    p.GacInstall(args[1]);
                }
                else if (args[0] == "ungac" || args[0] == "uninstall")
                {
                    p.GacRemove(args[1]);
                }
                else if (args[0] == "registerime")
                {
                    ImmInstallIMEA(Environment.GetFolderPath(Environment.SpecialFolder.System) + "\\OVIME.IME", "OpenVanilla 0.9b");
                    Console.WriteLine(" OVIME registered ");
                }
                else if (args[0] == "unregisterime")
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

                }
            }
            else
            {
                Console.WriteLine("Available command gac, ungac, registerime, unregisterime");
            }
        }
    }
}
