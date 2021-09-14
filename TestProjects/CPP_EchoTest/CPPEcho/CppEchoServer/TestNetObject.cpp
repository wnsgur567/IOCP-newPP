#include "TestNetObject.h"

void TestNetObject::SetInfo(uint64_t inSign_id, std::wstring inId, std::wstring inPw)
{
	sign_id = inSign_id;
	id = inId;
	pw = inPw;
}

void TestNetObject::Print() const
{
	printf("sign_id : %llu\n", sign_id);
	printf("id : %ws\n", id.c_str());
	printf("id : %ws\n", pw.c_str());

	for (const auto& item : integer_vec)
	{
		printf("%d ", item);
	}
	printf("\n");

}

int TestNetObject::Serialize(std::basic_ostringstream<Utils::byte>& out_stream)
{
	int size = 0;
	size += Utils::WriteToBinStream(out_stream, sign_id);
	size += Utils::WriteToBinStream(out_stream, id);
	size += Utils::WriteToBinStream(out_stream, pw);
	size += Utils::WriteToBinStream(out_stream, integer_vec);
	return size;
}

int TestNetObject::DeSerialize(std::basic_istringstream<Utils::byte>& in_stream)
{
	int size = 0;
	size += Utils::ReadFromBinStream(in_stream, sign_id);
	size += Utils::ReadFromBinStream(in_stream, id);
	size += Utils::ReadFromBinStream(in_stream, pw);
	size += Utils::ReadFromBinStream(in_stream, integer_vec);
	return size;
}