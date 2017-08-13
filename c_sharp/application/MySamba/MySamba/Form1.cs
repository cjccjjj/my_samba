using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using NGlobal;
using NConf;
using NDllApi;
using NQueue;

namespace MySamba
{
	public partial class Form1 : Form, IMessageFilter
	{
		private object m_receive_confirm_locker = new object();
		private bool m_receive_confirm_flag = false;
		private XQueue m_Queue = new XQueue();

		public Form1()
		{
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			this.StartPosition = FormStartPosition.Manual;
			this.Location = new Point(XConf.ins().PosX, XConf.ins().PosY);
			this.Size = new Size(XConf.ins().PosW, XConf.ins().PosH);
		}

		private void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			this.StartPosition = FormStartPosition.Manual;
			XConf.ins().PosX = this.Location.X;
			XConf.ins().PosY = this.Location.Y;
			XConf.ins().PosW = this.Size.Width;
			XConf.ins().PosH = this.Size.Height;
		}

		public void MyCallbackFunc(int value1, string text1)
		{
			// Queue 에 데이터 밀어넣은후에 메시지 보내야 한다.
			m_Queue.push(text1);

			ReceiveConfirmFlag = false;
			for (;;)
			{
				System.Threading.Thread.Sleep(100);

				CheckForIllegalCrossThreadCalls = false;
				XDllApi.PostMessage(this.Handle, (0x0400 + 1), 0, 0);
				if (ReceiveConfirmFlag)
				{
					break;
				}
			}
		}

		private bool ReceiveConfirmFlag
		{
			get
			{
				lock (m_receive_confirm_locker)
				{
					return m_receive_confirm_flag;
				}
			}

			set
			{
				lock (m_receive_confirm_locker)
				{
					m_receive_confirm_flag = value;
				}
			}
		}

		public bool PreFilterMessage(ref Message msg)
		{
			if (0x0400 + 1 == msg.Msg)
			{
				// 큐에서 메시지를 받는다.
				ReceiveConfirmFlag = true;

				while (0 < m_Queue.count())
				{
					string pop_string = pop_string = m_Queue.pop();
					textBoxMonitor.Text += pop_string;
				}

				return true;
			}
			return false;
		}

		private void btnConfig_Click(object sender, EventArgs e)
		{
			form_option mf = new form_option();
			mf.ShowDialog();
		}
	}
}
