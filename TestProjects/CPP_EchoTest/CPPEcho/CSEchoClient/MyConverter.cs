using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.Emit;
using System.Text;

namespace CSEchoClient
{
    public static class GlobalExtensions
    {
        // https://stackoverflow.com/questions/18167216/size-of-generic-structure
        public static int SizeOf<T>()
        {
            return SizeOf(typeof(T));
        }

        public static int SizeOf(this Type type)
        {
            var dynamicMethod = new DynamicMethod("SizeOf", typeof(int), Type.EmptyTypes);
            var generator = dynamicMethod.GetILGenerator();

            generator.Emit(OpCodes.Sizeof, type);
            generator.Emit(OpCodes.Ret);

            var function = (Func<int>)dynamicMethod.CreateDelegate(typeof(Func<int>));
            return function();
        }
    }

    class MyConverter
    {
        // write for iconvertable
        // for integer
        public static int WriteToBinStream<TItem>(MemoryStream writestream, TItem item) where TItem : IConvertible
        {
            if (typeof(string) == typeof(TItem))
                return WriteToBinStream(writestream, (string)((Object)item));

            var bytes = (byte[])typeof(BitConverter)
                             .GetMethod("GetBytes", new Type[] { item.GetType() })
                             .Invoke(null, new object[] { item });

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            writestream.Write(bytes);

            return bytes.Length;
        }

        public static int WriteToBinStream(MemoryStream writestream, float item)
        {
            var bytes = BitConverter.GetBytes(item);

            //if(BitConverter.IsLittleEndian)
            //{
            //    Array.Reverse(bytes);
            //}

            writestream.Write(bytes);

            return bytes.Length;
        }
        public static int WriteToBinStream(MemoryStream writestream, double item)
        {
            var bytes = BitConverter.GetBytes(item);

            //if (BitConverter.IsLittleEndian)
            //{
            //    Array.Reverse(bytes);
            //}

            writestream.Write(bytes);

            return bytes.Length;
        }

