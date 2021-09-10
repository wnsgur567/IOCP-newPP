#pragma once


#include <string>
#include <fstream>
#include <filesystem>

//#define __Examples

// c++17 has new byte type
// WARNING : do not use using namespace std
// filesystem (c++ 17) https://modoocode.com/306

namespace Utils
{
	class FileUtil
	{
	public:
		static bool IsFileExist(const std::string filepath);
		static bool IsDirectoryExist(const std::string& dirpath);
		static void CreateNewDirectory(const std::string& dirpath);

		// if directory has file or dir
		// remove all
		// param : dir or file
		static void RemoveAll(const std::string& path);

		// if directory has file or dir
		// exception
		// param : dir or file
		static void Remove(const std::string& path);

//#ifdef __DEBUG
//		static void Example()
//		{
//			std::cout << FileUtil::IsFileExist("FileTest/Test.txt") << std::endl;
//			std::cout << FileUtil::IsFileExist("FileTest/Test") << std::endl;
//			std::cout << FileUtil::IsDirectoryExist("DirTest") << std::endl;
//			std::cout << FileUtil::IsDirectoryExist("DirTest/Test.txt") << std::endl;
//
//			CreateNewDirectory("DirTest");
//
//			if (false == IsDirectoryExist("NewDir"))
//			{
//				CreateNewDirectory("NewDir");
//			}
//
//			RemoveAll("NewDir");
//		}
//#endif
	};

}