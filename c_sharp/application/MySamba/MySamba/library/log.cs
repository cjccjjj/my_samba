using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using Newtonsoft.Json.Linq;
using System.IO;
using System.Text;  // for Encoding
using System.Diagnostics;    // for StackTrace


namespace NLog
{

	public class XLog
	{
		private static XLog m_ins;
		private static object m_lock_object = new object();


		string m_log_path = "";
		int m_main_stack_count = 0;
		JObject m_root;
		bool m_message_box;

		protected XLog()
		{
		}
		public static XLog ins()
		{
			lock (m_lock_object)
			{
				if (null == m_ins)
				{
					m_ins = new XLog();
				}
			}
			return m_ins;
		}

		public bool create (string log_path, int main_stack_count, bool  message_box)
		{
			m_log_path = log_path;
			m_main_stack_count = main_stack_count;
			m_message_box = message_box;
			XLog.ins();

			return true;
		}

		public void write(string format, params object[] args)
		{
			lock (m_lock_object)
			{
				string log_message = "";
				StreamWriter SW;
				SW = File.AppendText(m_log_path);

				string timeStamp = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss.ff");
				string time_string = string.Format("[{0}]", timeStamp);
				//SW.Write(time_string);
				log_message += time_string;
				if (0 == m_main_stack_count)
				{
					StackFrame callStack = new StackFrame(1, true);
					string file_line = string.Format("[{0}:{1}]: ", callStack.GetFileName(), callStack.GetFileLineNumber());
					//SW.Write(file_line);
					log_message += file_line;
				}
				else
				{
					StackTrace st = new StackTrace();
					int frame_count = st.FrameCount;
					int current_stack_count = frame_count - m_main_stack_count;
					++current_stack_count;
					for (int i = 1; i < current_stack_count; ++i)
					{
						StackFrame callStack = new StackFrame(i, true);
						string file_line = string.Format("[{0}:{1}]", callStack.GetFileName(), callStack.GetFileLineNumber());
						//SW.WriteLine(file_line);
						log_message += file_line;
					}
				}
				log_message += string.Format(format, args);
				SW.Write(log_message);
				SW.Close();
				SW.Dispose();

				if (m_message_box)
				{
					MessageBox.Show(log_message);
				}
				
			}
		}

		//public void test2 ([System.Runtime.CompilerServices.CallerFilePath] string filePath = null, [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
		//{

		//}

		public int __LINE__([System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0)
		{
			return lineNumber;
		}
		static string __FILE__([System.Runtime.CompilerServices.CallerFilePath] string fileName = "")
		{
			return fileName;
		}


	}
}
