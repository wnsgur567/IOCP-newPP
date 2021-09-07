#include <iostream>
using namespace std;

#include "StringUtil.h"

int main()
{
	wchar_t w_str[1024];
	char str[1024];

	const char* test_str = "Ű���� ���� ������ �Լ����� ������ �ϰ� Ư���� ����� �ʿ�ġ �ʴ�";

	strcpy(str, test_str);

	printf("ansi : %s\n", str);
	printf("ansi : %llu\n", strlen(str));

	StringUtil::SetLocale();

	std::wstring w_char_str;
	StringUtil::Convert_ansi_to_unicode_string(w_char_str, test_str, strlen(test_str));

	printf("unicode : %ws\n", w_char_str.c_str());
	printf("ansi : %llu\n", w_char_str.length());

	return 0;
}