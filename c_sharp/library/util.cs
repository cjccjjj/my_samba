using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using Newtonsoft.Json.Linq;
using System.IO;
using System.Text;  // for Encoding
using System.Diagnostics;    // for StackTrace


namespace NUtil
{

	public static class XUtil
	{
		public static string get_current_directory ()
		{
			return System.IO.Directory.GetCurrentDirectory();
		}
		public static string get_process_name ()
		{
			return System.Diagnostics.Process.GetCurrentProcess().ProcessName;
		}

		public static string get_process_name_without_extension()
		{
			string process_name = System.Diagnostics.Process.GetCurrentProcess().ProcessName;
			return Path.GetFileNameWithoutExtension(process_name);	
		}
	}
}
