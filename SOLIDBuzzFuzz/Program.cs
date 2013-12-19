using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLIDBuzzFuzz
{
	class Program
	{
		static void Main(string[] args)
		{
			foreach (var w in Buzz.Fuzz.Add(11, "Wham").Add(3, "pow").StopAt(143)) //.StartAt(3)
				Console.Write("{0}, ", w);
		}
	}
}
