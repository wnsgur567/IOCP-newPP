#pragma once

// ID �ߺ� ��� x
// ID ���� ������ ����
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