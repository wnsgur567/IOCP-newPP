#include "IOCPNet_RootHeader.h"

Implementation_sInstance(CharacterSelect::CharacterSelectManager);

namespace CharacterSelect
{
	const wchar_t* CharacterSelectManager::ResultMSG::NoCharacterMsg = L"������ ĳ���Ͱ� �����ϴ�";
	const wchar_t* CharacterSelectManager::ResultMSG::CharacterInfosMsg = L"��� ĳ���� �˻� ����";
	const wchar_t* CharacterSelectManager::ResultMSG::CharacterSelectSuccessMsg = L"ĳ���� ���� �Ϸ�";
	const wchar_t* CharacterSelectManager::ResultMSG::UndefinedCharacterMsg = L"ĳ���� id error";


	bool CharacterSelectManager::Initialize(LPVOID) noexcept
	{
#ifdef  __DEBUG
		printf("\n----- CharacterSelectManager::Initialize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- CharacterSelectManager::Initialize() Complete -----\n");
#endif //  __DEBUG
		return true;
	}

	void CharacterSelectManager::Finalize() noexcept
	{
#ifdef  __DEBUG
		printf("\n----- CharacterSelectManager::Finalize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- CharacterSelectManager::Finalize() Complete -----\n");
#endif //  __DEBUG
	}

	bool CharacterSelectManager::IsExist(uint64_t character_id) const
	{
		std::string query("select * from usercharacter where user_id = ");
		query += std::to_string(character_id);

		SQL::SQLManager::queryResult_t results_vec;
		try
		{
#ifdef __DEBUG
			printf("query : %s\n", query.c_str());
#endif
			SQL::SQLManager::sInstance->Query(query, results_vec);
		}
		catch (const SQL::SQLQueryException& e)
		{
			printf("%s\n", e.what());
			throw;
		}

		if (results_vec.size() == 0)
			return false;
		return true;
	}

	void CharacterSelectManager::LoadInfo(uint64_t character_id, CharacterInfoPtr& outInfo)
	{
		std::string query("select * from usercharacter where character_id = ");
		query += std::to_string(character_id);
		SQL::SQLManager::queryResult_t results_vec;
		try
		{
#ifdef __DEBUG
			printf("query : %s\n", query.c_str());
#endif
			SQL::SQLManager::sInstance->Query(query, results_vec);
		}
		catch (const SQL::SQLQueryException& e)
		{
			printf("%s\n", e.what());
			throw;
		}

		if (results_vec.size() == 0)
		{
			outInfo = nullptr;
			return;
		}

		outInfo = std::make_shared<CharacterInfo>();

		outInfo->SetInfo(
			std::stoull(results_vec[0][0]),	// char id
			std::stoull(results_vec[0][1]),	// user id
			std::stoul(results_vec[0][2]),	// char type
			std::wstring().assign(results_vec[0][3].begin(), results_vec[0][3].end()) // char name
		);
	}

	void CharacterSelectManager::LoadInfos(uint64_t user_id, std::vector<CharacterInfoPtr>& outVec)
	{
		outVec.clear();

		std::string query("select * from usercharacter where user_id = ");
		query += std::to_string(user_id);
		SQL::SQLManager::queryResult_t results_vec;
		try
		{
#ifdef __DEBUG
			printf("query : %s\n", query.c_str());
#endif
			SQL::SQLManager::sInstance->Query(query, results_vec);
		}
		catch (const SQL::SQLQueryException& e)
		{
			printf("%s\n", e.what());
			throw;
		}

		if (results_vec.size() == 0)
		{

			return;
		}

		for (const auto& row_item : results_vec)
		{
			CharacterInfoPtr newInfo = std::make_shared<CharacterInfo>();

			newInfo->SetInfo(
				std::stoull(row_item[0]),	// char id
				std::stoull(row_item[1]),	// user id
				std::stoul(row_item[2]),	// char type
				std::wstring().assign(row_item[3].begin(), row_item[3].end()) // char name
			);

			outVec.push_back(newInfo);
		}
	}

