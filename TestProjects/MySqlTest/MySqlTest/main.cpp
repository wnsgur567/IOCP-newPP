
#include "SQLManager.h"

#include<iostream>


int main()
{

	using queryResult_t = SQLManager::queryResult_t;

	SQLManager::InitArgs args;

	args.ip = "49.1.218.29";
	args.host_name = "root";
	args.host_password = "1234";
	args.db_name = "sign";
	args.port = 3306;

	SQLManager manager;
	manager.__Initialize(&args);


	queryResult_t results;
	manager.Query("select * from signinfo", results);
	
	manager.Query("INSERT INTO signinfo VALUES(null , 'code1','1234', now() , 1)");

	manager.Query("select * from signinfo", results);

	manager.__Finalize();

	return 0;
}

//int main()
//{
//	MYSQL mysql;
//	mysql_init(&mysql);
//	auto ret = mysql_real_connect(&mysql, "localhost", "root", "1234", "sign", 3306, NULL, 0);
//
//	if (ret == NULL)
//	{
//		printf("aa");
//	}
//	else
//	{
//		printf("nn");
//	}
//
//	return 0;
//}