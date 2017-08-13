using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Text;  // for StringBuilder

using Newtonsoft.Json.Linq;
using System.IO;
using System.Runtime.InteropServices;   // for DllImport
namespace NDllApi
{
    static public class XDllApi
	{
		[DllImport("user32.dll")]
		public static extern bool PostMessage(IntPtr hWnd, uint Msg, int wParam, int lParam);


		public delegate void Win32Callback(int value1, [MarshalAs(UnmanagedType.LPWStr)] string text1);

		public static Win32Callback s_callback;

		[DllImport("samba_dll.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern bool samba_create(StringBuilder p_ip, int port, StringBuilder p_id, StringBuilder p_password, StringBuilder p_local_dir, StringBuilder p_remote_dir, Win32Callback callback);
		[DllImport("samba_dll.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void samba_clear_exclusive_ext();
		[DllImport("samba_dll.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern bool samba_add_exclusive_ext(StringBuilder exclusive_ext);
		
		[DllImport("samba_dll.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void samba_clear_exclusive_dir();
		[DllImport("samba_dll.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern bool samba_add_exclusive_dir(StringBuilder exclusive_ext);
	}
}