	void CharacterSelectManager::SaveInfo(const CharacterInfoPtr inInfo)
	{
		std::stringstream query;
		query << "insert into usercharacter values( null , ";
		query << std::to_string(inInfo->character_id) << " , ";
		query << std::to_string(inInfo->user_id) << " , ";
		query << std::to_string(inInfo->character_type) << " , ";
		query << "\'" << inInfo->character_name.c_str() << "\'" << " ) ";


		try
		{
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

	void CharacterSelectManager::DeleteInfo(const CharacterInfoPtr inInfo)
	{
		std::stringstream query;
		query << "DELETE FROM usercharacter WHERE character_id = ";
		query << std::to_string(inInfo->character_id);

		try
		{
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


	// sign state ���� character select state �� ����Ǿ��� �� ó���� �͵�
	// �ش� ������ ���� ĳ���� �������� �ҷ��ͼ� send ��
	CharacterSelectManager::EResult CharacterSelectManager::StateChangedProcess(IOCPSessionPtr inpSession)
	{
		// 1. user id�� Ȯ��
		// 2. �ش� ������ ���� ��� ĳ���� ���� �ҷ����� (SQL)
		// 3. �ҷ��� ������ Session ���ؿ��� ����
		// 4. ��� ĳ���� ������ ����

		EProtocol protocol;
		EResult result;
		std::wstring msg;

		auto user_id = inpSession->GetUserID();

		// load character data from db
		std::vector<CharacterInfoPtr> user_character_infos;
		LoadInfos(user_id, user_character_infos);


		if (user_character_infos.size() == 0)
		{	// no character
			protocol = EProtocol::AllCharacterInfo;
			result = EResult::NoData;
			msg = ResultMSG::NoCharacterMsg;

			// out stream ����
			auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg (no data)
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
			write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
			write_size += NetBase::WriteToBinStream(pOutputStream, msg);
			// write end
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg.c_str());
#endif
			// send result to CLIENT
			inpSession->Send(pOutputStream);

			return result;
		}
		else
		{
			protocol = EProtocol::AllCharacterInfo;
			result = EResult::CharaterInfos;
			msg = ResultMSG::CharacterInfosMsg;

			// out stream ����
			auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + characterInfo vector + msg (no data)
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
			write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
			write_size += NetBase::WriteToBinStream(pOutputStream, user_character_infos);
			write_size += NetBase::WriteToBinStream(pOutputStream, std::wstring(msg));
			// write end
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg.c_str());
#endif
			// send result to CLIENT
			inpSession->Send(pOutputStream);

			return result;
		}
	}

	CharacterSelectManager::EResult CharacterSelectManager::CharacterSelectProcess(NetBase::InputMemoryStreamPtr inpStream, IOCPSessionPtr inpSession)
	{
		// get selected character info from stream
		CharacterInfoPtr pInfo = std::make_shared<CharacterInfo>();
		//ISerializable* ptr = pInfo.get();
		NetBase::ReadFromBinStream(inpStream, pInfo);	
		inpSession->SetCharacterInfo(pInfo);		

		EProtocol protocol;
		EResult result;
		std::wstring msg;


		// db ���� ���� ��������
		CharacterInfoPtr check_info;
		LoadInfo(pInfo->character_id, check_info);
		
		if (nullptr == check_info)
		{	// character ���� ����
			protocol = EProtocol::CharacterSelect;
			result = EResult::UndefinedCharacter;
			msg = ResultMSG::CharacterInfosMsg;

			// out stream ����
			auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
			write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
			write_size += NetBase::WriteToBinStream(pOutputStream, msg);
			// write end
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg.c_str());
#endif			
			// send result to CLIENT
			inpSession->Send(pOutputStream);

			return result;
		}
		else
		{	// ĳ���� ���� ����
			protocol = EProtocol::CharacterSelect;
			result = EResult::Success_CharacterSelect;
			msg = ResultMSG::CharacterSelectSuccessMsg;

			auto pOutputStream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(pOutputStream, (ProtocolSize_t)protocol);
			write_size += NetBase::WriteToBinStream(pOutputStream, (ResultSize_t)result);
			write_size += NetBase::WriteToBinStream(pOutputStream, msg);
			// write end
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg.c_str());
#endif
			// send result to CLIENT
			inpSession->Send(pOutputStream);

			return result;
		}
	}
}
