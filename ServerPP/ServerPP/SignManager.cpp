#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Sign::SignManager);

#include <string>

namespace Sign
{

	const wchar_t* SignManager::ResultMSG::SignInSuccessMsg = L"로그인 성공";
	const wchar_t* SignManager::ResultMSG::SignOutSuccessMsg = L"로그아웃 성공";
	const wchar_t* SignManager::ResultMSG::SignUpSuccessMsg = L"회원가입 성공";
	const wchar_t* SignManager::ResultMSG::DeleteSuccessMsg = L"회원탈퇴 성공";

	const wchar_t* SignManager::ResultMSG::IDExistMsg = L"이미 존재하는 아이디입니다";
	const wchar_t* SignManager::ResultMSG::NotExistIDMsg = L"일치하는 아이디가 없습니다";
	const wchar_t* SignManager::ResultMSG::WrongPWMsg = L"잘못된 패스워드입니다";

	bool SignManager::Initialize(LPVOID) noexcept
	{
#ifdef  __DEBUG
		printf("\n----- SignManager::Initialize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- SignManager::Initialize() Complete -----\n");
#endif //  __DEBUG

		return true;
	}

	void SignManager::Finalize() noexcept
	{
#ifdef  __DEBUG
		printf("\n----- SignManager::Finalize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- SignManager::Finalize() Complete -----\n");
#endif //  __DEBUG
	}

	SignManager::~SignManager()
	{
	}

	bool SignManager::IsExist(const std::wstring& inID)
	{
#ifdef __DEBUG
		printf("\t- SignManager::IsExist() -\n");
#endif
		SQL::SQLManager::queryResult_t results;
		try
		{
			std::stringstream query;
			query << "select * from signinfo where = ";	
			query << "\'" << Utils::StringUtil::WstrToStr(inID) << "\'";

#ifdef __DEBUG
			printf("%s\n", query.str().c_str());
#endif
			SQL::SQLManager::sInstance->Query(query.str(), results);
		}
		catch (const SQL::SQLQueryException& e)
		{
			printf("%s\n", e.what());
		}
		if (results.size() == 0)
			return false;
		return true;
	}

	void SignManager::LoadInfo(const std::wstring& inID, SignInfoPtr& outInfo)
	{
#ifdef __DEBUG
		printf("\t- SignManager::LoadInfo() -\n");
#endif // __DEBUG

		SQL::SQLManager::queryResult_t results;
		try
		{
			std::stringstream query;
			query << "select * from signinfo where ID = ";			
			query << "\'" << Utils::StringUtil::WstrToStr(inID) << "\'";

#ifdef __DEBUG
			printf("query : %s\n", query.str().c_str());
#endif
			SQL::SQLManager::sInstance->Query(query.str(), results);			
		}
		catch (const SQL::SQLQueryException& e)
		{
			printf("%s\n", e.what());
			throw;
		}

		if (results.size() == 0)
		{
#ifdef __DEBUG
			printf("not exists()\n");
#endif
			outInfo = nullptr;
			return;
		}

		auto& row = results[0];
		outInfo = std::make_shared<SignInfo>();
		outInfo->sign_id = std::stoull(row[0]);
		outInfo->ID = Utils::StringUtil::StrToWstr(row[1]);
		outInfo->PW = Utils::StringUtil::StrToWstr(row[2]); 
		outInfo->JoinDate = Utils::StringUtil::StrToWstr(row[3]);
		outInfo->IsActivated = static_cast<bool>(std::stoi(row[4]));
		
#ifdef __DEBUG
		printf("Load / %llu : (%ws , %ws)\n",
			outInfo->sign_id,
			outInfo->ID.c_str(),
			outInfo->PW.c_str());
#endif	
	}

