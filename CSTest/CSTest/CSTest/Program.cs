#define __DEBUG

using System;
using System.Collections.Generic;
using System.Text;

namespace CSTest
{
    using DWORD = System.UInt32;
    using WORD = System.UInt16;

    class Program
    {
        static uint BLOCK_SIZE = 16;

        public static DWORD[] userKey = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 		// User secret key
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        public static DWORD[] m_pdwRoundKey = new DWORD[32];

        static unsafe void Main(string[] args)
        {
            //            Cipher.CipherTest.SEED_KeySchedKey(userKey, m_pdwRoundKey);

            //            string test_string = @"This is a lot of 12 point text to test the
            //ocr code and see if it works on all types
            //of file format.

            //The quick brown dog jumped over the
            //lazy fox.The quick brown dog jumped
            //over the lazy fox.The quick brown dog
            //jumped over the lazy fox.The quick
            //brown dog jumped over the lazy fox.";

            //            int str_size = test_string.Length;
            //            byte[] str_bytes = Encoding.UTF8.GetBytes(test_string);

            //            Console.WriteLine(Encoding.UTF8.GetString(str_bytes));
            //            Console.WriteLine();

            //            Encryption(str_bytes, Convert.ToUInt32(str_size));
            //            Console.WriteLine(Encoding.UTF8.GetString(str_bytes));
            //            Console.WriteLine();

            //            Decryption(str_bytes, Convert.ToUInt32(str_size));
            //            Console.WriteLine(Encoding.UTF8.GetString(str_bytes));
            //            Console.WriteLine();


            Cipher.CipherTest.SEED_KeySchedKey(userKey, m_pdwRoundKey);

            NetworkManagerClient client = new NetworkManagerClient();

            client.__Initialize();


            while (true)
            {
                if (false == DoSomthing(client))
                    break;
            }

            client.__Finalize();

            Console.WriteLine("\n---app end---");
        }

        public static bool DoSomthing(NetworkManagerClient client)
        {
            string text = "this is test text for networking";

            while (true)
            {
                var keyInfo = Console.ReadKey();
                if (keyInfo.Key == ConsoleKey.A)
                {



                    int packet_id = 1;
                    int state = 1;  // sign 기준 sign in

                    int str_size = text.Length;

                    

                    byte[] state_bytes = BitConverter.GetBytes(state);
                    byte[] str_length_bytes = BitConverter.GetBytes(str_size);                    
                    byte[] str_bytes = Encoding.Unicode.GetBytes(text);

                    byte[] combination = new byte[512]; // tmp size


                    // data ... state, strsize, str
                    state_bytes.CopyTo(combination, 0);
                    str_length_bytes.CopyTo(combination, sizeof(int));
                    str_bytes.CopyTo(combination, sizeof(int) + sizeof(int));

                    // data encryption
#if __DEBUG
                    for (int i = 0; i < sizeof(int) + sizeof(int) + str_size; i++)
                    {
                        Console.Write("{0:X} ", combination[i]);
                    }
                    Console.WriteLine();
#endif
                    //var size = Program.Encryption(combination, Convert.ToUInt32(sizeof(int) + sizeof(int) + str_size));
                    //int total_size = sizeof(int) + Convert.ToInt32(size);

                    //// total size
                    //byte[] packet_size_bytes = BitConverter.GetBytes(total_size);
                    //client.WriteToStream(packet_size_bytes, sizeof(int));
                    //// id
                    //byte[] id_bytes = BitConverter.GetBytes(packet_id);
                    //client.WriteToStream(id_bytes, sizeof(int));

                    //client.WriteToStream(combination, Convert.ToInt32(size));

                    // total size
                    byte[] total = BitConverter.GetBytes(sizeof(int) + sizeof(int) + str_size);
                    client.WriteToStream(total,sizeof(int));
                    // state
                    client.WriteToStream(state_bytes, sizeof(int));
                    // data
                    client.WriteToStream(str_length_bytes, sizeof(int));
                    client.WriteToStream(str_bytes, str_size);

                    client.Send();

                    //size =  Program.Decryption(combination, Convert.ToUInt32(sizeof(int) + sizeof(int) + str_size));
                }
                else if (keyInfo.Key == ConsoleKey.B)
                {
                    break;
                }
            }

            return false;
        }

        public static unsafe uint Encryption(byte[] pData, uint inSize)
        {
            uint complete_size = 0;

#if __DEBUG
            Console.WriteLine("Encryption...");
#endif

            while (complete_size < inSize)
            {
                fixed (byte* ptr = pData)
                    Cipher.CipherTest.SEED_Encrtpy(
                        ptr + complete_size,
                        m_pdwRoundKey);
#if __DEBUG
                for (int i = 0; i < BLOCK_SIZE; i++)
                {
                    fixed (byte* ptr = pData)
                        Console.Write("{0:X} ", (ptr + complete_size)[i]);
                }
                Console.WriteLine();
#endif
                complete_size += BLOCK_SIZE;
            }


            return complete_size;
        }

        public static unsafe uint Decryption(byte[] pData, uint inSize)
        {
            uint complete_size = 0;

#if __DEBUG
            Console.WriteLine("Decryption...");
#endif

            while (complete_size < inSize)
            {
                fixed (byte* ptr = pData)
                    Cipher.CipherTest.SEED_Decrypt(
                        ptr + complete_size,
                        m_pdwRoundKey);
#if __DEBUG
                for (int i = 0; i < BLOCK_SIZE; i++)
                {
                    fixed (byte* ptr = pData)
                        Console.Write("{0:X} ", (ptr + complete_size)[i]);
                }
                Console.WriteLine();
#endif
                complete_size += BLOCK_SIZE;
            }

            return complete_size;
        }
    }
}
