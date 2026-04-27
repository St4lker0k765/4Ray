using NetFwTypeLib;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;
using System.Security.Policy;
using System.Threading;
using System.Windows.Forms;

namespace uAgent
{
    public class AgentForm : Form
    {
        [DllImport("uGrid.dll")]
        private static extern void ShutdownAgent();

        [DllImport("uGrid.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetAgentStatus(ref AgentForm.AgentInfo info);

        [DllImport("uGrid.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void GetTaskInfo(int task_index, ref AgentForm.TaskInfo info);

        [DllImport("uGrid.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetGridVersion();

        public AgentForm()
        {
            this.InitializeComponent();
            this.AddApplicationToFirewall();
        }
        private void AddApplicationToFirewall()
        {
            string fileName = Process.GetCurrentProcess().MainModule.FileName;
            INetFwMgr netFwMgr = (INetFwMgr)AgentForm.getInstance("INetFwMgr");
            INetFwPolicy localPolicy = netFwMgr.LocalPolicy;
            INetFwProfile currentProfile = localPolicy.CurrentProfile;
            INetFwAuthorizedApplications authorizedApplications = currentProfile.AuthorizedApplications;
            INetFwAuthorizedApplication netFwAuthorizedApplication = (INetFwAuthorizedApplication)AgentForm.getInstance("INetAuthApp");
            netFwAuthorizedApplication.Name = "uService";
            netFwAuthorizedApplication.ProcessImageFileName = fileName;
            authorizedApplications.Add(netFwAuthorizedApplication);
        }
        protected static object getInstance(string typeName)
        {
            if (typeName == "INetFwMgr")
            {
                Type typeFromCLSID = Type.GetTypeFromCLSID(new Guid("{304CE942-6E39-40D8-943A-B913C40C9CD4}"));
                return Activator.CreateInstance(typeFromCLSID);
            }
            if (typeName == "INetAuthApp")
            {
                Type typeFromCLSID2 = Type.GetTypeFromCLSID(new Guid("{EC9846B3-2762-4A6B-A214-6ACB603462D2}"));
                return Activator.CreateInstance(typeFromCLSID2);
            }
            if (typeName == "INetOpenPort")
            {
                Type typeFromCLSID3 = Type.GetTypeFromCLSID(new Guid("{0CA545C6-37AD-4A6C-BF92-9F7610067EF5}"));
                return Activator.CreateInstance(typeFromCLSID3);
            }
            return null;
        }
        private void AgentForm_Load(object sender, EventArgs e)
        {
            base.ShowInTaskbar = false;
            base.WindowState = FormWindowState.Minimized;
            base.Hide();
            this.Text += Marshal.PtrToStringAnsi(AgentForm.GetGridVersion());
            this.labels.Add(this.lbl1);
            this.labels.Add(this.lbl2);
            this.labels.Add(this.lbl3);
            this.labels.Add(this.lbl4);
            this.labels.Add(this.lbl5);
            this.labels.Add(this.lbl6);
            this.labels.Add(this.lbl7);
            this.labels.Add(this.lbl8);
            this.labels_val.Add(this.lbv1);
            this.labels_val.Add(this.lbv2);
            this.labels_val.Add(this.lbv3);
            this.labels_val.Add(this.lbv4);
            this.labels_val.Add(this.lbv5);
            this.labels_val.Add(this.lbv6);
            this.labels_val.Add(this.lbv7);
            this.labels_val.Add(this.lbv8);
            for (int i = 0; i < this.labels.Count; i++)
            {
                this.labels[i].Text = "";
                this.labels_val[i].Text = "";
            }
            this.timer1.Enabled = true;
        }

        private void AgentForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.WindowsShutDown || e.CloseReason == CloseReason.TaskManagerClosing || e.CloseReason == CloseReason.ApplicationExitCall)
            {
                this.exit = true;
            }
            if (!this.exit)
            {
                e.Cancel = true;
                base.WindowState = FormWindowState.Minimized;
                return;
            }
            base.Hide();
            AgentForm.ShutdownAgent();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.agent_info.CoordinatorName = new string(' ', 260);
            AgentForm.GetAgentStatus(ref this.agent_info);
            this.statusConnection.Text = string.Format("Coordinator: {0}", (this.agent_info.Status == 0) ? "Unassigned" : this.agent_info.CoordinatorName);
            string text;
            string text2;
            if (this.agent_info.Status == 0)
            {
                text = "uAgent: Unassigned";
                text2 = "Unassigned";
            }
            else
            {
                text = "uAgent: " + this.agent_info.CoordinatorName;
                if (this.agent_info.UserTasks == 0)
                {
                    text2 = "idle";
                }
                else
                {
                    text2 = "";
                }
            }
            for (int i = 0; i < this.labels.Count; i++)
            {
                if (i >= this.agent_info.UserTasks)
                {
                    this.labels[i].Text = "";
                    this.labels_val[i].Text = "";
                }
                else
                {
                    AgentForm.TaskInfo taskInfo = default(AgentForm.TaskInfo);
                    taskInfo.ContextName = new string(' ', 260);
                    taskInfo.UserName = new string(' ', 260);
                    AgentForm.GetTaskInfo(i, ref taskInfo);
                    text2 = text2 + string.Format("{0} - {1}", taskInfo.UserName, taskInfo.ContextName) + "\r\n";
                    this.labels[i].Text = string.Format("{0} - {1}", taskInfo.UserName, taskInfo.ContextName);
                    this.labels_val[i].Text = string.Format("[{0} / {1}]", taskInfo.TaskRunning, taskInfo.TaskCount);
                }
            }
            if (this.notifyIcon1.BalloonTipTitle != text || this.notifyIcon1.BalloonTipText != text2)
            {
                this.notifyIcon1.BalloonTipTitle = text;
                this.notifyIcon1.BalloonTipText = text2;
            }
        }

        private void notifyIcon1_DoubleClick(object sender, EventArgs e)
        {
            base.Show();
            base.WindowState = FormWindowState.Normal;
            base.Activate();
        }

        private void AgentForm_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == base.WindowState)
            {
                base.Hide();
            }
        }

        private void miExit_Click(object sender, EventArgs e)
        {
            Process.GetCurrentProcess().Kill();
        }

        private void miStatus_Click(object sender, EventArgs e)
        {
            base.Show();
            base.WindowState = FormWindowState.Normal;
            base.Activate();
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == 18 || m.Msg == 17)
            {
                Process.GetCurrentProcess().Kill();
            }
            base.WndProc(ref m);
        }

        private void notifyIcon1_Click(object sender, EventArgs e)
        {
        }

        private void notifyIcon1_BalloonTipClicked(object sender, EventArgs e)
        {
            base.Show();
            base.WindowState = FormWindowState.Normal;
            base.Activate();
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing && this.components != null)
            {
                this.components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.components = new Container();
            ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(AgentForm));
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.notifyIcon1 = new NotifyIcon(this.components);
            this.contextMenuStrip1 = new ContextMenuStrip(this.components);
            this.miStatus = new ToolStripMenuItem();
            this.miExit = new ToolStripMenuItem();
            this.labelsplitter = new System.Windows.Forms.Label();
            this.statusStrip1 = new StatusStrip();
            this.statusConnection = new ToolStripStatusLabel();
            this.panel1 = new Panel();
            this.lbl8 = new System.Windows.Forms.Label();
            this.lbl7 = new System.Windows.Forms.Label();
            this.lbl6 = new System.Windows.Forms.Label();
            this.lbl5 = new System.Windows.Forms.Label();
            this.lbl4 = new System.Windows.Forms.Label();
            this.lbl3 = new System.Windows.Forms.Label();
            this.lbl2 = new System.Windows.Forms.Label();
            this.lbl1 = new System.Windows.Forms.Label();
            this.panel2 = new Panel();
            this.lbv8 = new System.Windows.Forms.Label();
            this.lbv7 = new System.Windows.Forms.Label();
            this.lbv6 = new System.Windows.Forms.Label();
            this.lbv5 = new System.Windows.Forms.Label();
            this.lbv4 = new System.Windows.Forms.Label();
            this.lbv3 = new System.Windows.Forms.Label();
            this.lbv2 = new System.Windows.Forms.Label();
            this.lbv1 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.contextMenuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            base.SuspendLayout();
            this.timer1.Tick += this.timer1_Tick;
            this.notifyIcon1.ContextMenuStrip = this.contextMenuStrip1;
            this.notifyIcon1.Icon = (Icon)componentResourceManager.GetObject("notifyIcon1.Icon");
            this.notifyIcon1.Text = "uAgent";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.BalloonTipClicked += this.notifyIcon1_BalloonTipClicked;
            this.notifyIcon1.Click += this.notifyIcon1_Click;
            this.notifyIcon1.DoubleClick += this.notifyIcon1_DoubleClick;
            this.contextMenuStrip1.Items.AddRange(new ToolStripItem[]
            {
                this.miStatus,
                this.miExit
            });
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.ShowImageMargin = false;
            this.contextMenuStrip1.Size = new Size(82, 48);
            this.miStatus.Name = "miStatus";
            this.miStatus.Size = new Size(81, 22);
            this.miStatus.Text = "Status";
            this.miStatus.Click += this.miStatus_Click;
            this.miExit.Name = "miExit";
            this.miExit.Size = new Size(81, 22);
            this.miExit.Text = "Exit";
            this.miExit.Click += this.miExit_Click;
            this.labelsplitter.Dock = DockStyle.Top;
            this.labelsplitter.Font = new Font("Arial Narrow", 11.25f, FontStyle.Bold, GraphicsUnit.Point, 204);
            this.labelsplitter.Location = new Point(0, 0);
            this.labelsplitter.Name = "labelsplitter";
            this.labelsplitter.Size = new Size(261, 23);
            this.labelsplitter.TabIndex = 4;
            this.labelsplitter.Text = "Tasks:";
            this.labelsplitter.TextAlign = ContentAlignment.MiddleCenter;
            this.statusStrip1.Items.AddRange(new ToolStripItem[]
            {
                this.statusConnection
            });
            this.statusStrip1.Location = new Point(0, 207);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new Size(329, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 13;
            this.statusStrip1.Text = "statusStrip1";
            this.statusConnection.Name = "statusConnection";
            this.statusConnection.Size = new Size(106, 17);
            this.statusConnection.Text = "Status: Unassigned";
            this.panel1.Controls.Add(this.lbl8);
            this.panel1.Controls.Add(this.lbl7);
            this.panel1.Controls.Add(this.lbl6);
            this.panel1.Controls.Add(this.lbl5);
            this.panel1.Controls.Add(this.lbl4);
            this.panel1.Controls.Add(this.lbl3);
            this.panel1.Controls.Add(this.lbl2);
            this.panel1.Controls.Add(this.lbl1);
            this.panel1.Dock = DockStyle.Fill;
            this.panel1.Location = new Point(0, 23);
            this.panel1.Name = "panel1";
            this.panel1.Size = new Size(261, 184);
            this.panel1.TabIndex = 14;
            this.lbl8.Dock = DockStyle.Top;
            this.lbl8.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl8.Location = new Point(0, 161);
            this.lbl8.Name = "lbl8";
            this.lbl8.Size = new Size(261, 23);
            this.lbl8.TabIndex = 20;
            this.lbl8.Text = "lbl8";
            this.lbl8.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl7.Dock = DockStyle.Top;
            this.lbl7.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl7.Location = new Point(0, 138);
            this.lbl7.Name = "lbl7";
            this.lbl7.Size = new Size(261, 23);
            this.lbl7.TabIndex = 19;
            this.lbl7.Text = "lbl7";
            this.lbl7.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl6.Dock = DockStyle.Top;
            this.lbl6.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl6.Location = new Point(0, 115);
            this.lbl6.Name = "lbl6";
            this.lbl6.Size = new Size(261, 23);
            this.lbl6.TabIndex = 18;
            this.lbl6.Text = "lbl6";
            this.lbl6.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl5.Dock = DockStyle.Top;
            this.lbl5.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl5.Location = new Point(0, 92);
            this.lbl5.Name = "lbl5";
            this.lbl5.Size = new Size(261, 23);
            this.lbl5.TabIndex = 17;
            this.lbl5.Text = "lbl5";
            this.lbl5.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl4.Dock = DockStyle.Top;
            this.lbl4.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl4.Location = new Point(0, 69);
            this.lbl4.Name = "lbl4";
            this.lbl4.Size = new Size(261, 23);
            this.lbl4.TabIndex = 16;
            this.lbl4.Text = "lbl4";
            this.lbl4.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl3.Dock = DockStyle.Top;
            this.lbl3.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl3.Location = new Point(0, 46);
            this.lbl3.Name = "lbl3";
            this.lbl3.Size = new Size(261, 23);
            this.lbl3.TabIndex = 15;
            this.lbl3.Text = "lbl3";
            this.lbl3.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl2.Dock = DockStyle.Top;
            this.lbl2.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl2.Location = new Point(0, 23);
            this.lbl2.Name = "lbl2";
            this.lbl2.Size = new Size(261, 23);
            this.lbl2.TabIndex = 14;
            this.lbl2.Text = "lbl2";
            this.lbl2.TextAlign = ContentAlignment.MiddleLeft;
            this.lbl1.Dock = DockStyle.Top;
            this.lbl1.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbl1.Location = new Point(0, 0);
            this.lbl1.Name = "lbl1";
            this.lbl1.Size = new Size(261, 23);
            this.lbl1.TabIndex = 13;
            this.lbl1.Text = "lbl1";
            this.lbl1.TextAlign = ContentAlignment.MiddleLeft;
            this.panel2.Controls.Add(this.lbv8);
            this.panel2.Controls.Add(this.lbv7);
            this.panel2.Controls.Add(this.lbv6);
            this.panel2.Controls.Add(this.lbv5);
            this.panel2.Controls.Add(this.lbv4);
            this.panel2.Controls.Add(this.lbv3);
            this.panel2.Controls.Add(this.lbv2);
            this.panel2.Controls.Add(this.lbv1);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Dock = DockStyle.Right;
            this.panel2.Location = new Point(261, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new Size(68, 207);
            this.panel2.TabIndex = 15;
            this.lbv8.Dock = DockStyle.Top;
            this.lbv8.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv8.Location = new Point(0, 184);
            this.lbv8.Name = "lbv8";
            this.lbv8.Size = new Size(68, 23);
            this.lbv8.TabIndex = 28;
            this.lbv8.Text = "lbv8";
            this.lbv8.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv7.Dock = DockStyle.Top;
            this.lbv7.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv7.Location = new Point(0, 161);
            this.lbv7.Name = "lbv7";
            this.lbv7.Size = new Size(68, 23);
            this.lbv7.TabIndex = 27;
            this.lbv7.Text = "lbv7";
            this.lbv7.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv6.Dock = DockStyle.Top;
            this.lbv6.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv6.Location = new Point(0, 138);
            this.lbv6.Name = "lbv6";
            this.lbv6.Size = new Size(68, 23);
            this.lbv6.TabIndex = 26;
            this.lbv6.Text = "lbv6";
            this.lbv6.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv5.Dock = DockStyle.Top;
            this.lbv5.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv5.Location = new Point(0, 115);
            this.lbv5.Name = "lbv5";
            this.lbv5.Size = new Size(68, 23);
            this.lbv5.TabIndex = 25;
            this.lbv5.Text = "lbv5";
            this.lbv5.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv4.Dock = DockStyle.Top;
            this.lbv4.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv4.Location = new Point(0, 92);
            this.lbv4.Name = "lbv4";
            this.lbv4.Size = new Size(68, 23);
            this.lbv4.TabIndex = 24;
            this.lbv4.Text = "lbv4";
            this.lbv4.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv3.Dock = DockStyle.Top;
            this.lbv3.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv3.Location = new Point(0, 69);
            this.lbv3.Name = "lbv3";
            this.lbv3.Size = new Size(68, 23);
            this.lbv3.TabIndex = 23;
            this.lbv3.Text = "lbv3";
            this.lbv3.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv2.Dock = DockStyle.Top;
            this.lbv2.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv2.Location = new Point(0, 46);
            this.lbv2.Name = "lbv2";
            this.lbv2.Size = new Size(68, 23);
            this.lbv2.TabIndex = 22;
            this.lbv2.Text = "lbv2";
            this.lbv2.TextAlign = ContentAlignment.MiddleCenter;
            this.lbv1.Dock = DockStyle.Top;
            this.lbv1.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 204);
            this.lbv1.Location = new Point(0, 23);
            this.lbv1.Name = "lbv1";
            this.lbv1.Size = new Size(68, 23);
            this.lbv1.TabIndex = 21;
            this.lbv1.Text = "lbv1";
            this.lbv1.TextAlign = ContentAlignment.MiddleCenter;
            this.label1.Dock = DockStyle.Top;
            this.label1.Font = new Font("Arial Narrow", 11.25f, FontStyle.Bold, GraphicsUnit.Point, 204);
            this.label1.Location = new Point(0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new Size(68, 23);
            this.label1.TabIndex = 5;
            this.label1.TextAlign = ContentAlignment.MiddleCenter;
            base.AutoScaleDimensions = new SizeF(6f, 13f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(329, 229);
            base.Controls.Add(this.panel1);
            base.Controls.Add(this.labelsplitter);
            base.Controls.Add(this.panel2);
            base.Controls.Add(this.statusStrip1);
            base.FormBorderStyle = FormBorderStyle.FixedSingle;
            base.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "AgentForm";
            this.Text = "ugrid agent";
            base.FormClosing += this.AgentForm_FormClosing;
            base.Load += this.AgentForm_Load;
            base.Resize += this.AgentForm_Resize;
            this.contextMenuStrip1.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            base.ResumeLayout(false);
            base.PerformLayout();
        }

        public const int WM_QUIT = 18;
        public const int WM_QUERYENDSESSION = 17;
        private bool exit;
        private List<System.Windows.Forms.Label> labels = new List<System.Windows.Forms.Label>();
        private List<System.Windows.Forms.Label> labels_val = new List<System.Windows.Forms.Label>();
        private AgentForm.AgentInfo agent_info;
        private IContainer components;
        private System.Windows.Forms.Timer timer1;
        private NotifyIcon notifyIcon1;
        private ContextMenuStrip contextMenuStrip1;
        private ToolStripMenuItem miExit;
        private ToolStripMenuItem miStatus;
        private System.Windows.Forms.Label labelsplitter;
        private StatusStrip statusStrip1;
        private ToolStripStatusLabel statusConnection;
        private Panel panel1;
        private System.Windows.Forms.Label lbl8;
        private System.Windows.Forms.Label lbl7;
        private System.Windows.Forms.Label lbl6;
        private System.Windows.Forms.Label lbl5;
        private System.Windows.Forms.Label lbl4;
        private System.Windows.Forms.Label lbl3;
        private System.Windows.Forms.Label lbl2;
        private System.Windows.Forms.Label lbl1;
        private Panel panel2;
        private System.Windows.Forms.Label lbv8;
        private System.Windows.Forms.Label lbv7;
        private System.Windows.Forms.Label lbv6;
        private System.Windows.Forms.Label lbv5;
        private System.Windows.Forms.Label lbv4;
        private System.Windows.Forms.Label lbv3;
        private System.Windows.Forms.Label lbv2;
        private System.Windows.Forms.Label lbv1;
        private System.Windows.Forms.Label label1;

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct AgentInfo
        {
            public int Status;
            public int UserTasks;
            public int TaskRunning;
            public int TaskCount;
            public string CoordinatorName;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct TaskInfo
        {
            public int TaskCount;
            public int TaskRunning;
            public string UserName;
            public string ContextName;
        }
    }
}
