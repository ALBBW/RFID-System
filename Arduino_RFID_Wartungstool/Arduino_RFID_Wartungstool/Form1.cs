using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO;

namespace Arduino_RFID_Wartungstool
{
    public partial class Form1 : Form
    {
        Connector conn = new Connector();
        string connCommander = "";
        bool ispause = false;

        public Form1()
        {
            InitializeComponent();
            backgroundWorker1.RunWorkerAsync();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            string zse = "";
            while (true)
            {
                string arduData = conn.Connect(connCommander);
                if (!ispause)
                {
                    if(zse != "")
                    {
                        richTextBox1.Invoke((MethodInvoker)delegate
                        {
                            richTextBox1.Text = zse;
                        });
                        zse = "";
                    }
                    richTextBox1.Invoke((MethodInvoker)delegate
                    {
                        richTextBox1.Text += arduData + "\n";
                        richTextBox1.SelectionStart = richTextBox1.Text.Length;
                        richTextBox1.ScrollToCaret();
                    });
                }
                else
                {
                    if (zse == "")
                    {
                        richTextBox1.Invoke((MethodInvoker)delegate
                        {
                            zse = richTextBox1.Text;
                        });
                    }
                    zse += arduData;
                }
                connCommander = "";
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string path = "Arduino_Report_" + DateTime.Now.Day + "_" + DateTime.Now.Month + "_" + DateTime.Now.Year + "_" + DateTime.Now.Hour + "_" + DateTime.Now.Minute + "_" + DateTime.Now.Second + ".txt";
            StreamWriter sw = new StreamWriter(path);
            sw.WriteLine(richTextBox1.Text);
            sw.Flush();
            sw.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if(!ispause)
            {
                ispause = true;
                button3.Text = "Fortsetzen";
            }
            else
            {
                button3.Text = "Pause";
                ispause = false;
            }
        }
    }
}
