using System;
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
            Cipher.CipherTest.SEED_KeySchedKey(userKey, m_pdwRoundKey);

            string test_string = @"This is a lot of 12 point text to test the
ocr code and see if it works on all types
of file format.

The quick brown dog jumped over the
lazy fox.The quick brown dog jumped
over the lazy fox.The quick brown dog
jumped over the lazy fox.The quick
brown dog jumped over the lazy fox.";

            int str_size = test_string.Length;
            byte[] str_bytes = Encoding.UTF8.GetBytes(test_string);

            Console.WriteLine(Encoding.UTF8.GetString(str_bytes));
            Console.WriteLine();

            Encryption(str_bytes, Convert.ToUInt32(str_size));
            Console.WriteLine(Encoding.UTF8.GetString(str_bytes));
            Console.WriteLine();

            Decryption(str_bytes, Convert.ToUInt32(str_size));
            Console.WriteLine(Encoding.UTF8.GetString(str_bytes));
            Console.WriteLine();





            // input plaintext to be encrypted
            //byte[] pbData = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            //         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

            //fixed (byte* ptr = pbData)
            //    Cipher.CipherTest.SEED_Encrtpy(ptr, m_pdwRoundKey);

            //for (int i = 0; i < 16; i++)
            //{
            //    Console.Write("{0} ", pbData[i]);
            //}
            //Console.WriteLine();

            //fixed (byte* ptr = pbData)
            //    Cipher.CipherTest.SEED_Decrypt(ptr, m_pdwRoundKey);


            //for (int i = 0; i < 16; i++)
            //{
            //    Console.Write("{0} ", pbData[i]);
            //}
            //Console.WriteLine();




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
                        Console.WriteLine("{0}", (ptr + complete_size)[i]);
                }
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
                        Console.WriteLine("{0}", (ptr + complete_size)[i]);
                }
#endif
                complete_size += BLOCK_SIZE;
            }

            return complete_size;
        }
    }
}
