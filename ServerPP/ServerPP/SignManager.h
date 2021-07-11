#pragma once

class SignManager : public AppSingleton<SignManager>
{
	friend class Singleton;
public:
	static constexpr size_t MAX_IDSIZE = 30;
	static constexpr size_t MAX_PWSIZE = 40;

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
	enum class E_FailType
	{
		None,
		ID_exist,
		ID_mismatch,
		PW_mismatch,
	};

	struct ResultInfo
	{
		ESignResult result;
		E_FailType failType;
		ResultInfo() : result(ESignResult::None), failType(E_FailType::None) {}
	};

	struct ProcessResult
	{

	};

protected:
	SignManager() {}
public:
	bool Initialize() override;
	void Finalize() override;
	~SignManager();
private:
	bool LoadInfo();	// 정보를 읽어 list 에 셋팅
	bool SaveInfo();	// 정보 변경사항을 저장
	std::list<SignInfoPtr> m_info_list;
public:
	ResultInfo SignUpProcess(const SignInfo inInfo);
	ResultInfo DeleteAccountProcess(const SignInfo inInfo);
	ResultInfo SignInProcess(const SignInfo inInfo);
	ResultInfo SignOutProcess(const SignInfo inInfo);

	ProcessResult StreamProcess<ProcessResult>(IOCPSessionPtr inpSession, InputMemoryStreamPtr inpStream) override;
};