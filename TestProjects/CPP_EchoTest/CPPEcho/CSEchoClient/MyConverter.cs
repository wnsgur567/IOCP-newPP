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

        // for string
        // only window // c++ wchar 2byte
        public static int WriteToBinStream(ref MemoryStream writestream, ref string s)
        {
            int write_size = 0;


            var bytes = Encoding.Unicode.GetBytes(s);
            int bytes_length = bytes.Length;
            // write string size
            writestream.Write(BitConverter.GetBytes(bytes_length));
            write_size += sizeof(int);

            // write string
            if (BitConverter.IsLittleEndian)
            {
                int wchar_size = 2;
                for (int i = 0; i < bytes.Length; i = i + wchar_size)
                {
                    Array.Reverse(bytes, i, wchar_size);
                }
            }

            writestream.Write(bytes);
            write_size = bytes.Length;

            return write_size;
        }

        // for string
        // only window // c++ wchar 2byte
        public static int ReadFromBinStream(ref MemoryStream readstream, out string item)
        {
            int read_size = 0;

            int bytes_length;
            ReadFromBinStream(ref readstream, out bytes_length);
            read_size += sizeof(int);

            byte[] bytes = new byte[bytes_length];
            readstream.Read(bytes);
            read_size += bytes.Length;

            if (BitConverter.IsLittleEndian)
            {
                int wchar_size = 2;
                for (int i = 0; i < bytes.Length; i = i + wchar_size)
                {
                    Array.Reverse(bytes, i, wchar_size);
                }
            }

            item = Encoding.Unicode.GetString(bytes);

            return read_size;
        }

        // write for iconvertable
        // can int... float .. double etc
        public static int WriteToBinStream<TItem>(ref MemoryStream writestream, ref TItem item) where TItem : IConvertible
        {
            var bytes = (byte[])typeof(BitConverter)
                             .GetMethod("GetBytes", new Type[] { item.GetType() })
                             .Invoke(null, new object[] { item });

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            writestream.Write(bytes);

            return bytes.Length;
        }

        #region Read fundamental
        public static int ReadFromBinStream(ref MemoryStream readstream, out char item)
        {
            byte[] bytes = new byte[sizeof(char)];
            int read_size = readstream.Read(bytes, 0, sizeof(char));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToChar(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out Int16 item)
        {
            byte[] bytes = new byte[sizeof(Int16)];
            int read_size = readstream.Read(bytes, 0, sizeof(Int16));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToInt16(bytes);

            return read_size;
        }
        public static int ReadFromBinStream(ref MemoryStream readstream, out UInt16 item)
        {
            byte[] bytes = new byte[sizeof(UInt16)];
            int read_size = readstream.Read(bytes, 0, sizeof(UInt16));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToUInt16(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out Int32 item)
        {
            byte[] bytes = new byte[sizeof(Int32)];
            int read_size = readstream.Read(bytes, 0, sizeof(Int32));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToInt32(bytes);

            return read_size;
        }
        public static int ReadFromBinStream(ref MemoryStream readstream, out UInt32 item)
        {
            byte[] bytes = new byte[sizeof(UInt32)];
            int read_size = readstream.Read(bytes, 0, sizeof(UInt32));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToUInt32(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out Int64 item)
        {
            byte[] bytes = new byte[sizeof(Int64)];
            int read_size = readstream.Read(bytes, 0, sizeof(Int64));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToInt64(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out UInt64 item)
        {
            byte[] bytes = new byte[sizeof(UInt64)];
            int read_size = readstream.Read(bytes, 0, sizeof(UInt64));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToUInt64(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out float item)
        {
            byte[] bytes = new byte[sizeof(float)];
            int read_size = readstream.Read(bytes, 0, sizeof(float));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToSingle(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out double item)
        {
            byte[] bytes = new byte[sizeof(double)];
            int read_size = readstream.Read(bytes, 0, sizeof(double));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToDouble(bytes);

            return read_size;
        }

        public static int ReadFromBinStream(ref MemoryStream readstream, out bool item)
        {
            byte[] bytes = new byte[sizeof(char)];
            int read_size = readstream.Read(bytes, 0, sizeof(char));

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            item = BitConverter.ToBoolean(bytes);

            return read_size;
        }

        #endregion



        public static int WriteToBinStream<TCollection, TItem>(MemoryStream writestream, ref TCollection collection)
            where TCollection : ICollection<TCollection> where TItem : IConvertible
        {
            int write_size = 0;
            byte[] bytes;

            int collection_len = collection.Count;
            bytes = BitConverter.GetBytes(collection_len);
            writestream.Write(bytes);
            foreach (var item in collection)
            {

                //BitConverter.GetBytes(item);
            }
            return 0;
        }
    }
}
