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

	SignManager::EResult SignManager::SignUpProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{	// 회원가입 process

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

		// ID 정보가 있는지 확인하기
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
	{	// 회원 탈퇴 process

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

		// db 정보 읽기
		SignInfoPtr getInfo;
		LoadInfo(pInfo->ID, getInfo);


		if (nullptr == getInfo)
		{	// 해당 ID 가 없는 경우
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
		{	// ID 일치
			if (getInfo->PW == pInfo->PW)
			{	// PW 일치

				// db 에서 삭제
				DeleteInfo(pInfo);

				protocol = EProtocol::DeleteAccount;
				result = EResult::Success_DeleteAccount;
				msg = ResultMSG::DeleteSuccessMsg;

				// 로그아웃 시키기
				inpSession->SetSign(false);
			}
			else
			{	// PW 불일치
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
	{	// 로그인 process

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

		// db 정보 읽기
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

			// 로그인 시키기
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
	{	// 로그아웃 process

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

		// db 정보 읽기
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

			// 로그아웃 시키기
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