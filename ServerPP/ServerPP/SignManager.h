#pragma once

namespace Sign
{

	class SignManager : public MyBase::Singleton<SignManager>
	{
		friend class Singleton;
	public:
		using SignMap = std::unordered_map<SignInfo::signid_t, SignInfoPtr>;
		static constexpr size_t MAX_IDSIZE = SignInfo::MAX_IDSIZE;
		static constexpr size_t MAX_PWSIZE = SignInfo::MAX_PWSIZE;

		struct ResultMSG
		{
		public:
			static const wchar_t* SignInSuccessMsg;
			static const wchar_t* SignOutSuccessMsg;
			static const wchar_t* SignUpSuccessMsg;
			static const wchar_t* DeleteSuccessMsg;

			static const wchar_t* IDExistMsg;
			static const wchar_t* NotExistIDMsg;
			static const wchar_t* WrongPWMsg;
		};

		enum class EProtocol : ProtocolSize_t
		{
			None = 0,

			// flags...
			SignIn = 1 << 0,
			SignOut = 1 << 1,
			SignUp = 1 << 2,
			DeleteAccount = 1 << 3,
		};

		enum class EResult : ResultSize_t
		{
			None = 0,

			// start 1000
			ExistID = 1001,
			NotExistID,
			WrongPW,

			Success_SingIn,
			Success_SignOut,
			Success_SignUp,
			Success_DeleteAccount,
		};

		struct ResultData
		{
			EProtocol protocol;
			EResult result;
			const wchar_t* msg;
		};

	protected:
		SignManager() {}
	public:
		bool Initialize(LPVOID) noexcept override;
		void Finalize() noexcept override;
		~SignManager();
	private:
		bool IsExist(const std::wstring& inID);
		void LoadInfo(const std::wstring& inID, SignInfoPtr& outInfo);
		void SaveInfo(const SignInfoPtr inInfo);		
		void DeleteInfo(const SignInfoPtr inInfo);
		//SignMap m_info_map;
	public:
		ResultData SignUpProcess(const SignInfoPtr inInfo);
		ResultData DeleteAccountProcess(const SignInfoPtr inInfo);
		ResultData SignInProcess(const SignInfoPtr inInfo);
		ResultData SignOutProcess(const SignInfoPtr inInfo);
	};
}