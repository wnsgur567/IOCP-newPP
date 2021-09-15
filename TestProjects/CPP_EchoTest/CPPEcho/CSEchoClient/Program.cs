using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.IO;
using System.Net;
using System.Net.Sockets;
using CSEchoClient;

namespace ConsoleTest
{
    class Program
    {
        static void Main(string[] args)
        {
            SerializeIntegerTest();
            //SerializeFloatingTest();
            //SerializeCharacterTest();
            //SerializeStringTest();
            //SerializeGenericTest();


            //SerializeLinkedListTest();
            //SerializeSetTest();
            //SerializeMapTest();

            //TestNetObject obj = new TestNetObject();
            //obj.SetInfo();

            //byte[] bytes = new byte[512];

            //MemoryStream stream = new MemoryStream();

            //stream.Position = sizeof(int);
            //int size = MyConverter.WriteToBinStream(stream, obj);
            //stream.Position = 0;
            //MyConverter.WriteToBinStream(stream, size);
            //stream.Position = 0;

            //MemoryStream read_stream = new MemoryStream();
            //read_stream.Write(stream.GetBuffer(), 0, size + sizeof(int));
            //read_stream.Position = 0;
            //int read_size;
            //MyConverter.ReadFromBinStream(read_stream, out read_size);
            //Console.WriteLine(read_size);

            //TestNetObject read_obj = new TestNetObject();            
            //MyConverter.ReadFromBinStream(read_stream, read_obj);

            //read_obj.Print();

            //int PORT = 3500;
            //string IP = "localhost";

            //NetworkStream NS = null;
            //StreamReader SR = null;
            //StreamWriter SW = null;
            //TcpClient client = null;
            //try
            //{
            //    client = new TcpClient(IP, PORT); //client 연결
            //    Console.WriteLine("{0}:{1}에 접속하였습니다.", IP, PORT);
            //    NS = client.GetStream(); // 소켓에서 메시지를 가져오는 스트림
            //    SR = new StreamReader(NS, Encoding.UTF8); // Get message
            //    SW = new StreamWriter(NS, Encoding.UTF8); // Send message


            //    string SendMessage = string.Empty;
            //    string GetMessage = string.Empty;


            //    TestNetObject obj = new TestNetObject();
            //    obj.SetInfo();

            //    MemoryStream stream = new MemoryStream();

            //    stream.Position = sizeof(int);
            //    int size = MyConverter.WriteToBinStream(stream, obj);
            //    stream.Position = 0;
            //    MyConverter.WriteToBinStream(stream, size);
            //    stream.Position = 0;

            //    var aa = stream.GetBuffer();
            //    NS.Write(stream.GetBuffer(), 0, sizeof(int) + size);
            //    Console.WriteLine("write size : {0}", sizeof(int) + size);

            //    byte[] read_bytes = new byte[4];
            //    NS.Read(read_bytes, 0, sizeof(int));
            //    int read_size = BitConverter.ToInt32(read_bytes);

            //    byte[] read_bytes_data = new byte[read_size];
            //    Console.WriteLine("write size : {0}", read_size);
            //    NS.Read(read_bytes_data, 0, read_size);
            //    MemoryStream read_stream = new MemoryStream();
            //    read_stream.Write(read_bytes_data, 0, read_size);
            //    read_stream.Position = 0;

            //    TestNetObject read_obj = new TestNetObject();
            //    MyConverter.ReadFromBinStream(read_stream, read_obj);

            //    read_obj.Print();

            //}
            //catch (Exception e)
            //{
            //    System.Console.WriteLine(e.Message);
            //}
            //finally
            //{
            //    if (SW != null) SW.Close();
            //    if (SR != null) SR.Close();
            //    if (client != null) client.Close();
            //}
        }


