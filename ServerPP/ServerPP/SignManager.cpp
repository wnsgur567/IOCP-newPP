#include "IOCPNet_RootHeader.h"

Implementation_sInstance(Sign::SignManager);

#include <string>

namespace Sign
{

	const wchar_t* SignManager::ResultMSG::SignInSuccessMsg = L"�α��� ����";
	const wchar_t* SignManager::ResultMSG::SignOutSuccessMsg = L"�α׾ƿ� ����";
	const wchar_t* SignManager::ResultMSG::SignUpSuccessMsg = L"ȸ������ ����";
	const wchar_t* SignManager::ResultMSG::DeleteSuccessMsg = L"ȸ��Ż�� ����";

	const wchar_t* SignManager::ResultMSG::IDExistMsg = L"�̹� �����ϴ� ���̵��Դϴ�";
	const wchar_t* SignManager::ResultMSG::NotExistIDMsg = L"��ġ�ϴ� ���̵� �����ϴ�";
	const wchar_t* SignManager::ResultMSG::WrongPWMsg = L"�߸��� �н������Դϴ�";

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

	SignManager::EResult SignManager::SignUpProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{	// ȸ������ process

		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
		pInfo->Flush();
		ISerializable* ptr = pInfo.get();
		// get id and pw from stream
		NetBase::ReadFromBinStream(inpStream, ptr);


		EProtocol protocol;
		EResult result;
		std::wstring msg;

		// ID ������ �ִ��� Ȯ���ϱ�
		SignInfoPtr getInfo;
		LoadInfo(pInfo->ID, getInfo);


		if (nullptr == getInfo)
		{	// id not exist
			// sign up sueccess
			SaveInfo(pInfo);

			protocol = EProtocol::SignUp;
			result = EResult::Success_SignUp;
			msg = ResultMSG::SignUpSuccessMsg;
		}
		else
		{	// id exist
			protocol = EProtocol::SignUp;
			result = EResult::ExistID;
			msg = ResultMSG::IDExistMsg;
		}


#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
		printf("%d\n", result);
		wprintf(L" msg : %ws\n", msg.c_str());
#endif // __DEBUG

		// get new send stream
		auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream(only data part)
			// sendpacket is composed with (size + id + data)

		int write_size = 0;
		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol	
		write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
		// write result
		write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
		// write result msg
		write_size += NetBase::WriteToBinStream(pOutputStream, msg);
		/// write end		

#ifdef __DEBUG
		printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	


		// send result to CLIENT
		inpSession->Send(pOutputStream);

		return result;
	}

	SignManager::EResult SignManager::DeleteAccountProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{	// ȸ�� Ż�� process

		/// Read from Input Stream
	// get data from recv stream
		Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
		pInfo->Flush();
		ISerializable* ptr = pInfo.get();
		// get id and pw from stream
		NetBase::ReadFromBinStream(inpStream, ptr);

		EProtocol protocol;
		EResult result;
		std::wstring msg;

		// db ���� �б�
		SignInfoPtr getInfo;
		LoadInfo(pInfo->ID, getInfo);


		if (nullptr == getInfo)
		{	// �ش� ID �� ���� ���
			protocol = EProtocol::DeleteAccount;
			result = EResult::NotExistID;
			msg = ResultMSG::NotExistIDMsg;

#ifdef __DEBUG
			wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
			printf("%d\n", result);
			wprintf(L" msg : %ws\n", msg.c_str());
#endif // __DEBUG
		}
		else
		{	// ID ��ġ
			if (getInfo->PW == pInfo->PW)
			{	// PW ��ġ

				// db ���� ����
				DeleteInfo(pInfo);

				protocol = EProtocol::DeleteAccount;
				result = EResult::Success_DeleteAccount;
				msg = ResultMSG::DeleteSuccessMsg;

				// �α׾ƿ� ��Ű��
				inpSession->SetSign(false);
			}
			else
			{	// PW ����ġ
				protocol = EProtocol::DeleteAccount;
				result = EResult::WrongPW;
				msg = ResultMSG::WrongPWMsg;
			}
		}

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
		printf("%d\n", result);
		wprintf(L" msg : %ws\n", msg.c_str());
#endif // __DEBUG

		// get new send stream
		auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream(only data part)
			// sendpacket is composed with (size + id + data)

		int write_size = 0;
		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol	
		write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
		// write result
		write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
		// write result msg
		write_size += NetBase::WriteToBinStream(pOutputStream, msg);
		/// write end		

#ifdef __DEBUG
		printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	


		// send result to CLIENT
		inpSession->Send(pOutputStream);

		return result;
	}

