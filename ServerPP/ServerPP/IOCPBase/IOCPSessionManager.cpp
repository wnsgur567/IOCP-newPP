#include "IOCPBase_RootHeader.h"

Implementation_sInstance(IOCP_Base::IOCPSessionManager);

namespace IOCP_Base
{
	bool IOCPSessionManager::Initialize(LPVOID)
	{
		return true;
	}
	
	void IOCPSessionManager::Finalize()
	{
	}

	void IOCPSessionManager::RegistCreationFunction(SessionCreationFunc fp)
	{
		fpCreateSession = fp;
	}

	IOCPSessionBasePtr IOCPSessionManager::CreateSession()
	{
		IOCPSessionBasePtr newSession = fpCreateSession();
		m_session_set.insert(newSession);
		return newSession;
	}

	void IOCPSessionManager::DestroySession(IOCPSessionBasePtr inpSession)
	{
		auto delete_session = m_session_set.find(inpSession);
		if (m_session_set.end() != delete_session)
		{
			m_session_set.erase(delete_session);
		}
	}	
}