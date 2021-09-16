#pragma once

// https://armful-log.tistory.com/5		anisi-unicode standard function info
// https://wendys.tistory.com/84		convert ansi-unicode function implements
// https://modoocode.com/249			std::string find function reference


#ifndef _WINSOCK2API_
#include <Windows.h>
#endif // !_WINSOCK2API_

#include <locale.h>
#include <string>
#include <vector>

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

		static std::string trim(std::string& s, const std::string& drop = TRIM_SPACE);
		static std::string rtrim(std::string& s, const std::string& drop = TRIM_SPACE);
		static std::string ltrim(std::string& s, const std::string& drop = TRIM_SPACE);

		template<typename _Ty>
		static inline void Split(std::vector<std::basic_string<_Ty>>& outVec, const std::basic_string<_Ty>& inStr, _Ty delim) noexcept
		{
			outVec.clear();
			std::basic_stringstream<_Ty>  s_stream(inStr);

			std::basic_string<_Ty> tmp;
			while (std::getline(s_stream, tmp, delim))
			{
				outVec.push_back(tmp);
			}
		}

		static std::wstring StrToWstr(const std::string& inStr)
		{
			return std::wstring(inStr.begin(), inStr.end());
		}

		static std::string WstrToStr(const std::wstring& inWstr)
		{
			std::string ret_string;
			Convert_unicode_to_ansi_string(ret_string, inWstr.c_str(), inWstr.size());
			return ret_string;

			// codecvt is deprecated in c++17
			// do not use
			/*using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			return converter.to_bytes(inWstr);*/
		}
	};	
}