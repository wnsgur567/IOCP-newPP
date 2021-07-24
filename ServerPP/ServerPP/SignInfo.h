#pragma once

class SignInfo;
using SignInfoPtr = std::shared_ptr<SignInfo>;

// ID �ߺ� ��� x
// ID ���� ������ ����
class SignInfo
{
public:
	static constexpr size_t MAX_IDSIZE = 30;
	static constexpr size_t MAX_PWSIZE = 40;
public:
	size_t id_len;
	size_t pw_len;
	char ID[MAX_IDSIZE];
	char PW[MAX_PWSIZE];
public:
	SignInfo() : id_len(0), pw_len(0), ID(), PW() {}
	SignInfo(const char* inID, const char* inPW);
	SignInfo(std::string inID, std::string inPW);
	bool operator == (const SignInfo& other);
	bool operator != (const SignInfo& other);
	bool operator > (const SignInfo& other);
	bool operator < (const SignInfo& other);
};