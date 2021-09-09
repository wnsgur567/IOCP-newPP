//#include "FileUtil.h"
//
//namespace Utils
//{
//	inline bool FileUtil::IsFileExist(const std::string filepath)
//	{
//		return std::filesystem::is_regular_file(filepath);
//	}
//	inline bool FileUtil::IsDirectoryExist(const std::string& dirpath)
//	{
//		return std::filesystem::is_directory(dirpath);
//	}
//	inline void FileUtil::CreateNewDirectory(const std::string& dirpath)
//	{
//		std::filesystem::create_directories(dirpath);
//	}
//
//	// if directory has file or dir
//	// remove all
//	// param : dir or file
//
//	inline void FileUtil::RemoveAll(const std::string& path)
//	{
//		std::filesystem::remove_all(path);
//	}
//
//	// if directory has file or dir
//	// exception
//	// param : dir or file
//
//	inline void FileUtil::Remove(const std::string& path)
//	{
//		std::filesystem::remove(path);
//	}
//}