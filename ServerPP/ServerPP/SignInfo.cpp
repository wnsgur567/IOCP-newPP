#include "IOCPNet_RootHeader.h"

SignInfo::SignInfo(const wchar_t* inID, const wchar_t* inPW)
	: id_len(0), pw_len(0), ID(), PW()
{
	wcscpy_s(ID, SignManager::MAX_IDSIZE, inID);
	//strcpy_s(ID, SignManager::MAX_IDSIZE, inID);
	id_len = wcslen(ID);
	wcscpy_s(PW, SignManager::MAX_PWSIZE, inPW);
	//strcpy_s(PW, SignManager::MAX_PWSIZE, inPW);
	pw_len = wcslen(PW);
}

SignInfo::SignInfo(std::wstring inID, std::wstring inPW)
	: id_len(0), pw_len(0), ID(), PW()
{
	wcscpy_s(ID, SignManager::MAX_IDSIZE, inID.c_str());
	id_len = wcslen(ID);
	wcscpy_s(PW, SignManager::MAX_PWSIZE, inPW.c_str());
	pw_len = wcslen(PW);
}

bool SignInfo::operator==(const SignInfo& other)
{
	if (wcscmp(this->ID, other.ID) || wcscmp(this->PW, other.PW))
		return false;
	return true;
}

bool SignInfo::operator!=(const SignInfo& other)
{
	return !(*this == other);
}

bool SignInfo::operator>(const SignInfo& other)
{
	if (wcscmp(this->ID, other.ID) > 0)
		return true;
	return false;
}

bool SignInfo::operator<(const SignInfo& other)
{
	if (wcscmp(this->ID, other.ID) < 0)
		return true;
	return false;
}
