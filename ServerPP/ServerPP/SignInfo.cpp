#include "base.h"

SignInfo::SignInfo(const char* inID, const char* inPW)
	: id_len(0), pw_len(0), ID(), PW()
{
	strcpy_s(ID, SignManager::MAX_IDSIZE, inID);
	id_len = strlen(ID);
	strcpy_s(PW, SignManager::MAX_PWSIZE, inPW);
	pw_len = strlen(PW);
}

SignInfo::SignInfo(std::string inID, std::string inPW)
	: id_len(0), pw_len(0), ID(), PW()
{
	strcpy_s(ID, SignManager::MAX_IDSIZE, inID.c_str());
	id_len = strlen(ID);
	strcpy_s(PW, SignManager::MAX_PWSIZE, inPW.c_str());
	pw_len = strlen(PW);
}

bool SignInfo::operator==(const SignInfo& other)
{
	if (strcmp(this->ID, other.ID) || strcmp(this->PW, other.PW))
		return false;
	return true;
}

bool SignInfo::operator!=(const SignInfo& other)
{
	return !(*this == other);
}

bool SignInfo::operator>(const SignInfo& other)
{
	if (strcmp(this->ID, other.ID) > 0)
		return true;
	return false;
}

bool SignInfo::operator<(const SignInfo& other)
{
	if (strcmp(this->ID, other.ID) < 0)
		return true;
	return false;
}
