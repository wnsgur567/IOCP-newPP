#pragma once

// https://armful-log.tistory.com/5		anisi-unicode standard function info
// https://wendys.tistory.com/84		convert ansi-unicode function implements
// https://modoocode.com/249			std::string find function reference



#include <Windows.h>
#include <locale.h>
#include <string>

namespace Utils
{
constexpr auto TRIM_SPACE = " \t\n\v";
constexpr auto SPLITER = '`';

	class StringUtil
	{
	public:
		static bool SetLocale();
		static DWORD Convert_ansi_to_unicode_string(
			std::wstring& unicode,
			const char* ansi,
			const size_t ansi_size
		);
		static DWORD Convert_unicode_to_ansi_string(
			std::string& ansi,
			const wchar_t* unicode,
			const size_t unicode_size);


		static DWORD Convert_unicode_to_utf8_string(
			std::string& utf8,
			const wchar_t* unicode,
			const size_t unicode_size);
		static DWORD Convert_utf8_to_unicode_string(
			std::wstring& unicode,
			const char* utf8,
			const size_t utf8_size);

		std::string trim(std::string& s, const std::string& drop = TRIM_SPACE);
		std::string rtrim(std::string& s, const std::string& drop = TRIM_SPACE);
		std::string ltrim(std::string& s, const std::string& drop = TRIM_SPACE);
	};
}