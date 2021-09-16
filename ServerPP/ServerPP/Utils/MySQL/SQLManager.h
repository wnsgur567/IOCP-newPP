#pragma once

// c examples https://zetcode.com/db/mysqlc/

#include <mysql.h>
#pragma comment(lib,"libmysql.lib")

#include <string>
#include <vector>
#include <memory>

#include "../Singleton.h"
#include "../TryCatchFinally.h"

namespace SQL
{
	class SQLQueryException : public std::exception
	{
		public:
		const char* what() const noexcept override
		{
			return "No Available Query";
		}
	};

	class SQLManager : public MyBase::Singleton<SQLManager>
	{
		friend class Singleton;
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

		SQLManager() : m_args(), m_conn(), m_handle(nullptr) {}
	public:

		bool Initialize(LPVOID args) noexcept override;
		void Finalize() noexcept override;

		// for select results
		void Query(
			const char* inQuery,
			queryResult_t& outVec);
		void Query(
			const std::string& inQuery,
			queryResult_t& outVec);

		void Query(const char* inQuery);
		void Query(const std::string& inQuery);
	};
}