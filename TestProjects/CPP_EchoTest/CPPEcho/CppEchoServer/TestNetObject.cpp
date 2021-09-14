#include "TestNetObject.h"
#include "standard.h"

void TestNetObject::SetInfo()
{
	m_int64 = 12231126534198247U;
	m_float = 75.2165654f;
	m_wstr = L"hello world!!";
	integer_vec.push_back(10);
	integer_vec.push_back(90);
	integer_vec.push_back(70);
	integer_vec.push_back(50);
	integer_vec.push_back(44);
	int_to_wstr_map.insert({ 8,L"kdkdkdkdk" });
	int_to_wstr_map.insert({ 4,L"icicicicicicic oc" });
	int_to_wstr_map.insert({ 111,L"pipipipipipipip" });

	class_ptr = new TestMemeberClass();
	class_ptr->x = 1213.8f;
	class_ptr->y = -0.021f;
	class_ptr->z = 111.f;
}

void TestNetObject::Print() const
{
	printf("==== print\n");
	printf("%llu\n", m_int64);
	printf("%f\n", m_float);
	printf("%ws\n", m_wstr.c_str());

	printf("vector : ");
	for (const auto& item : integer_vec)
	{
		printf("%d ", item);
	}
	printf("\n");

	printf("map : ");
	for (const auto& item : int_to_wstr_map)
	{
		printf("[ %d , %ws ]\n", item.first, item.second.c_str());
	}
	printf("\n");

	printf("==== print end\n");
}

int TestNetObject::Serialize(std::basic_ostream<byte>& out_stream)
{
	int size = 0;	
	size += Utils::WriteToBinStream(out_stream, m_int64);
	size += Utils::WriteToBinStream(out_stream, m_float);
	size += Utils::WriteToBinStream(out_stream, m_wstr);
	size += Utils::WriteToBinStream(out_stream, integer_vec);
	size += Utils::WriteToBinStream(out_stream, int_to_wstr_map);
	size += Utils::WriteToBinStream(out_stream, class_ptr);
	return size;
}

int TestNetObject::DeSerialize(std::basic_istream<byte>& in_stream)
{
	int size = 0;	
	size += Utils::ReadFromBinStream(in_stream, m_int64);
	size += Utils::ReadFromBinStream(in_stream, m_float);
	size += Utils::ReadFromBinStream(in_stream, m_wstr);
	size += Utils::ReadFromBinStream(in_stream, integer_vec);
	size += Utils::ReadFromBinStream(in_stream, int_to_wstr_map);
	size += Utils::ReadFromBinStream(in_stream, class_ptr);
	return size;
}

int TestMemeberClass::Serialize(std::basic_ostream<byte>& out_stream)
{
	int size = 0;
	size += Utils::WriteToBinStream(out_stream, x);
	size += Utils::WriteToBinStream(out_stream, y);
	size += Utils::WriteToBinStream(out_stream, z);
	return size;
}

int TestMemeberClass::DeSerialize(std::basic_istream<byte>& in_stream)
{
	int size = 0;
	size += Utils::ReadFromBinStream(in_stream, x);
	size += Utils::ReadFromBinStream(in_stream, y);
	size += Utils::ReadFromBinStream(in_stream, z);
	return size;
}

void TestMemeberClass::Print() const
{
	printf("(%f,%f,%f)\n", x, y, z);
}
