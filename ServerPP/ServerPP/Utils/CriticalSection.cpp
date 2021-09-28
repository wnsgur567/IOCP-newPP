#include "CriticalSection.h"

namespace MyBase
{
	CriticalSection::CriticalSection()
	{
		InitializeCriticalSection(&m_cs);
	}

	CriticalSection::~CriticalSection()
	{
		DeleteCriticalSection(&m_cs);
	}

	void CriticalSection::Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void CriticalSection::UnLock()
	{
		LeaveCriticalSection(&m_cs);
	}


	AutoLocker::AutoLocker(CriticalSection* pCs)
		: isUnlocked(false), m_pCs(pCs)
	{
		m_pCs->Lock();
	}
	AutoLocker::~AutoLocker()
	{
		ManualUnlock();
	}
	void AutoLocker::ManualUnlock()
	{
		if (false == isUnlocked)
		{
			isUnlocked = true;
			m_pCs->UnLock();
		}
	}
}