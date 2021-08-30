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
        NetworkStream stream;
        byte[] recvstream;

        public void __Initialize()
        {
            client = new TcpClient(IP, PORT);

            stream = client.GetStream();
            recvstream = new byte[BUFSIZE];

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
            stream.Close();
            client.Close();
        }

        public void Send(byte[] pData)
        {
            stream.Write(pData, 0, pData.Length);
        }
        public void Recv(ref byte[] pData)
        {
            int recvbytes = stream.Read(pData, 0, pData.Length);
        }

        void RecvThread()
        {

            while (true)
            {
                try
                {
                    if(stream.DataAvailable)
                    {
                        int recv_bytes = stream.Read(recvstream, 0, recvstream.Length);
                        if (recv_bytes == 0)
                        {
#if __DEBUG
                        Console.WriteLine("Recv end signal");                        
#endif
                            break;
                        }


                    }


                    // TODO : State

                    string test_string = Encoding.Unicode.GetString(recvstream);
                    Console.WriteLine(test_string);
                }
                catch (Exception)
                {


                }
            }
        }
    }
}
