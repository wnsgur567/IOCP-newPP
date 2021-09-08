#pragma once

// c examples https://zetcode.com/db/mysqlc/

#define __DEBUG

#include <mysql.h>
#pragma comment(lib,"libmysql.lib")

#include <string>
#include <vector>
#include <memory>

#include "../Singleton.h"

namespace SQL
{

	class SQLManager : public MyBase::Singleton<SQLManager>
	{
	public:
		using queryResult_t = std::vector<std::vector<std::string>>;
		constexpr static int STR_SIZE = 20;

		struct InitArgs
		{
		public:
			std::string ip;
			std::string host_name;
			std::string host_password;
			std::string db_name;
			unsigned int port = 3306;
		};

	private:
		InitArgs m_args;
		MYSQL m_conn;
		MYSQL* m_handle;		// sql handle	
	public:
		SQLManager() : m_args(), m_conn(), m_handle(nullptr) {}


		bool Initialize(LPVOID args) override;
		void Finalize() override;

		// for select results
		bool Query(
			const char* inQuery,
			queryResult_t& outVec);

		bool Query(const char* inQuery);
	};
}