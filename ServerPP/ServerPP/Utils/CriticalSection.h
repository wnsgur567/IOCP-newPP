#pragma once

#ifndef _WINSOCK2API_
#include <Windows.h>
#endif // !_WINSOCK2API_


namespace MyBase
{

	class CriticalSection
	{
	private:
		CRITICAL_SECTION m_cs;
	public:
		CriticalSection();
		~CriticalSection();
	public:
		void Lock();
		void UnLock();
	};

	class AutoLocker
	{
		CriticalSection* m_pCs;
	public:
		AutoLocker(CriticalSection* pCs);
		~AutoLocker();
	};
}