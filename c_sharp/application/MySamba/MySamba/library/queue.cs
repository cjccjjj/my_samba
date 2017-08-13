using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Collections;
using System.IO;

namespace NQueue
{
    public class XQueue
	{
		object m_locker = new object();
		Queue m_Queue = new Queue();

		public void push (string input_data)
		{
			lock (m_locker)
			{
				m_Queue.Enqueue(input_data);
			}
		}

		public string pop ()
		{
			lock (m_locker)
			{
				return (string)m_Queue.Dequeue();
			}
		}

		public int count ()
		{
			lock (m_locker)
			{
				return m_Queue.Count;
			}
		}

	}
}
