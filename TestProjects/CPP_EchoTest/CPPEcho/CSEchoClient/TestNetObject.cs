using System;
using System.Collections.Generic;
using System.IO;
using System.Text;



namespace CSEchoClient
{
    interface ISerializable
    {
        abstract int Serialize(MemoryStream stream);
        abstract int DeSerialize(MemoryStream stream);
    }

    class TestMemberClass : ISerializable
    {
        float x;
        float y;
        float z;
        Int32 a;
        public TestMemberClass()
        {
            x = 123.123f;
            y = 0.4874f;
            z = 7154.454f;
            a = 1;
        }

        public int DeSerialize(MemoryStream stream)
        {
            int size = 0;
            size += MyConverter.ReadFromBinStream(stream, out x);
            size += MyConverter.ReadFromBinStream(stream, out y);
            size += MyConverter.ReadFromBinStream(stream, out z);
            size += MyConverter.ReadFromBinStream(stream, out a);
            return size;
        }

        public int Serialize(MemoryStream stream)
        {
            int size = 0;
            size += MyConverter.WriteToBinStream(stream, x);
            size += MyConverter.WriteToBinStream(stream, y);
            size += MyConverter.WriteToBinStream(stream, z);
            size += MyConverter.WriteToBinStream(stream, a);
            return size;
        }

        public void Print()
        {
            Console.WriteLine("{0}, {1}, {2}", x, y, z);
        }
    }

    class TestNetObject : ISerializable
    {
        UInt64 m_int64;
        float m_float;
        string m_str;
        List<Int32> integer_vec;
        Dictionary<int, string> int_to_wstr_map;
        TestMemberClass class_ptr;

        public TestNetObject()
        {
            class_ptr = new TestMemberClass();
        }

        public int DeSerialize(MemoryStream stream)
        {
            int size = 0;
            size += MyConverter.ReadFromBinStream(stream, out m_int64);
            size += MyConverter.ReadFromBinStream(stream, out m_float);
            size += MyConverter.ReadFromBinStream(stream, out m_str);
            size += MyConverter.ReadFromBinStream(stream, out integer_vec);
            size += MyConverter.ReadFromBinStream(stream, out int_to_wstr_map);
            size += MyConverter.ReadFromBinStream(stream, class_ptr);
            return size;
        }

        public int Serialize(MemoryStream stream)
        {
            int size = 0;            
            size += MyConverter.WriteToBinStream(stream, m_int64);
            size += MyConverter.WriteToBinStream(stream, m_float);
            size += MyConverter.WriteToBinStream(stream, m_str);
            size += MyConverter.WriteToBinStream(stream, integer_vec);
            size += MyConverter.WriteToBinStream(stream, int_to_wstr_map);
            size += MyConverter.WriteToBinStream(stream, class_ptr);
            return size;
        }

        public void SetInfo()
        {
            integer_vec = new List<int>();
            int_to_wstr_map = new Dictionary<int, string>();

            m_int64 = 12198247U;
            m_float = 0.200654f;
            m_str = "hello world!! this is client!!";
            integer_vec.Add(1022);
            integer_vec.Add(90456);
            integer_vec.Add(7078);
            integer_vec.Add(50445);
            integer_vec.Add(44232);
            int_to_wstr_map.Add(8, "kdkdkdkdk");
            int_to_wstr_map.Add(4, "icicicicicicic oc");
            int_to_wstr_map.Add(111, "pipipipipipipip");
        }

        public void Print()
        {
            Console.WriteLine("==== print");
            Console.WriteLine("{0}", m_int64);
            Console.WriteLine("{0}", m_float);
            Console.WriteLine("{0}", m_str);

            Console.WriteLine("vector : ");
            foreach (var item in integer_vec)
            {
                Console.WriteLine("{0} ", item);
            }
            Console.WriteLine();

            Console.WriteLine("map : ");
            foreach (var item in int_to_wstr_map)
            {
                Console.WriteLine("[ {0} , {1} ]", item.Key, item.Value);
            }
            Console.WriteLine();

            class_ptr.Print();

            Console.WriteLine("==== print end\n");
        }

    }



}
