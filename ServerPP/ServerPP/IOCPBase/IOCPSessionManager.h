#pragma once

namespace IOCP_Base
{
	class IOCPSessionManager : public MyBase::Singleton<IOCPSessionManager>
	{
		friend class Singleton;
	public:
		using SessionCreationFunc = IOCPSessionBasePtr(*)();
	protected:
		std::unordered_set<IOCPSessionBasePtr> m_session_set;
		SessionCreationFunc fpCreateSession;
		IOCPSessionManager() :
			m_session_set(), fpCreateSession() {}
	public:
		void RegistCreationFunction(SessionCreationFunc fp);
		
		IOCPSessionBasePtr CreateSession();
		void DestroySession(IOCPSessionBasePtr inpSession);

	public:
		bool Initialize(LPVOID) override;
		void Finalize() override;
	};
}