	SignManager::EResult SignManager::SignInProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{	// �α��� process

		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
		pInfo->Flush();
		ISerializable* ptr = pInfo.get();
		// get id and pw from stream
		NetBase::ReadFromBinStream(inpStream, ptr);

		EProtocol protocol;
		EResult result;
		std::wstring msg;

		// db ���� �б�
		SignInfoPtr getInfo;
		LoadInfo(pInfo->ID, getInfo);


		if (nullptr == getInfo)
		{	// id not exist
			protocol = EProtocol::SignIn;
			result = EResult::NotExistID;
			msg = ResultMSG::NotExistIDMsg;
		}
		else if (getInfo->PW == pInfo->PW)
		{	// sign in success
			protocol = EProtocol::SignIn;
			result = EResult::Success_SingIn;
			msg = ResultMSG::SignInSuccessMsg;
			inpSession->SetUserID(getInfo->sign_id);
			printf("sign id : %llu\n", getInfo->sign_id);

			// �α��� ��Ű��
			inpSession->SetSign(true);
		}
		else
		{
			protocol = EProtocol::SignIn;
			result = EResult::WrongPW;
			msg = ResultMSG::WrongPWMsg;
		}

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
		printf("%d\n", result);
		wprintf(L" msg : %ws\n", msg.c_str());
#endif // __DEBUG

		// get new send stream
		auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream(only data part)
		// sendpacket is composed with (size + id + data)

		int write_size = 0;
		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol	
		write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
		// write result
		write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
		// write result msg
		write_size += NetBase::WriteToBinStream(pOutputStream, msg);
		/// write end		

#ifdef __DEBUG
		printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	


		// send result to CLIENT
		inpSession->Send(pOutputStream);

		return result;
	}

	SignManager::EResult SignManager::SignOutProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{	// �α׾ƿ� process

		/// Read from Input Stream
		// get data from recv stream
		Sign::SignInfoPtr pInfo = std::make_shared<Sign::SignInfo>();
		pInfo->Flush();
		ISerializable* ptr = pInfo.get();
		// get id and pw from stream
		NetBase::ReadFromBinStream(inpStream, ptr);

		EProtocol protocol;
		EResult result;
		std::wstring msg;

		// db ���� �б�
		SignInfoPtr getInfo;
		LoadInfo(pInfo->ID, getInfo);


		if (nullptr == getInfo)
		{	// id not exist
			protocol = EProtocol::SignOut;
			result = EResult::NotExistID;
			msg = ResultMSG::NotExistIDMsg;
		}
		else if (getInfo->PW == pInfo->PW)
		{	// sign out success
			protocol = EProtocol::SignOut;
			result = EResult::Success_SignOut;
			msg = ResultMSG::SignOutSuccessMsg;

			// �α׾ƿ� ��Ű��
			inpSession->SetSign(false);
		}
		else
		{	// wrong pw
			protocol = EProtocol::SignOut;
			result = EResult::WrongPW;
			msg = ResultMSG::WrongPWMsg;
		}

#ifdef __DEBUG
		wprintf(L" [id : %ws] , [pw : %ws]\n ", pInfo->ID.c_str(), pInfo->PW.c_str());
		printf("%d\n", result);
		wprintf(L" msg : %ws\n", msg.c_str());
#endif // __DEBUG

		// get new send stream
		auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

		// write result to send stream(only data part)
			// sendpacket is composed with (size + id + data)

		int write_size = 0;
		/// Write to Output Stream
		// sign data = protocol + result + msg len + msg
		// write protocol	
		write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
		// write result
		write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
		// write result msg
		write_size += NetBase::WriteToBinStream(pOutputStream, msg);
		/// write end		

#ifdef __DEBUG
		printf("SignState write to stream : %dbytes\n", write_size);
#endif // __DEBUG	


		// send result to CLIENT
		inpSession->Send(pOutputStream);

		return result;
	}
}