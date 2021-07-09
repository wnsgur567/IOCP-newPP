#include "base.h"

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
	: m_pCs(pCs)
{
	m_pCs->Lock();
}
AutoLocker::~AutoLocker()
{
	m_pCs->UnLock();
}