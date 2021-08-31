#define __DEBUG

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;


namespace CSTest
{

    class NetworkManagerClient
    {
        static int BUFSIZE = 512;
        static string IP = "127.0.0.1";
        static int PORT = 9000;

        TcpClient client;
        NetworkStream netstream;


        int sendstream_head;
        byte[] sendstream;  // TODO : -> send queue

        int recvstream_head;
        byte[] recvstream;

        public void __Initialize()
        {
            client = new TcpClient(IP, PORT);

            netstream = client.GetStream();
            recvstream = new byte[BUFSIZE];
            sendstream = new byte[BUFSIZE];

            //string msg = "Test Message for sending to Server";
            //byte[] sendbuf = Encoding.UTF8.GetBytes(msg);         

            //int recv_bytes = stream.Read(recvbuf, 0, recvbuf.Length);
            //string output = Encoding.UTF8.GetString(recvbuf);

            // create recv thread 
            Thread t1 = new Thread(RecvThread);
            t1.Start();

        }

        public void __Finalize()
        {
            netstream.Close();
            client.Close();
        }

        public void WriteToStream(byte[] pData, int size)
        {
            Array.Copy(pData, 0, sendstream, sendstream_head, size);
            sendstream_head += size;
            //pData.CopyTo(sendstream, sendstream_head);
        }

        public void Send()
        {
            netstream.Write(sendstream, 0, sendstream_head);
            sendstream_head = 0;
        }
        public void Recv(ref byte[] pData)
        {
            int recvbytes = netstream.Read(pData, 0, pData.Length);
        }

        void RecvThread()
        {
#if __DEBUG
            Console.WriteLine("RecvThread Start");
#endif
            while (true)
            {
                try
                {

                    int recv_bytes = netstream.Read(recvstream, 0, recvstream.Length);
                    if (recv_bytes == 0)
                    {
#if __DEBUG
                        Console.WriteLine("Recv end signal");
#endif
                        break;
                    }





                    // TODO : State

                    string test_string = Encoding.Unicode.GetString(recvstream);
                    Console.WriteLine(test_string);
                }
                catch (Exception)
                {

#if __DEBUG
                    Console.WriteLine("RecvThread exception");
#endif
                }
            }

#if __DEBUG
            Console.WriteLine("RecvThread End");
#endif
        }
    }



}
