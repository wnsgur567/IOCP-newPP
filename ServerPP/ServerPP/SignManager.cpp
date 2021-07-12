#include "base.h"

Implementation_sInstance(SignManager);

bool SignManager::Initialize()
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
	// 임시 값 셋팅
	for (size_t i = 0; i < 4; i++)
	{
		SignInfoPtr newInfo = std::make_shared<SignInfo>("abcd" + ('e' + i), "1234");
		m_info_list.push_back(newInfo);
	}

	return true;
}

bool SignManager::SaveInfo()
{
	return true;
}

SignManager::ResultInfo SignManager::SignUpProcess(const SignInfo inInfo)
{
	ResultInfo retData;

	for (const auto& item : m_info_list)
	{
		if (item->ID == inInfo.ID)
		{
			retData.result = ESignResult::Fail;
			retData.failType = EFailType::ID_exist;

			return retData;
		}
	}

	m_info_list.push_back(std::make_shared<SignInfo>(inInfo));

	retData.result = ESignResult::Success;
	return retData;
}

SignManager::ResultInfo SignManager::DeleteAccountProcess(const SignInfo inInfo)
{
	ResultInfo retData;

	for (std::list<SignInfoPtr>::iterator it = m_info_list.begin();
		it != m_info_list.end(); ++it)
	{
		SignInfoPtr pInfo = *it;

		if (pInfo->ID == inInfo.ID)
		{
			if (pInfo->PW == inInfo.PW)
			{	// success
				retData.result = ESignResult::Success;
				return retData;
			}
			// pw mismatch
			retData.result = ESignResult::Fail;
			retData.failType = EFailType::PW_mismatch;
			return retData;
		}
	}

	// id mismatch
	retData.result = ESignResult::Fail;
	retData.failType = EFailType::ID_mismatch;
	return retData;
}

SignManager::ResultInfo SignManager::SignInProcess(const SignInfo inInfo)
{
	ResultInfo retData;

	for (const auto& item : m_info_list)
	{
		if (item->ID == inInfo.ID)
		{
			if (item->PW == inInfo.PW)
			{	// success
				retData.result = ESignResult::Success;
				return retData;
			}
			// pw mismatch
			retData.result = ESignResult::Fail;
			retData.failType = EFailType::PW_mismatch;
			return retData;
		}
	}
	// id mismatch
	retData.result = ESignResult::Fail;
	retData.failType = EFailType::ID_mismatch;
	return retData;
}

SignManager::ResultInfo SignManager::SignOutProcess(const SignInfo inInfo)
{
	// .. login 된 상태면 바로 로그아웃 시키도록....

	ResultInfo retData;

	return retData;
}

SignManager::ProcessResult SignManager::StreamProcess(IOCPSessionPtr inpSession, InputMemoryStreamPtr inpStream)
{
	ProcessResult result;

	EProtocol protocol;
	SignInfo _signInfo;

	inpStream->Read(&protocol, sizeof(EProtocol));

	switch (protocol)
	{
	case SignManager::EProtocol::SignUp:
	{	// id , pw
		inpStream->Read(&_signInfo.id_len, sizeof(_signInfo.id_len));
		inpStream->Read(_signInfo.ID, _signInfo.id_len);
		inpStream->Read(&_signInfo.pw_len, sizeof(_signInfo.pw_len));
		inpStream->Read(_signInfo.PW, _signInfo.pw_len);

		result.resultInfo = SignUpProcess(_signInfo);

		result.pStream = std::make_shared<OutputMemoryStream>(BUFSIZE);

		switch (result.resultInfo.result)
		{
		case ESignResult::Success:
		{	// success			
			// stream = protocol + sign result + msg len + msg
			protocol = EProtocol::SignResult;			
			size_t msg_len = strlen(SignUpSuccessMsg);

			result.pStream->Write(&protocol, sizeof(EProtocol));
			result.pStream->Write(&result.resultInfo.result, sizeof(result.resultInfo.result));
			result.pStream->Write(&msg_len, sizeof(size_t));
			result.pStream->Write(SignUpSuccessMsg, msg_len);
		}
		break;
		case ESignResult::Fail:
		{	// fail
			// stream = protocol + Sign result + msg len + msg
			result.pStream->Write(&protocol, sizeof(EProtocol));
			result.pStream->Write(&result.resultInfo.result, sizeof(result.resultInfo.result));

			switch (result.resultInfo.failType)
			{
			case EFailType::ID_exist:
			{
				size_t msg_len = strlen(IDExistMsg);
				result.pStream->Write(&msg_len, sizeof(msg_len));
				result.pStream->Write(IDExistMsg, msg_len);
			}
			break;
			case EFailType::ID_mismatch:
			{
				size_t msg_len = strlen(IDMismatchMsg);
				result.pStream->Write(&msg_len, sizeof(msg_len));
				result.pStream->Write(IDMismatchMsg, msg_len);
			}
			break;
			case EFailType::PW_mismatch:
			{
				size_t msg_len = strlen(PWMismatchMsg);
				result.pStream->Write(&msg_len, sizeof(msg_len));
				result.pStream->Write(PWMismatchMsg, msg_len);
			}
			break;
			}
		}
		break;
		}		
	}
	break;
	case SignManager::EProtocol::SignIn:
	{	// id , pw
		inpStream->Read(&_signInfo.id_len, sizeof(_signInfo.id_len));
		inpStream->Read(_signInfo.ID, _signInfo.id_len);
		inpStream->Read(&_signInfo.pw_len, sizeof(_signInfo.pw_len));
		inpStream->Read(_signInfo.PW, _signInfo.pw_len);

		result.resultInfo = SignInProcess(_signInfo);
	}
	break;
	case SignManager::EProtocol::SignOut:
	{
		result.resultInfo = SignOutProcess(_signInfo);
	}
	break;
	}

	return result;
}

