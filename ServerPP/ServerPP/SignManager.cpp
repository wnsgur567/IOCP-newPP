#include "base.h"

Implementation_sInstance(SignManager);

const wchar_t* SignManager::ResultMSG::SignInSuccessMsg = L"�α��� ����";
const wchar_t* SignManager::ResultMSG::SignOutSuccessMsg = L"�α׾ƿ� ����";
const wchar_t* SignManager::ResultMSG::SignUpSuccessMsg = L"ȸ������ ����";
const wchar_t* SignManager::ResultMSG::DeleteSuccessMsg = L"ȸ��Ż�� ����";

const wchar_t* SignManager::ResultMSG::IDExistMsg = L"�̹� �����ϴ� ���̵��Դϴ�";
const wchar_t* SignManager::ResultMSG::NotExistIDMsg = L"��ġ�ϴ� ���̵� �����ϴ�";
const wchar_t* SignManager::ResultMSG::WrongPWMsg = L"��ġ�ϴ� �н����尡 �����ϴ�";

bool SignManager::Initialize(LPVOID)
{
	if (false == LoadInfo())
		return false;

	return true;
}

void SignManager::Finalize()
{
}

SignManager::~SignManager()
{
}

bool SignManager::LoadInfo()
{
	// �ӽ� �� ����
	std::wstring s = L"abcd";
	for (size_t i = 0; i < 4; i++)
	{
		std::wstring ss = s + (wchar_t)('e' + i);
		SignInfoPtr newInfo = std::make_shared<SignInfo>(ss.c_str(), L"1234");
		m_info_list.push_back(newInfo);
	}

	return true;
}

bool SignManager::SaveInfo()
{
	return true;
}


SignManager::ResultData SignManager::SignUpProcess(const SignInfo inInfo)
{	// ȸ������
	ResultData retData;

	for (const auto& item : m_info_list)
	{
		if (item->ID == inInfo.ID)
		{
			retData.result = EResult::ExistID;
			retData.msg = ResultMSG::IDExistMsg;

			return retData;
		}
	}

	m_info_list.push_back(std::make_shared<SignInfo>(inInfo));

	retData.result = EResult::Success_SignUp;
	retData.msg = ResultMSG::SignUpSuccessMsg;
	return retData;
}

SignManager::ResultData SignManager::DeleteAccountProcess(const SignInfo inInfo)
{
	ResultData retData;

	for (std::list<SignInfoPtr>::iterator it = m_info_list.begin();
		it != m_info_list.end(); ++it)
	{
		SignInfoPtr pInfo = *it;

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

	for (const auto& item : m_info_list)
	{
		if (item->ID == inInfo.ID)
		{
			if (item->PW == inInfo.PW)
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