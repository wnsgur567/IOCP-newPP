#include "IOCPBase_RootHeader.h"

Implementation_sInstance(IOCP_Base::IOCPSessionManager);

namespace IOCP_Base
{
	bool IOCPSessionManager::Initialize(LPVOID) noexcept
	{
#ifdef  __DEBUG
		printf("\n----- IOCPSessionManager::Initialize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- IOCPSessionManager::Initialize() Complete -----\n");
#endif //  __DEBUG

		return true;
	}
	
	void IOCPSessionManager::Finalize() noexcept
	{
#ifdef  __DEBUG
		printf("\n----- IOCPSessionManager::Finalize() -----\n");
#endif //  __DEBUG

#ifdef  __DEBUG
		printf("----- IOCPSessionManager::Finalize() Complete -----\n");
#endif //  __DEBUG
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