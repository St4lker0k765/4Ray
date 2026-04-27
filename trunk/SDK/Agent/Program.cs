using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace uAgent
{
    internal class Program
    {
        [DllImport("kernel32.dll")]
        private static extern int LoadLibrary(string FileName);

        [DllImport("kernel32.dll")]
        private static extern bool FreeLibrary(int hModule);

        [DllImport("uGrid.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern bool StartupAgent(bool safe);

        private static void Main()
        {
            if (Program.LoadLibrary("uGrid.dll") == 0)
            {
                MessageBox.Show("ugrid agent requires uGrid.dll", "Error");
            }
            if (!Program.StartupAgent(true))
            {
                return;
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new AgentForm());
        }
    }
}