        #region Read fundamental
        public static int ReadFromBinStream(MemoryStream readstream, out char item)
        {
            byte[] bytes = new byte[sizeof(char)];
            int read_size = readstream.Read(bytes, 0, sizeof(char));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToChar(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(MemoryStream readstream, out Int16 item)
        {
            byte[] bytes = new byte[sizeof(Int16)];
            int read_size = readstream.Read(bytes, 0, sizeof(Int16));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToInt16(bytes);

            return read_size;
        }
        public static int ReadFromBinStream(MemoryStream readstream, out UInt16 item)
        {
            byte[] bytes = new byte[sizeof(UInt16)];
            int read_size = readstream.Read(bytes, 0, sizeof(UInt16));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToUInt16(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(MemoryStream readstream, out Int32 item)
        {
            byte[] bytes = new byte[sizeof(Int32)];
            int read_size = readstream.Read(bytes, 0, sizeof(Int32));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToInt32(bytes);

            return read_size;
        }
        public static int ReadFromBinStream(MemoryStream readstream, out UInt32 item)
        {
            byte[] bytes = new byte[sizeof(UInt32)];
            int read_size = readstream.Read(bytes, 0, sizeof(UInt32));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToUInt32(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(MemoryStream readstream, out Int64 item)
        {
            byte[] bytes = new byte[sizeof(Int64)]; 
            int read_size = readstream.Read(bytes, 0, sizeof(Int64));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToInt64(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(MemoryStream readstream, out UInt64 item)
        {
            byte[] bytes = new byte[sizeof(UInt64)];
            int read_size = readstream.Read(bytes, 0, sizeof(UInt64));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToUInt64(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(MemoryStream readstream, out float item)
        {
            byte[] bytes = new byte[sizeof(float)];
            int read_size = readstream.Read(bytes, 0, sizeof(float));

            //if (BitConverter.IsLittleEndian)
            //{
            //    Array.Reverse(bytes);
            //}

            item = BitConverter.ToSingle(bytes);

            return read_size;
        }
       

        public static int ReadFromBinStream(MemoryStream readstream, out double item)
        {
            byte[] bytes = new byte[sizeof(double)];
            int read_size = readstream.Read(bytes, 0, sizeof(double));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToDouble(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(MemoryStream readstream, out bool item)
        {
            byte[] bytes = new byte[sizeof(char)];
            int read_size = readstream.Read(bytes, 0, sizeof(char));

            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes);

            item = BitConverter.ToBoolean(bytes);

            return read_size;
        }

        #endregion

        #region string read write

        // for string
        // only window // c++ wchar 2byte
        public static int WriteToBinStream(MemoryStream writestream, string s)
        {
            int write_size = 0;
            var bytes = Encoding.Unicode.GetBytes(s);

            // write string size
            int bytes_length = bytes.Length;
            var bytes_length_bytes = BitConverter.GetBytes(bytes_length);
            //if (BitConverter.IsLittleEndian)
            //    Array.Reverse(bytes_length_bytes);
            writestream.Write(bytes_length_bytes);
            write_size += sizeof(int);

            // write string
            //if (BitConverter.IsLittleEndian)
            //{
            //    int wchar_size = 2;
            //    for (int i = 0; i < bytes.Length; i = i + wchar_size)
            //    {
            //        Array.Reverse(bytes, i, wchar_size);
            //    }
            //}

            writestream.Write(bytes);
            write_size += bytes.Length;

            return write_size;
        }

        // for string
        // only window // c++ wchar 2byte
        public static int ReadFromBinStream(MemoryStream readstream, out string item)
        {
            int read_size = 0;

            // str length
            int bytes_length;
            read_size += ReadFromBinStream(readstream, out bytes_length);

            byte[] bytes = new byte[bytes_length];
            readstream.Read(bytes);
            read_size += bytes.Length;

            //if (BitConverter.IsLittleEndian)
            //{
            //    int wchar_size = 2;
            //    for (int i = 0; i < bytes.Length; i = i + wchar_size)
            //    {
            //        Array.Reverse(bytes, i, wchar_size);
            //    }
            //}

            item = Encoding.Unicode.GetString(bytes);

            return read_size;
        }

        #endregion

        #region container

        //public static T ConvertTo<T>(byte[] bytes, int offset = 0) where T : IConvertible
        //{
        //    //https://codereview.stackexchange.com/questions/101636/performance-byte-to-generic
        //    var type = typeof(T);
        //    if (type == typeof(sbyte)) return (T)(object)((sbyte)bytes[offset]);
        //    if (type == typeof(byte)) return (T)(object)bytes[offset];
        //    if (type == typeof(short)) return (T)(object)((short)(bytes[offset + 1] << 8 | bytes[offset]));
        //    if (type == typeof(ushort)) return (T)(object)((ushort)(bytes[offset + 1] << 8 | bytes[offset]));
        //    if (type == typeof(int)) return (T)(object)(bytes[offset + 3] << 24 | bytes[offset + 2] << 16 | bytes[offset + 1] << 8 | bytes[offset]);
        //    if (type == typeof(uint)) return (T)(object)((uint)bytes[offset + 3] << 24 | (uint)bytes[offset + 2] << 16 | (uint)bytes[offset + 1] << 8 | bytes[offset]);
        //    if (type == typeof(long)) return (T)(object)((long)bytes[offset + 7] << 56 | (long)bytes[offset + 6] << 48 | (long)bytes[offset + 5] << 40 | (long)bytes[offset + 4] << 32 | (long)bytes[offset + 3] << 24 | (long)bytes[offset + 2] << 16 | (long)bytes[offset + 1] << 8 | bytes[offset]);
        //    if (type == typeof(ulong)) return (T)(object)((ulong)bytes[offset + 7] << 56 | (ulong)bytes[offset + 6] << 48 | (ulong)bytes[offset + 5] << 40 | (ulong)bytes[offset + 4] << 32 | (ulong)bytes[offset + 3] << 24 | (ulong)bytes[offset + 2] << 16 | (ulong)bytes[offset + 1] << 8 | bytes[offset]);

        //    throw new NotImplementedException();
        //}

        public static int ReadFromBinStream<TItem>(MemoryStream readstream, out TItem item) where TItem : IConvertible
        {
            var type = typeof(TItem);
            int size = 0;
            if (type == typeof(byte))
            {
                byte retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(Boolean))
            {
                Boolean retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(char))
            {
                char retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(Int16))
            {
                Int16 retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(UInt16))
            {
                UInt16 retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(Int32))
            {
                Int32 retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(UInt32))
            {
                UInt32 retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(Int64))
            {
                Int64 retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(UInt64))
            {
                UInt64 retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(float))
            {
                float retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(double))
            {
                double retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }
            if (type == typeof(string))
            {
                string retval;
                size = ReadFromBinStream(readstream, out retval);
                item = (TItem)((object)retval);
                return size;
            }

            throw new NotImplementedException();
        }


        // collection write
        public static int WriteToBinStream<TItem>(MemoryStream writestream, ICollection<TItem> collection)
           where TItem : IConvertible
        {
            int write_size = 0;

            // collection length
            int collection_len = collection.Count;
            write_size += WriteToBinStream(writestream, collection_len);

            // cotainer items
            foreach (var item in collection)
            {
                write_size += WriteToBinStream(writestream, item);
            }

            return write_size;
        }
        public static int WritToBinStream<TKey, TValue>(MemoryStream writestream, KeyValuePair<TKey, TValue> pair) where TKey : IConvertible where TValue : IConvertible
        {
            int first_write_size = WriteToBinStream(writestream, pair.Key);
            int second_write_size = WriteToBinStream(writestream, pair.Value);
            return first_write_size + second_write_size;
        }

        public static int WriteToBinStream<TKey, TValue>(MemoryStream writestream, ICollection<KeyValuePair<TKey, TValue>> collection)
             where TKey : IConvertible where TValue : IConvertible
        {
            int read_size = 0;

            // length
            int collection_len = collection.Count;
            read_size = WriteToBinStream(writestream, collection_len);

            // data
            foreach (var item in collection)
            {
                read_size += WriteToBinStream(writestream, item.Key);
                read_size += WriteToBinStream(writestream, item.Value);
            }
            return read_size;
        }

        // read list
        public static int ReadFromBinStream<TItem>(MemoryStream readstream, out List<TItem> collection) where TItem : IConvertible
        {
            int read_size = 0;

            int collection_len;
            read_size = ReadFromBinStream(readstream, out collection_len);

            collection = new List<TItem>(collection_len);
            for (int i = 0; i < collection_len; i++)
            {
                TItem outItem;
                read_size += ReadFromBinStream(readstream, out outItem);
                collection.Add(outItem);
            }
            return read_size;
        }

        // read linked list
        public static int ReadFromBinStream<TItem>(MemoryStream readstream, out LinkedList<TItem> collection) where TItem : IConvertible
        {
            int read_size = 0;

            int collection_len;
            read_size = ReadFromBinStream(readstream, out collection_len);

            collection = new LinkedList<TItem>();
            for (int i = 0; i < collection_len; i++)
            {
                TItem outItem;
                read_size += ReadFromBinStream(readstream, out outItem);
                collection.AddLast(outItem);
            }
            return read_size;
        }

        // read hashset
        public static int ReadFromBinStream<TItem>(MemoryStream readstream, out HashSet<TItem> collection) where TItem : IConvertible
        {
            int read_size = 0;

            int collection_len;
            read_size = ReadFromBinStream(readstream, out collection_len);

            collection = new HashSet<TItem>(collection_len);
            for (int i = 0; i < collection_len; i++)
            {
                TItem outItem;
                read_size += ReadFromBinStream(readstream, out outItem);
                collection.Add(outItem);
            }
            return read_size;
        }

        // read sortedset
        public static int ReadFromBinStream<TItem>(MemoryStream readstream, out SortedSet<TItem> collection) where TItem : IConvertible
        {
            int read_size = 0;

            int collection_len;
            read_size = ReadFromBinStream(readstream, out collection_len);

            collection = new SortedSet<TItem>();
            for (int i = 0; i < collection_len; i++)
            {
                TItem outItem;
                read_size += ReadFromBinStream(readstream, out outItem);
                collection.Add(outItem);
            }
            return read_size;
        }

        // read key value pair
        public static int ReadFromBinStream<TKey, TValue>(MemoryStream readstream, out KeyValuePair<TKey, TValue> collection) where TKey : IConvertible where TValue : IConvertible
        {
            int read_size = 0;

            TKey outKey;
            TValue outValue;
            read_size += ReadFromBinStream(readstream, out outKey);
            read_size += ReadFromBinStream(readstream, out outValue);

            collection = new KeyValuePair<TKey, TValue>(outKey, outValue);

            return read_size;
        }

        public static int ReadFromBinStream<TKey, TValue>(MemoryStream readstream, out Dictionary<TKey, TValue> collection) where TKey : IConvertible where TValue : IConvertible
        {
            int read_size = 0;

            int collection_len;
            read_size = ReadFromBinStream(readstream, out collection_len);

            collection = new Dictionary<TKey, TValue>(collection_len);
            for (int i = 0; i < collection_len; i++)
            {
                TKey outKey;
                TValue outValue;
                read_size += ReadFromBinStream(readstream, out outKey);
                read_size += ReadFromBinStream(readstream, out outValue);

                collection.Add(outKey, outValue);
            }
            return read_size;
        }
        #endregion

        public static int WriteToBinStream(MemoryStream writestream, ISerializable serializable) 
        {
            return serializable.Serialize(writestream);
        }
        public static int ReadFromBinStream(MemoryStream readstream, ISerializable serializable)
        {
            return serializable.DeSerialize(readstream);           
        }
    }
}
