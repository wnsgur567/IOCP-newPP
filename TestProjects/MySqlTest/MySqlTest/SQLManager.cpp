#include "SQLManager.h"

namespace SQL
{
	bool SQLManager::__Initialize(PVOID args)
	{
		m_args = *((InitArgs*)args);
		mysql_init(&m_conn);

		m_handle = mysql_real_connect(
			&m_conn,
			m_args.ip.c_str(),
			m_args.host_name.c_str(),
			m_args.host_password.c_str(),
			m_args.db_name.c_str(),
			m_args.port,
			NULL,
			0
		);

		if (m_handle == nullptr)
		{
#ifdef __DEBUG
			printf("MySql Initializing Error : %s\n", mysql_error(&m_conn));
#endif
			return false;
		}
#ifdef __DEBUG
		printf("SQL connection Initialize... / (version : %s)\n", mysql_get_client_info());
#endif


		mysql_query(m_handle, "set session character_set_connection = euckr;");
		mysql_query(m_handle, "set session character_set_results = euckr;");
		mysql_query(m_handle, "set session character_set_client = euckr;");

#ifdef __DEBUG
		printf("SQL character_set euckr\n");
#endif

		return true;
	}

	void SQLManager::__Finalize()
	{
		mysql_close(m_handle);

#ifdef  _DEBUG
		printf("MySql Close");
#endif
	}

	bool SQLManager::Query(const char* inQuery, queryResult_t& outVec)
	{
		outVec.clear();

		int retval = mysql_query(m_handle, inQuery);
		if (retval != 0)
		{	// query fail
			printf("MySql query error : %s", mysql_error(&m_conn));
			return false;
		}

		// print query results		
		MYSQL_RES* pResult;
		MYSQL_ROW row_data;

		pResult = mysql_store_result(m_handle);

		uint64_t row_count = pResult->row_count;
		uint64_t col_count = pResult->field_count;


		// reserve outVec 		
		outVec.reserve(row_count);

		// read from DB data	
		for (uint64_t row = 0; row < row_count; row++)
		{
			// reserve outVec's vec
			outVec.push_back(std::vector<std::string>());
			outVec[row].reserve(col_count);
			row_data = mysql_fetch_row(pResult);

			for (uint64_t col = 0; col < col_count; col++)
			{
				outVec[row].push_back(row_data[col]);
#ifdef __DEBUG
				printf("%s ", row_data[col]);
#endif
			}
#ifdef __DEBUG
			printf("\n");
#endif
		}
		mysql_free_result(pResult);

#ifdef __DEBUG
		printf("select query complete\n");
#endif

		return true;
	}

	bool SQLManager::Query(const char* inQuery)
	{
		int retval = mysql_query(m_handle, inQuery);
		if (retval != 0)
		{	// query fail
			printf("MySql query error : %s", mysql_error(&m_conn));
			return false;
		}


#ifdef __DEBUG
		printf("other query complete\n");
#endif

		return true;
	}
}