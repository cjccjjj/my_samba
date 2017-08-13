using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Text;  // for StringBuilder

using NGlobal;
using NConf;
using NLog;
using NUtil;
using System.Diagnostics;
using NDllApi;
//using Samba;

namespace MySamba
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			//Application.EnableVisualStyles();
			//Application.SetCompatibleTextRenderingDefault(false);
			//Application.Run(new Form1());

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);

			string current_directory = XUtil.get_current_directory();
			string process_name = XUtil.get_process_name();
			process_name = XUtil.get_process_name_without_extension();
			string conf_file = process_name + ".conf";
			string log_file = process_name + ".log";

			if (false == XConf.ins().create(conf_file))
			{
				MessageBox.Show("conf file error.");
				return;
			}
			StackTrace st = new StackTrace();
			//XLog.ins().create(log_file, st.FrameCount, true);
			XLog.ins().create(log_file, 0, true);
			//XLog.ins().write("log test\r\n");
			
			Form1 temp = new Form1();
			bool return_check = XDllApi.samba_create(new StringBuilder(XConf.ins().Ip), Convert.ToInt32(XConf.ins().Port),
				new StringBuilder(XConf.ins().Id), new StringBuilder(XConf.ins().Password),
				new StringBuilder(XConf.ins().LocalDirectory), new StringBuilder(XConf.ins().RemoteDirectory),
				temp.MyCallbackFunc);
			if (false == return_check)
			{
				MessageBox.Show("samba_create fail.");
				return;
			}
			XDllApi.samba_clear_exclusive_ext();
			for (int i = 0; i < XConf.ins().get_exclude_ext_count(); ++i)
			{
				XDllApi.samba_add_exclusive_ext(new StringBuilder(XConf.ins().get_exclude_ext(i)));
			}
			XDllApi.samba_clear_exclusive_dir();
			for (int i = 0; i < XConf.ins().get_exclude_dir_count(); ++i)
			{
				XDllApi.samba_add_exclusive_dir(new StringBuilder(XConf.ins().get_exclude_dir(i)));
			}

			Application.AddMessageFilter(temp);
			Application.Run(temp);

			XConf.ins().update();
		}
	}
}
