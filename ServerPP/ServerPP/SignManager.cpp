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
	const wchar_t* SignManager::ResultMSG::WrongPWMsg = L"일치하는 패스워드가 없습니다";

	bool SignManager::Initialize(LPVOID) noexcept
	{
		if (false == LoadInfo())
			return false;

		return true;
	}

	void SignManager::Finalize() noexcept
	{
	}

	SignManager::~SignManager()
	{
	}

	bool SignManager::LoadInfo()
	{
		SQL::SQLManager::queryResult_t results;
		if (false == SQL::SQLManager::sInstance->Query("select * from signinfo", results))
			return false;

		for (auto& item : results)
		{
			SignInfoPtr pInfo = std::make_shared<SignInfo>();

			pInfo->sign_id = SignInfo::StringToSignid(item[0]);		// sign id			
			pInfo->ID.assign(item[1].begin(), item[1].end());		// ID
			pInfo->PW.assign(item[2].begin(), item[2].end());		// PW

			m_info_map.insert({ pInfo->sign_id, pInfo });

#ifdef __DEBUG
			printf("Load / %llu : (%ws , %ws)\n",
				pInfo->sign_id,
				pInfo->ID.c_str(),
				pInfo->PW.c_str());
#endif
		}
#ifdef __DEBUG
		printf("SignManager::LoadInfo() Complete!!\n%llu info are loaded...\n", m_info_map.size());
#endif

		return true;
	}

	bool SignManager::SaveInfo()
	{
		return true;
	}


	SignManager::ResultData SignManager::SignUpProcess(const SignInfo inInfo)
	{	// 회원가입
		ResultData retData;

		for (const auto& item : m_info_map)
		{
			if (item.second->ID == inInfo.ID)
			{
				retData.result = EResult::ExistID;
				retData.msg = ResultMSG::IDExistMsg;

				return retData;
			}
		}

		m_info_map.insert({ inInfo.sign_id ,std::make_shared<SignInfo>(inInfo) });

		retData.result = EResult::Success_SignUp;
		retData.msg = ResultMSG::SignUpSuccessMsg;
		return retData;
	}

	SignManager::ResultData SignManager::DeleteAccountProcess(const SignInfo inInfo)
	{
		ResultData retData;

		for (SignMap::iterator it = m_info_map.begin();
			it != m_info_map.end(); ++it)
		{
			SignInfoPtr pInfo = it->second;

			if (pInfo->ID == inInfo.ID)
			{
				if (pInfo->PW == inInfo.PW)
				{	// delete success
					retData.result = EResult::Success_DeleteAccount;
					retData.msg = ResultMSG::DeleteSuccessMsg;
					return retData;
				}
				// pw mismatch
				retData.result = EResult::WrongPW;
				retData.msg = ResultMSG::WrongPWMsg;
				return retData;
			}
		}

		// NotExistID
		retData.result = EResult::NotExistID;
		retData.msg = ResultMSG::NotExistIDMsg;

		return retData;
	}

	SignManager::ResultData SignManager::SignInProcess(const SignInfo inInfo)
	{
		ResultData retData;

		for (const auto& item : m_info_map)
		{
			if (item.second->ID == inInfo.ID)
			{
				if (item.second->PW == inInfo.PW)
				{	// success
					retData.result = EResult::Success_SingIn;
					retData.msg = ResultMSG::SignInSuccessMsg;
					return retData;
				}
				// pw mismatch
				retData.result = EResult::WrongPW;
				retData.msg = ResultMSG::WrongPWMsg;
				return retData;
			}
		}
		// id mismatch
		retData.result = EResult::NotExistID;
		retData.msg = ResultMSG::NotExistIDMsg;
		return retData;
	}

	SignManager::ResultData SignManager::SignOutProcess(const SignInfo inInfo)
	{
		ResultData retData;

		retData.result = EResult::Success_SignOut;
		retData.msg = ResultMSG::SignOutSuccessMsg;

		return retData;
	}

}