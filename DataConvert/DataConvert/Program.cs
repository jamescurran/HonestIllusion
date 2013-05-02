using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace NovelTheory
{
    class Program
    {
        static void Main(string[] args)
        {

            // Setup for ConvertTo
            object a = 1;
            object b = DBNull.Value;

            // For ReadAs().
            //  Let's create a fake DataRow instead of reading a real database.
            var dt = new DataTable();
            dt.Columns.Add(new DataColumn("Price", typeof(decimal)));
            dt.Columns.Add(new DataColumn("Quantity", typeof(int)));
            dt.Columns.Add(new DataColumn("NullInt", typeof(int)));
            var dr = dt.NewRow();
            dr.ItemArray = new object[3] { 123.45, 12345, null };

            // For ReadCollection, we'll mock up a DataReader
            var rdr = new MockDataReader();
            rdr.Add(0, "MyNumber", 111).Add(0, "MyDecimal", 111.11m).Add(0, "AnotherInt", -100);
            rdr.Add(1, "MyNumber", 222).Add(1, "MyDecimal", 222.22m).Add(1, "AnotherInt", -200);
            rdr.Add(2, "MyNumber", 333).Add(2, "MyDecimal", 333.33m).Add(2, "AnotherInt", -300);
            // OK, hand-waving done.....

            // Here we start the actual tests.....

            Console.WriteLine(DataConvert.ConvertTo<int>(a, 100));      // Prints "1" (value of a)
            Console.WriteLine(DataConvert.ConvertTo<int>(b, 100));      // Prints "100" (default)
//            Console.WriteLine(DataConvert.ConvertTo<int>(b, 100.234));    // would be compiler error.
            Console.WriteLine(DataConvert.ConvertTo(b, 100));               // prints 100
            Console.WriteLine(DataConvert.ConvertTo<int>(b));               // prints "0" (default for ints)

            Console.WriteLine("----");

            Console.WriteLine(dr.ReadAs<decimal>("Price"));         // print 123.45
            Console.WriteLine(dr.ReadAs<int>("Quantity"));          // prints 12345
            Console.WriteLine(dr.ReadAs<int>("NullInt", 10));       // prints 10 (default value)

            Console.WriteLine("----");

            List<TestObj> coll = rdr.BuildCollection<TestObj>();

            foreach (var c in coll)
                Console.WriteLine(c);           // prints constructed TestObj objects using TestObj.ToString()
        
        }
    }

    class TestObj
    {
        public int MyNumber { get; set; }
        public decimal MyDecimal { get; set; }
        public int AnotherInt { get; set; }

        public override string ToString()
        {
            return String.Format("MyN;{0}, MyD:{1}, AnI:{2}", MyNumber, MyDecimal, AnotherInt);
        }
    }
}
