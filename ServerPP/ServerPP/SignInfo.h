#pragma once

namespace Sign
{

	class SignInfo;
	using SignInfoPtr = std::shared_ptr<SignInfo>;

	// ID 중복 허용 x
	// ID 사전 순으로 정렬
	class SignInfo
	{
	public:
		static constexpr size_t MAX_IDSIZE = 30;
		static constexpr size_t MAX_PWSIZE = 40;
		using signid_t = unsigned __int64;
	public:
		signid_t sign_id;
		std::wstring ID;
		std::wstring PW;
	public:
		SignInfo() : sign_id(0), ID(), PW() {}
		SignInfo(signid_t inSignID, const wchar_t* inID, const wchar_t* inPW);
		SignInfo(signid_t inSignID, std::wstring inID, std::wstring inPW);
		bool operator == (const SignInfo& other);
		bool operator != (const SignInfo& other);
		bool operator > (const SignInfo& other);
		bool operator < (const SignInfo& other);
		
		static signid_t StringToSignid(const std::string& inStr);
		static signid_t WStringToSignid(const std::wstring& inStr);
	};
}