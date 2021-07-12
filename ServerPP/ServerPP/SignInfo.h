#pragma once

// ID 중복 허용 x
// ID 사전 순으로 정렬
class SignInfo
{
public:
	size_t id_len;
	size_t pw_len;
	char ID[SignManager::MAX_IDSIZE];
	char PW[SignManager::MAX_PWSIZE];
public:
	SignInfo() : id_len(0), pw_len(0), ID(), PW() {}
	SignInfo(const char* inID, const char* inPW);
	SignInfo(std::string inID, std::string inPW);
	bool operator == (const SignInfo& other);
	bool operator != (const SignInfo& other);
	bool operator > (const SignInfo& other);
	bool operator < (const SignInfo& other);
};