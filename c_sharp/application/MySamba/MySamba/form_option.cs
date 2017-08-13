using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using NConf;

namespace MySamba
{
	public partial class form_option : Form
	{
		public form_option()
		{
			InitializeComponent();
		}

		private void form_option_Load(object sender, EventArgs e)
		{
			this.textBoxLocalPath.Text = XConf.ins().LocalDirectory;
			this.textBoxRemotePath.Text = XConf.ins().RemoteDirectory;

			textBoxIp.Text = XConf.ins().Ip;
			textBoxPort.Text = XConf.ins().Port;
			textBoxId.Text = XConf.ins().Id;
			textBoxPassword.Text = XConf.ins().Password;

			for (int i = 0; i < XConf.ins().get_exclude_ext_count(); ++i)
			{
				listBoxExclusiveExt.Items.Add(XConf.ins().get_exclude_ext(i));
			}
			for (int i = 0; i < XConf.ins().get_exclude_dir_count(); ++i)
			{
				listBoxExclusiveDir.Items.Add(XConf.ins().get_exclude_dir(i));
			}
		}

		private void btnAddExt_Click(object sender, EventArgs e)
		{
			if (textBoxExt.Text == "")
			{

			}
			else
			{
				listBoxExclusiveExt.Items.Add(textBoxExt.Text);
			}
		}

		private void btnDelExt_Click(object sender, EventArgs e)
		{
			for (int x = listBoxExclusiveExt.SelectedIndices.Count - 1; x >= 0; x--)
			{
				int var = listBoxExclusiveExt.SelectedIndices[x];
				listBoxExclusiveExt.Items.RemoveAt(var);
			}
		}

		private void btnAddDir_Click(object sender, EventArgs e)
		{
			if (textBoxDir.Text == "")
			{

			}
			else
			{
				listBoxExclusiveDir.Items.Add(textBoxDir.Text);
			}
		}

		private void btnDelDir_Click(object sender, EventArgs e)
		{
			for (int x = listBoxExclusiveDir.SelectedIndices.Count - 1; x >= 0; x--)
			{
				int var = listBoxExclusiveDir.SelectedIndices[x];
				listBoxExclusiveDir.Items.RemoveAt(var);
			}
		}

		private void btnOK_Click(object sender, EventArgs e)
		{
			XConf.ins().LocalDirectory = this.textBoxLocalPath.Text;
			XConf.ins().RemoteDirectory = this.textBoxRemotePath.Text;

			XConf.ins().Ip = textBoxIp.Text;
			XConf.ins().Port = textBoxPort.Text;
			XConf.ins().Id = textBoxId.Text;
			XConf.ins().Password = textBoxPassword.Text;

			XConf.ins().clear_cxclude_ext();
			for (int i = 0; i < listBoxExclusiveExt.Items.Count; ++i)
			{
				XConf.ins().add_exclude_ext(listBoxExclusiveExt.Items[i].ToString());
			}
			XConf.ins().clear_cxclude_dir();
			for (int i = 0; i < listBoxExclusiveDir.Items.Count; ++i)
			{
				XConf.ins().add_exclude_dir(listBoxExclusiveDir.Items[i].ToString());
			}

			Close();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}
