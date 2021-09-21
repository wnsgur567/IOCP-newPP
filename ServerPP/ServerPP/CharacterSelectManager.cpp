#include "IOCPNet_RootHeader.h"

Implementation_sInstance(CharacterSelect::CharacterSelectManager);

namespace CharacterSelect
{
	const wchar_t* CharacterSelectManager::ResultMSG::NoCharacterMsg = L"생성된 캐릭터가 없습니다";
	const wchar_t* CharacterSelectManager::ResultMSG::CharacterInfosMsg = L"모든 캐릭터 검색 성공";
	const wchar_t* CharacterSelectManager::ResultMSG::CharacterSelectSuccessMsg = L"캐릭터 선택 완료";
	const wchar_t* CharacterSelectManager::ResultMSG::UndefinedCharacterMsg = L"캐릭터 id error";


	bool CharacterSelectManager::Initialize(LPVOID) noexcept
	{
		return true;
	}

	void CharacterSelectManager::Finalize() noexcept
	{
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
	CharacterSelectManager::ResultData CharacterSelectManager::StateChangedProcess(uint64_t inUserID)
	{
		ResultData retData;

		std::vector<CharacterInfoPtr> user_character_infos;
		LoadInfos(inUserID, user_character_infos);

		retData.protocol = EProtocol::AllCharacterInfo;
		if (user_character_infos.size() == 0)
		{	// no data
			retData.result = EResult::NoData;
			auto msg = ResultMSG::NoCharacterMsg;

			// out stream 조림
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg (no data)
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)retData.protocol);
			write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)retData.result);
			write_size += NetBase::WriteToBinStream(stream, std::wstring(msg));
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg);
#endif
			// write end
			retData.outpStream = stream;
			return retData;
		}
		else
		{
			retData.result = EResult::CharaterInfos;
			auto msg = ResultMSG::CharacterInfosMsg;
			// out stream 조림
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg (no data)
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)retData.protocol);
			write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)retData.result);
			write_size += NetBase::WriteToBinStream(stream, user_character_infos);
			write_size += NetBase::WriteToBinStream(stream, std::wstring(msg));
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg);
#endif
			// write end

			retData.outpStream = stream;
			return retData;
		}
	}

	CharacterSelectManager::ResultData CharacterSelectManager::CharacterSelectProcess(const CharacterInfoPtr inInfo)
	{
		ResultData retData;

		CharacterInfoPtr check_info;
		LoadInfo(inInfo->character_id, check_info);

		retData.protocol = EProtocol::CharacterSelect;
		if (nullptr == check_info)
		{	// 검증 실패
			retData.result = EResult::UndefinedCharacter;
			auto msg = ResultMSG::CharacterInfosMsg;

			// out stream 조림
			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)retData.protocol);
			write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)retData.result);
			write_size += NetBase::WriteToBinStream(stream, std::wstring(msg));
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg);
#endif
			// write end
			retData.outpStream = stream;
			return retData;
		}
		else
		{	// 캐릭터 선택 성공
			retData.result = EResult::Success_CharacterSelect;
			auto msg = ResultMSG::CharacterSelectSuccessMsg;

			auto stream = NetBase::PacketManager::sInstance->GetSendStreamFromPool();

			// wirte to stream 
			// protocol + result + msg
			int write_size = 0;
			write_size += NetBase::WriteToBinStream(stream, (ProtocolSize_t)retData.protocol);
			write_size += NetBase::WriteToBinStream(stream, (ResultSize_t)retData.result);
			write_size += NetBase::WriteToBinStream(stream, std::wstring(msg));
#ifdef __DEBUG	
			printf("CharacterState write to stream : %dbytes\n", write_size);
			printf("%ws\n", msg);
#endif
			// write end
			retData.outpStream = stream;
			return retData;
		}
	}

}
