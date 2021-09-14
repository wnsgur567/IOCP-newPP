#include "standard.h"
#include "TestSignInfo.h"


class TestPacket
{
	using byte = Utils::byte;

	int m_read_head = 0;
	int m_write_head = 0;
	byte m_buffer[512];

public:


	void Write(ISerializable* obj)
	{
		std::basic_ostringstream<byte> _stream;

		int pack_size = obj->Serialize(_stream);

		memcpy(m_buffer + m_write_head, &pack_size, sizeof(int));
		m_write_head += sizeof(int);
		memcpy(m_buffer + m_write_head, _stream.str().c_str(), pack_size);
		m_write_head += pack_size;
	}
	void Read(ISerializable* obj)
	{
		int pack_size;
		memcpy(&pack_size, m_buffer + m_read_head, sizeof(int));
		m_read_head += sizeof(int);

		byte* start_ptr = m_buffer + m_read_head;
		Utils::byte_string str(start_ptr, start_ptr + pack_size);
		
		std::basic_istringstream<byte> _stream;
		_stream.str(str);
		m_read_head += obj->DeSerialize(_stream);

		m_read_head += pack_size;
	}
};

void ObjTest()
{
	TestSignInfo sign_info;
	sign_info.SetInfo(10, L"code1", L"1234");

	TestPacket packet;

	packet.Write(&sign_info);


	TestSignInfo second_info;
	packet.Read(&second_info);

	second_info.Print();
}

int main()
{
	//Utils::SerializationIntegralTest();
	//Utils::SerializationFloatingTest();
	//Utils::characterTest();
	//Utils::StringTest();
	//Utils::VectorTest();
	//Utils::ListTest();
	//Utils::SetTest();
	Utils::MapTest();

	//ObjTest();
	//ObjTest();
	//ObjTest();
	//ObjTest();
	//ObjTest();
}

