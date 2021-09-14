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
            //SerializeIntegerTest();
            //SerializeFloatingTest();
            SerializeCharacterTest();
            SerializeStringTest();

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

            //    while ((SendMessage = Console.ReadLine()) != null)
            //    {
            //        SW.WriteLine(SendMessage); // 메시지 보내기
            //        SW.Flush();

            //        GetMessage = SR.ReadLine();
            //        Console.WriteLine(GetMessage);
            //    }
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
            write_size = MyConverter.WriteToBinStream(ref write_stream, ref integer16);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int16));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out integer16_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", integer16);
            Console.WriteLine("output : {0}", integer16_out);
            Console.WriteLine();

            // uint 16
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref uinteger16);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(UInt16));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out uinteger16_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", uinteger16);
            Console.WriteLine("output : {0}", uinteger16_out);
            Console.WriteLine();

            // int 32
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref integer32);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int32));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out integer32_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", integer32);
            Console.WriteLine("output : {0}", integer32_out);
            Console.WriteLine();

            // uint 32   
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref uinteger32);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(UInt32));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out uinteger32_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", uinteger32);
            Console.WriteLine("output : {0}", uinteger32_out);
            Console.WriteLine();

            // int 64
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref integer64);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int64));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out integer64_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", integer64);
            Console.WriteLine("output : {0}", integer64_out);
            Console.WriteLine();

            // uint 64
            write_stream.Position = 0;


            write_size = MyConverter.WriteToBinStream(ref write_stream, ref uinteger64);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(Int64));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out uinteger64_out);
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

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref float_1);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(float));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out float_1_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", float_1);
            Console.WriteLine("output : {0}", float_1_out);
            Console.WriteLine();

            // float 2
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref float_2);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(float));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out float_2_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", float_2);
            Console.WriteLine("output : {0}", float_2_out);
            Console.WriteLine();

            // double 1
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref double_1);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(double));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out double_1_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", double_1);
            Console.WriteLine("output : {0}", double_1_out);
            Console.WriteLine();

            // double 2
            write_stream.Position = 0;

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref double_2);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(double));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out double_2_out);
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

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref c);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, sizeof(char));
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out d);
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

            write_size = MyConverter.WriteToBinStream(ref write_stream, ref str1);
            Console.WriteLine("Write bytes : {0}", write_size);

            read_stream.Position = 0;
            read_stream.Write(write_stream.GetBuffer(), 0, write_size);
            read_stream.Position = 0;

            read_size = MyConverter.ReadFromBinStream(ref read_stream, out str1_out);
            Console.WriteLine("Read bytes : {0}", read_size);

            Console.WriteLine("input : {0}", str1);
            Console.WriteLine("output : {0}", str1_out);
            Console.WriteLine();
        }
    }
}
