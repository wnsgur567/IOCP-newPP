#include "TestSignInfo.h"

void TestSignInfo::SetInfo(uint64_t inSign_id, std::wstring inId, std::wstring inPw)
{
	sign_id = inSign_id;
	id = inId;
	pw = inPw;
}

void TestSignInfo::Print() const
{
	printf("sign_id : %llu\n", sign_id);
	printf("id : %ws\n", id.c_str());
	printf("id : %ws\n", pw.c_str());
}

int TestSignInfo::Serialize(std::basic_ostringstream<Utils::byte>& out_stream)
{
	int size = 0;
	size += Utils::WriteToBinStream(out_stream, sign_id);
	size += Utils::WriteToBinStream(out_stream, id);
	size += Utils::WriteToBinStream(out_stream, pw);
	return size;
}

int TestSignInfo::DeSerialize(std::basic_istringstream<Utils::byte>& in_stream)
{
	int size = 0;
	size += Utils::ReadFromBinStreamImpl(in_stream, sign_id);
	size += Utils::ReadFromBinStreamImpl(in_stream, id);
	size += Utils::ReadFromBinStreamImpl(in_stream, pw);
	return size;
}