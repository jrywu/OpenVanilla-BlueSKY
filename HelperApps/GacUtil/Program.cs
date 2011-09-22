using System;
using System.EnterpriseServices.Internal;
using System.Runtime.InteropServices;



namespace GacUtil
{
    
    
    public class Program
    {
        [DllImport("Imm32.dll", EntryPoint = "ImmInstallIME")]
        public static extern IntPtr ImmInstallIMEA(string image, string name);

        static void Main(string[] args) 
        {

            Publish p = new Publish();

            if(args[0] == "install")
            {
                p.GacInstall(args[1]);
            }
            else if (args[0] == "uninstall")
            {                
                p.GacRemove(args[1]);
            }
            else if (args[0] == "registerime")
            {
                ImmInstallIMEA(Environment.GetFolderPath(Environment.SpecialFolder.System)+"\\OVIME.IME", "OpenVanilla 0.9b");
            }
        }
    }
}
