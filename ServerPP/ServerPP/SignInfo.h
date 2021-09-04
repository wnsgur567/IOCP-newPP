#pragma once

class SignInfo;
using SignInfoPtr = std::shared_ptr<SignInfo>;

// ID 중복 허용 x
// ID 사전 순으로 정렬
class SignInfo
{
public:
	static constexpr size_t MAX_IDSIZE = 30;
	static constexpr size_t MAX_PWSIZE = 40;
public:
	size_t id_len;
	size_t pw_len;
	wchar_t ID[MAX_IDSIZE];
	wchar_t PW[MAX_PWSIZE];
public:
	SignInfo() : id_len(0), pw_len(0), ID(), PW() {}
	SignInfo(const wchar_t* inID, const wchar_t* inPW);
	SignInfo(std::wstring inID, std::wstring inPW);
	bool operator == (const SignInfo& other);
	bool operator != (const SignInfo& other);
	bool operator > (const SignInfo& other);
	bool operator < (const SignInfo& other);
};