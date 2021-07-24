#pragma once

class SignManager : public Singleton<SignManager>
{
	friend class Singleton;
public:
	static constexpr size_t MAX_IDSIZE = SignInfo::MAX_IDSIZE;
	static constexpr size_t MAX_PWSIZE = SignInfo::MAX_PWSIZE;

	const char* SignUpSuccessMsg = "ȸ������ ����";
	const char* SignInSuccessMsg = "�α��� ����";
	const char* SignOutSuccessMsg = "�α׾ƿ� ����";
	const char* IDExistMsg = "�̹� �����ϴ� ���̵��Դϴ�";
	const char* IDMismatchMsg = "��ġ�ϴ� ���̵� �����ϴ�";
	const char* PWMismatchMsg = "��ġ�ϴ� �н����尡 �����ϴ�";

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
	bool LoadInfo();	// ������ �о� list �� ����
	bool SaveInfo();	// ���� ��������� ����
	std::list<SignInfoPtr> m_info_list;
public:
	// process ����
	ResultInfo SignUpProcess(const SignInfo inInfo);
	ResultInfo DeleteAccountProcess(const SignInfo inInfo);
	ResultInfo SignInProcess(const SignInfo inInfo);
	ResultInfo SignOutProcess(const SignInfo inInfo);

	ProcessResult StreamProcess(InputMemoryStreamPtr inpStream, bool IsSignedIn);
};