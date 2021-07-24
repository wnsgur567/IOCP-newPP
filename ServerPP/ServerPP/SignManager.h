#pragma once

class SignManager : public Singleton<SignManager>
{
	friend class Singleton;
public:
	static constexpr size_t MAX_IDSIZE = SignInfo::MAX_IDSIZE;
	static constexpr size_t MAX_PWSIZE = SignInfo::MAX_PWSIZE;

	const char* SignUpSuccessMsg = "회원가입 성공";
	const char* SignInSuccessMsg = "로그인 성공";
	const char* SignOutSuccessMsg = "로그아웃 성공";
	const char* IDExistMsg = "이미 존재하는 아이디입니다";
	const char* IDMismatchMsg = "일치하는 아이디가 없습니다";
	const char* PWMismatchMsg = "일치하는 패스워드가 없습니다";

	enum class EProtocol
	{
		None,

		SignUp,
		SignIn,
		SignOut,
		SignResult,
	};

	enum class ESignResult
	{
		None,
		Fail,
		Success,
	};

	enum class EFailType
	{
		None,
		ID_exist,
		ID_mismatch,
		PW_mismatch,
	};

	struct ResultInfo
	{
		ESignResult result;
		EFailType failType;
		ResultInfo() : result(ESignResult::None), failType(EFailType::None) {}
	};

	struct ProcessResult
	{
		// ...
		OutputMemoryStreamPtr pStream;
		ResultInfo resultInfo;
	};

protected:
	SignManager() {}
public:
	bool Initialize(LPVOID) override;
	void Finalize() override;
	~SignManager();
private:
	bool LoadInfo();	// 정보를 읽어 list 에 셋팅
	bool SaveInfo();	// 정보 변경사항을 저장
	std::list<SignInfoPtr> m_info_list;
public:
	// process 관련
	ResultInfo SignUpProcess(const SignInfo inInfo);
	ResultInfo DeleteAccountProcess(const SignInfo inInfo);
	ResultInfo SignInProcess(const SignInfo inInfo);
	ResultInfo SignOutProcess(const SignInfo inInfo);

	ProcessResult StreamProcess(InputMemoryStreamPtr inpStream, bool IsSignedIn);
};