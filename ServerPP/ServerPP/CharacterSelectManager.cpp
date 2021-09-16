#include "IOCPNet_RootHeader.h"

Implementation_sInstance(CharacterSelectManager);

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
