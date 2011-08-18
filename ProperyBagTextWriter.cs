
#region License
// Copyright (c) 2009, James M. Curran
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#endregion

namespace Castle.MonoRail.Framework.Internal
{
	using System.Collections;
	using System.IO;

	/// <summary>
	/// 
	/// </summary>
    class ProperyBagTextWriter : StringWriter
    {
        string key;
        IDictionary bag;

		/// <summary>
		/// Initializes a new instance of the <see cref="ProperyBagTextWriter"/> class.
		/// </summary>
		/// <param name="key">The key.</param>
		/// <param name="bag">The bag.</param>
        public ProperyBagTextWriter(string key, IDictionary bag)
        {
            this.key = key;
            this.bag = bag;
        }

		/// <summary>
		/// Clears all buffers for the current writer and causes any buffered data to be written to the underlying device.
		/// </summary>
        public override void Flush()
        {
            base.Flush();
            bag[key] = base.ToString();
        }
		/// <summary>
		/// Writes the specified region of a character array to this instance of the StringWriter.
		/// </summary>
		/// <param name="buffer">The character array to read data from.</param>
		/// <param name="index">The index at which to begin reading from <paramref name="buffer"/>.</param>
		/// <param name="count">The maximum number of characters to write.</param>
		/// <exception cref="T:System.ArgumentNullException">
		/// 	<paramref name="buffer"/> is null.
		/// </exception>
		/// <exception cref="T:System.ArgumentOutOfRangeException">
		/// 	<paramref name="index"/> or <paramref name="count"/> is negative.
		/// </exception>
		/// <exception cref="T:System.ArgumentException">
		/// (<paramref name="index"/> + <paramref name="count"/>)&gt; <paramref name="buffer"/>. Length.
		/// </exception>
		/// <exception cref="T:System.ObjectDisposedException">
		/// The writer is closed.
		/// </exception>
        public override void Write(char[] buffer, int index, int count)
        {
            base.Write(buffer, index, count);
            Flush();
        }
		/// <summary>
		/// Releases the unmanaged resources used by the <see cref="T:System.IO.StringWriter"/> and optionally releases the managed resources.
		/// </summary>
		/// <param name="disposing">true to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
		protected override void Dispose(bool disposing)
		{
			base.Dispose(disposing);
			if (disposing)
				Flush();
		}
    }
}
