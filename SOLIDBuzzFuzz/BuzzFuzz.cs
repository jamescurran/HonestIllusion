using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOLIDBuzzFuzz
{

	public class BuzzFuzz : IEnumerator<string>, IEnumerable<string>
	{
		internal class Substitution
		{
			public Substitution(int digit, string word)
			{
				Digit = digit;
				Word = word;
				_testChar = (Digit < 10) ? (char)(Digit+'0') : '_';

			}
			public int Digit { get; private set; }
			public string Word { get; private set; }

			private char _testChar;
			public bool SayIt(int num, string text)=> (num%Digit)==0 || text.Contains(_testChar);

		}

		#region Private fields
		private int _num;
		private int _start = 1;
		private int _end = Int32.MaxValue;
		private readonly List<Substitution> _substitutions = new List<Substitution>();
		#endregion

		#region Constructors

		public BuzzFuzz(int start)
		{
			_start = start;
			_substitutions.Add(new Substitution(digit: 5, word: "Buzz" ));
			_substitutions.Add(new Substitution ( digit: 7, word: "Fuzz" ));
			Reset();
		}

		public BuzzFuzz() : this(1)
		{
		}
		#endregion

		#region IEnumerator<string> members
		public string Current { get; private set; }

		public bool MoveNext()
		{
			Current = _num.ToString(CultureInfo.InvariantCulture);
			bool showWord = false;
			var sb = new StringBuilder();
			foreach (var sub in _substitutions)
			{
				if (sub.SayIt(_num, Current))
				{
					sb.Append(sub.Word);
					showWord = true;
				}
			}

			if (showWord)
				Current = sb.ToString();

			++_num;
			return _num <= _end;
		}

		public void Reset()
		{
			_num = _start;
		}
		#endregion

		#region Fluent Construction interface

		public BuzzFuzz Add(int digit, string word)
		{
			_substitutions.Add(new Substitution {Digit = digit, Word = word});
			return this;
		}

		public BuzzFuzz StartAt(int start)
		{
			_start = start;
			Reset();
			return this;
		}

		/// <summary>
		/// Sets EXCLUSIVE ending value.
		/// </summary>
		/// <param name="term"></param>
		/// <returns>If not set, enumeration continues to Int32.MaxValue</returns>
		public BuzzFuzz StopAt(int term)
		{
			_end = term;
			return this;
		}

		public BuzzFuzz Clear()
		{
			_substitutions.Clear();
			return this;
		}
		#endregion

		#region Unimportant methods

		object System.Collections.IEnumerator.Current
		{
			get { return Current; }
		}

		public void Dispose()
		{
		}

		#endregion

		#region IEnumerable methods

		public IEnumerator<string> GetEnumerator()
		{
			return this;
		}

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
		{
			return this;
		}

		#endregion
	}

	internal class Buzz
	{
		public static BuzzFuzz Fuzz
		{
			get { return new BuzzFuzz(); }
		}
	}
}

#region Home of dead code

#if false
		internal class BuzzFuzz_impl
		{
			internal int num = 1;
			internal int term = Int32.MaxValue;

			internal List<Substitution> substitutions = new List<Substitution>();
		   
			public IEnumerable<string> Generate()
			{
				while (num < term)
				{
					bool showWord = false;
					var sb = new StringBuilder();
					var strNum = num.ToString();
					foreach (var sub in substitutions)
					{
						if ((num % sub.Digit == 0) ||
							 (sub.Digit < 10 && strNum.Contains(sub.Digit.ToString())))
						{
							sb.Append(sub.Word);
							showWord = true;
						}
					}

					if (showWord)
						yield return sb.ToString();
					else
						yield return strNum 

					++num;

				}
			}
		}
#endif

#endregion
