#include "IOCPNet_RootHeader.h"

namespace Sign
{

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
	SignInfo::signid_t SignInfo::StringToSignid(const std::string& inStr)
	{
		return std::stoull(inStr, 0);
	}
	SignInfo::signid_t SignInfo::WStringToSignid(const std::wstring& inStr)
	{
		return std::stoull(inStr, 0);
	}
}
