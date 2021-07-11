#pragma once

// ID 중복 허용 x
// ID 사전 순으로 정렬
class SignInfo
{
public:
	std::string ID;
	std::string PW;
public:
	SignInfo() {}
	SignInfo(const char* inID, const char* inPW);
	SignInfo(std::string inID, std::string inPW);
	bool operator == (const SignInfo& other);
	bool operator != (const SignInfo& other);
	bool operator > (const SignInfo& other);
	bool operator < (const SignInfo& other);
};