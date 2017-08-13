using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using Newtonsoft.Json.Linq;
using System.IO;
using System.Text;  // for Encoding

namespace NConf
{

	public class XConf
	{
		private static XConf m__ins;
		private static object m_lock_object = new object();

		// position
		private int m_pos_x = 0;
		private int m_pos_y = 0;
		private int m_pos_w = 0;
		private int m_pos_h = 0;

		// directory
		private string m_local_directory = "";
		private string m_remote_directory = "";

		private string m_ip = "";
		private string m_port = "";
		private string m_id = "";
		private string m_password = "";

		private JArray m_exclusive_ext;
		private JArray m_exclusive_dir;

		string m_conf_path = "";
		JObject m_root;
		protected XConf()
		{
		}
		public static XConf ins()
		{
			lock (m_lock_object)
			{
				if (null == m__ins)
				{
					m__ins = new XConf();
				}
			}
			return m__ins;
		}

		public bool create (string conf_path)
		{
			m_conf_path = conf_path;
			FileStream ConfFile = null;
			try
			{
				ConfFile = new FileStream(conf_path, FileMode.Open, FileAccess.Read);
				byte[] byte_array = new byte[ConfFile.Length];
				int numBytesToRead = (int)ConfFile.Length;
				int numBytesRead = 0;
				while (numBytesToRead > 0)
				{
					int n = ConfFile.Read(byte_array, numBytesRead, numBytesToRead);
					if (n == 0)
					{
						break;
					}

					numBytesRead += n;
					numBytesToRead -= n;
				}

				string result = System.Text.Encoding.ASCII.GetString(byte_array);

				m_root = JObject.Parse(result);

				// position
				var position = m_root["position"];
				m_pos_x = (int)position["pos_x"];
				m_pos_y = (int)position["pos_y"];
				m_pos_w = (int)position["pos_w"];
				m_pos_h = (int)position["pos_h"];

				var directory = m_root["directory"];
				m_local_directory = (string)directory["local"];
				m_remote_directory = (string)directory["remote"];

				var address = m_root["address"];
				m_ip = (string)address["ip"];
				m_port = (string)address["port"];

				var auth = m_root["auth"];
				m_id = (string)auth["id"];
				m_password = (string)auth["password"];

				m_exclusive_ext = (JArray)m_root["exclusive_ext"];
				m_exclusive_dir = (JArray)m_root["exclusive_dir"];

				ConfFile.Close();

				int i = 0;
				++i;
			}
			catch
			{
				MessageBox.Show("conf file create fail.");
				return false;
			}

			return true;
		}

		public bool update ()
		{
			FileStream ConfFile = null;
			try
			{
				//System.IO.File.Delete(m_conf_path);

				var position = m_root["position"];
				position["pos_x"] = m_pos_x;
				position["pos_y"] = m_pos_y;
				position["pos_w"] = m_pos_w;
				position["pos_h"] = m_pos_h;

				var directory = m_root["directory"];
				directory["local"] = m_local_directory;
				directory["remote"] = m_remote_directory;

				var exclusive_ext = m_root["exclusive_ext"];
				exclusive_ext = m_exclusive_ext;
				var exclusive_dir = m_root["exclusive_dir"];
				exclusive_dir = m_exclusive_dir;

				string root_string = m_root.ToString();
				byte[] byte_array = Encoding.Default.GetBytes(root_string);
				ConfFile = new FileStream(m_conf_path, FileMode.Truncate, FileAccess.Write);
				ConfFile.Write(byte_array, 0, byte_array.Length);


				ConfFile.Close();
			}
			catch
			{
				MessageBox.Show("conf file update fail.");
				return false;
			}

			return true;
		}

		//public int get_pos_x()
		//{
		//	return m_pos_x;
		//}
		//public void set_pos_x(int pos_x)
		//{
		//	m_pos_x = pos_x;
		//}
		public int PosX
		{
			get { return m_pos_x; }
			set { m_pos_x = value; }
		}
		public int PosY
		{
			get { return m_pos_y; }
			set { m_pos_y = value; }
		}
		public int PosW
		{
			get { return m_pos_w; }
			set { m_pos_w = value; }
		}
		public int PosH
		{
			get { return m_pos_h; }
			set { m_pos_h = value; }
		}
		public string LocalDirectory
		{
			get { return m_local_directory; }
			set { m_local_directory = value; }
		}
		public string RemoteDirectory
		{
			get { return m_remote_directory; }
			set { m_remote_directory = value; }
		}

		public string Ip
		{
			get { return m_ip; }
			set { m_ip = value; }
		}
		public string Port
		{
			get { return m_port; }
			set { m_port = value; }
		}
		public string Id
		{
			get { return m_id; }
			set { m_id = value; }
		}
		public string Password
		{
			get { return m_password; }
			set { m_password = value; }
		}

		public int get_exclude_ext_count()
		{
			return m_exclusive_ext.Count;
		}
		public string get_exclude_ext (int offset)
		{
			return (string)m_exclusive_ext[offset];
		}
		public void clear_cxclude_ext()
		{
			m_exclusive_ext.Clear();
		}
		public void add_exclude_ext (string exclude_ext)
		{
			m_exclusive_ext.Add(exclude_ext);
		}

		public int get_exclude_dir_count()
		{
			return m_exclusive_dir.Count;
		}
		public string get_exclude_dir(int offset)
		{
			return (string)m_exclusive_dir[offset];
		}
		public void clear_cxclude_dir()
		{
			m_exclusive_dir.Clear();
		}
		public void add_exclude_dir(string exclude_dir)
		{
			m_exclusive_dir.Add(exclude_dir);
		}

	}
}
