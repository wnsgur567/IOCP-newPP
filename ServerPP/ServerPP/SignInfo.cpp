#include "IOCPNet_RootHeader.h"

namespace Sign
{
	void SignInfo::Flush()
	{
		sign_id = 0;
		ID.clear();
		PW.clear();
	}
	SignInfo::SignInfo(signid_t inSignID, const wchar_t* inID, const wchar_t* inPW)
		: sign_id(inSignID), ID(inID), PW(inPW)
	{

	}

	SignInfo::SignInfo(signid_t inSignID, std::wstring inID, std::wstring inPW)
		: sign_id(inSignID), ID(inID), PW(inPW)
	{

	}

	bool SignInfo::operator==(const SignInfo& other)
	{
		if (sign_id == other.sign_id)
			return true;
		return false;
	}

	bool SignInfo::operator!=(const SignInfo& other)
	{
		return !(*this == other);
	}

	// ID 기준
	bool SignInfo::operator>(const SignInfo& other)
	{
		return (ID > other.ID);
	}

	// ID 기준
	bool SignInfo::operator<(const SignInfo& other)
	{
		return (ID < other.ID);
	}
	
	int SignInfo::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
	{
		int size = 0;
		WriteToStream(out_stream, ID);
		WriteToStream(out_stream, PW);
		return size;
	}
	int SignInfo::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
	{
		int size = 0;
		ReadFromStream(in_stream, ID);
		ReadFromStream(in_stream, PW);
		return size;
	}
}