        static void SerializeGenericTest()
        {
            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();


            float x = 10.8546f;
            float x_out;

            write_size = MyConverter.WriteToBinStream(write_stream, x);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(float));
            read_stream.Position = 0;
            read_size = MyConverter.ReadFromBinStream(read_stream, out x_out);

            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", x);
            Console.WriteLine("output : {0}", x_out);
        }

        static void ListTest()
        {
            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();

            // int
            List<int> int_list = new List<int>();
            int_list.Add(10);
            int_list.Add(22);
            int_list.Add(-973621);
            int_list.Add(6421);
            int_list.Add(892582);

            write_stream.Position = 0;
            write_size = MyConverter.WriteToBinStream(write_stream, int_list);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            List<int> int_list_out = null;
            read_size = MyConverter.ReadFromBinStream(read_stream, out int_list_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("int_list : ");
            foreach (var item in int_list_out)
            {
                Console.Write("{0} ", item);
            }
            Console.WriteLine();

            // string
            List<string> string_list = new List<string>();
            string_list.Add("asdfsdfsd");
            string_list.Add("7897987987");
            string_list.Add("876546546xcdvdfvxgrfsgsgrse4r6t5rs4d6f8y7sdty");

            write_stream.Position = 0;
            write_size = MyConverter.WriteToBinStream(write_stream, string_list);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            List<string> string_list_out = null;
            read_size = MyConverter.ReadFromBinStream(read_stream, out string_list_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("string_list : ");
            foreach (var item in string_list_out)
            {
                Console.Write("{0} ", item);
            }
            Console.WriteLine();

            // float
            List<float> float_list = new List<float>();
            float_list.Add(1.25167f);
            float_list.Add(0.00274f);
            float_list.Add(87654.3f);

            write_stream.Position = 0;
            write_size = MyConverter.WriteToBinStream(write_stream, float_list);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            List<float> float_list_out = null;
            read_size = MyConverter.ReadFromBinStream(read_stream, out float_list_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("float_list : ");
            foreach (var item in float_list_out)
            {
                Console.Write("{0} ", item);
            }
            Console.WriteLine();
        }

        static void SerializeLinkedListTest()
        {
            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();

            // int
            LinkedList<int> int_list = new LinkedList<int>();
            int_list.AddLast(10);
            int_list.AddLast(22);
            int_list.AddLast(-973621);
            int_list.AddLast(6421);
            int_list.AddLast(892582);

            write_stream.Position = 0;
            write_size = MyConverter.WriteToBinStream(write_stream, int_list);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            LinkedList<int> int_list_out = null;
            read_size = MyConverter.ReadFromBinStream(read_stream, out int_list_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("int_list : ");
            foreach (var item in int_list_out)
            {
                Console.Write("{0} ", item);
            }
            Console.WriteLine();
        }
        static void SerializeSetTest()
        {
            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();

            // int
            HashSet<int> int_set = new HashSet<int>();
            int_set.Add(103099348);
            int_set.Add(22);
            int_set.Add(-973621);
            int_set.Add(6421);
            int_set.Add(892582);

            write_stream.Position = 0;
            write_size = MyConverter.WriteToBinStream(write_stream, int_set);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            LinkedList<int> int_set_out = null;
            read_size = MyConverter.ReadFromBinStream(read_stream, out int_set_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("int_list : ");
            foreach (var item in int_set_out)
            {
                Console.Write("{0} ", item);
            }
            Console.WriteLine();
        }

        static void SerializeMapTest()
        {
            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();


            // pair

            // double string
            Dictionary<double, string> dic = new Dictionary<double, string>();
            dic.Add(123.584, "ttetestsetset");
            dic.Add(9832.5445649, "popopopop");
            dic.Add(0.00215, "serializeserializeserializeserializeserializeserialize");
            dic.Add(-0.654, "00");
            dic.Add(0.0001, "dfdf\tdfo\t");

            write_stream.Position = 0;
            write_size = MyConverter.WriteToBinStream(write_stream, dic);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            Dictionary<double, string> dic_out;
            read_size = MyConverter.ReadFromBinStream(read_stream, out dic_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("map : ");
            foreach (var item in dic_out)
            {
                Console.WriteLine("({0}, {1}) ", item.Key, item.Value);
            }
            Console.WriteLine();
        }



        static void SerializeIntegerTest()
        {
            Int16 integer16 = Convert.ToInt16(-56);
            Int16 integer16_out;
            UInt16 uinteger16 = Convert.ToUInt16(26);
            UInt16 uinteger16_out;
            Int32 integer32 = Convert.ToInt32(54465654);
            Int32 integer32_out;
            UInt32 uinteger32 = Convert.ToUInt32(654687765);
            UInt32 uinteger32_out;
            Int64 integer64 = Convert.ToInt64(12312448314162);
            Int64 integer64_out;
            UInt64 uinteger64 = Convert.ToUInt64(1377312448162);
            UInt64 uinteger64_out;

            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();


            // int 16
            write_size = MyConverter.WriteToBinStream(write_stream, integer16);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int16));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream<Int16>(read_stream, out integer16_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", integer16);
            Console.WriteLine("output : {0}", integer16_out);
            Console.WriteLine();

            // uint 16
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, uinteger16);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(UInt16));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out uinteger16_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", uinteger16);
            Console.WriteLine("output : {0}", uinteger16_out);
            Console.WriteLine();

            // int 32
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, integer32);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int32));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out integer32_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", integer32);
            Console.WriteLine("output : {0}", integer32_out);
            Console.WriteLine();

            // uint 32   
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, uinteger32);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(UInt32));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out uinteger32_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", uinteger32);
            Console.WriteLine("output : {0}", uinteger32_out);
            Console.WriteLine();

            // int 64
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, integer64);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int64));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out integer64_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", integer64);
            Console.WriteLine("output : {0}", integer64_out);
            Console.WriteLine();

            // uint 64
            write_stream.Position = 0;


            write_size = MyConverter.WriteToBinStream(write_stream, uinteger64);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int64));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out uinteger64_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", uinteger64);
            Console.WriteLine("output : {0}", uinteger64_out);
            Console.WriteLine();

        }

        static void SerializeFloatingTest()
        {
            float float_1 = 131465.12f;
            float float_1_out;
            float float_2 = -0.00257f;
            float float_2_out;

            double double_1 = 987.5421678253;
            double double_1_out;
            double double_2 = -0.000064954;
            double double_2_out;


            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();

            // float 1 
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, float_1);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(float));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out float_1_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", float_1);
            Console.WriteLine("output : {0}", float_1_out);
            Console.WriteLine();

            // float 2
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, float_2);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(float));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out float_2_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", float_2);
            Console.WriteLine("output : {0}", float_2_out);
            Console.WriteLine();

            // double 1
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, double_1);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(double));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out double_1_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", double_1);
            Console.WriteLine("output : {0}", double_1_out);
            Console.WriteLine();

            // double 2
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, double_2);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(double));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out double_2_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", double_2);
            Console.WriteLine("output : {0}", double_2_out);
            Console.WriteLine();
        }
        static void SerializeCharacterTest()
        {
            char c = 'd';
            char d;

            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();

            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, c);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(char));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out d);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", c);
            Console.WriteLine("output : {0}", d);
            Console.WriteLine();
        }

        static void SerializeStringTest()
        {
            string str1 = "this is test for endian convert and serialization";
            string str1_out;

            int write_size = 0;
            int read_size = 0;

            MemoryStream write_stream = new MemoryStream();
            MemoryStream read_stream = new MemoryStream();

            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(write_stream, str1);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(read_stream, out str1_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("strlen : {0}", str1.Length);
            Console.WriteLine("input : {0}", str1);
            Console.WriteLine("output : {0}", str1_out);
            Console.WriteLine();
        }
    }
}
