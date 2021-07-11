#include "base.h"

SignInfo::SignInfo(const char* inID, const char* inPW)
	: ID(inID), PW(inPW)
{

}

SignInfo::SignInfo(std::string inID, std::string inPW)
	: ID(inID), PW(inPW)
{

}

bool SignInfo::operator==(const SignInfo& other)
{
	if (this->ID != other.ID || this->PW != other.PW)
		return false;
	return true;
}

bool SignInfo::operator!=(const SignInfo& other)
{
	return !(*this == other);
}

bool SignInfo::operator>(const SignInfo& other)
{
	return this->ID > other.ID;
}

bool SignInfo::operator<(const SignInfo& other)
{
	return this->ID < other.ID;
}
