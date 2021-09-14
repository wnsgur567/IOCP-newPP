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

    class TestNetObject : ISerializable
    {
        UInt64 sign_id;
        string id;
        string pw;
        List<Int32> integer_vec;

        public TestNetObject()
        {
            integer_vec = new List<int>();
        }

        public int DeSerialize(MemoryStream stream)
        {
            int size = 0;

            size += MyConverter.WriteToBinStream(ref stream,ref sign_id);

            //stream.Write(BitConverter.GetBytes(sign_id));
            //stream.Write(Encoding.Unicode.GetBytes(id));
            //stream.Write(Encoding.Unicode.GetBytes(pw));
            //stream.Write(BitConverter.GetBytes(integer_vec));

            return size;
        }

        public int Serialize(MemoryStream stream)
        {
            int size = 0;


            return size;
        }

        public void SetInfo(UInt64 sign_id, string id, string pw)
        {
            this.sign_id = sign_id;
            this.id = id;
            this.pw = pw;
        }
    }


    
}