	void SignManager::SaveInfo(const SignInfoPtr inInfo)
	{
#ifdef __DEBUG
		printf("\t- SignManager::SaveInfo() -\n");
#endif // __DEBUG

		SQL::SQLManager::queryResult_t results;
		try
		{
			std::stringstream query;
			query << "insert into signinfo values ( null , ";
			query << "\'" << Utils::StringUtil::WstrToStr(inInfo->ID) << "\' , ";
			query << "\'" << Utils::StringUtil::WstrToStr(inInfo->PW) << "\' , ";
			query << "now() , ";
			query << "1 )" << std::endl;

#ifdef __DEBUG
			printf("query : %s\n", query.str().c_str());
#endif
			SQL::SQLManager::sInstance->Query(query.str());
		}
		catch (const SQL::SQLQueryException& e)
		{
			printf("%s\n", e.what());
			throw;
		}
	}

	void SignManager::DeleteInfo(const SignInfoPtr inInfo)
	{
		// TODO : implement delete process 
	}

	SignManager::ResultData SignManager::SignUpProcess(const SignInfoPtr inInfo)
	{	// 회원가입
		ResultData retData;

		SignInfoPtr getInfo;
		LoadInfo(inInfo->ID, getInfo);

		retData.protocol = EProtocol::SignUp;
		if (nullptr == getInfo)
		{	// not exist
			// sign up sueccess
			SaveInfo(inInfo);

			retData.result = EResult::Success_SignUp;
			retData.msg = ResultMSG::SignUpSuccessMsg;
			return retData;
		}

		// exist
		retData.result = EResult::ExistID;
		retData.msg = ResultMSG::SignUpSuccessMsg;
		return retData;
	}

	SignManager::ResultData SignManager::DeleteAccountProcess(const SignInfoPtr inInfo)
	{
		ResultData retData;

		SignInfoPtr getInfo;
		LoadInfo(inInfo->ID, getInfo);

		retData.protocol = EProtocol::DeleteAccount;
		if (nullptr == getInfo)
		{
			if (getInfo->PW == inInfo->PW)
			{
				DeleteInfo(inInfo);

				retData.result = EResult::Success_DeleteAccount;
				retData.msg = ResultMSG::DeleteSuccessMsg;
				return retData;
			}

			retData.result = EResult::WrongPW;
			retData.msg = ResultMSG::WrongPWMsg;
			return retData;
		}
		else
		{
			retData.result = EResult::NotExistID;
			retData.msg = ResultMSG::NotExistIDMsg;
			return retData;
		}
	}

	SignManager::ResultData SignManager::SignInProcess(const SignInfoPtr inInfo)
	{
		ResultData retData;

		SignInfoPtr getInfo;
		LoadInfo(inInfo->ID, getInfo);

		retData.protocol = EProtocol::SignIn;
		if (nullptr == getInfo)
		{	// id not exist
			retData.result = EResult::NotExistID;
			retData.msg = ResultMSG::NotExistIDMsg;
			return retData;
		}

		if (getInfo->PW == inInfo->PW)
		{	// sign in success
			retData.result = EResult::Success_SingIn;
			retData.msg = ResultMSG::SignInSuccessMsg;
			retData.user_id = getInfo->sign_id;
			printf("sign id : %llu\n", retData.user_id);
			return retData;
		}

		retData.result = EResult::WrongPW;
		retData.msg = ResultMSG::WrongPWMsg;
		return retData;
	}

	SignManager::ResultData SignManager::SignOutProcess(const SignInfoPtr inInfo)
	{
		ResultData retData;

		SignInfoPtr getInfo;
		LoadInfo(inInfo->ID, getInfo);

		retData.protocol = EProtocol::SignOut;
		if (nullptr == getInfo)
		{	// id not exist
			retData.result = EResult::NotExistID;
			retData.msg = ResultMSG::NotExistIDMsg;
			return retData;
		}

		if (getInfo->PW == inInfo->PW)
		{	// sign in success
			retData.result = EResult::Success_SignOut;
			retData.msg = ResultMSG::SignOutSuccessMsg;
			return retData;
		}

		retData.result = EResult::WrongPW;
		retData.msg = ResultMSG::WrongPWMsg;
		return retData;
	}

}