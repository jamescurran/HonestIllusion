using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace NovelTheory
{
    public static class DataConvert
    {
        public static T ConvertTo<T>(object value, T defaultValue = default(T))
            where T : struct
        {
            if (value.Equals(DBNull.Value))
                return (T)defaultValue;
            else
                return (T)value;
        }

        public static T ReadAs<T>(this DataRow dr, string colName,T defaultValue = default(T))
            where T: struct
        {
            object value = dr[colName];
            return ConvertTo<T>(value, defaultValue);
        }

         public static List<T> BuildCollection<T>(this IDataReader rdr)
            where T : new()
        {
            List<T> ret = new List<T>();
            T entity;
            PropertyInfo[] props = typeof(T).GetProperties();

            while (rdr.Read())
            {
                entity = new T();
                foreach (PropertyInfo col in props)
                {
                    if (rdr[col.Name].Equals(DBNull.Value))
                        col.SetValue(entity, null, null);
                    else
                        col.SetValue(entity, rdr[col.Name], null);
                }
                ret.Add(entity);
            }
            return ret;
        }

    }
}
