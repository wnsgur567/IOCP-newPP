#include "base.h"

std::unique_ptr<IOCPSessionManager> Singleton<IOCPSessionManager>::sInstance;

IOCPSessionManager::IOCPSessionManager()
	: m_newID(1),fpCreateSession()
{
}

void IOCPSessionManager::RegistCreationFunction(SessionCreationFunc fp)
{
	fpCreateSession = fp;
}

IOCPSessionPtr IOCPSessionManager::GetSession(const clientid_t inID)
{
	if (m_idToSession_map.end() == m_idToSession_map.find(inID))
		return nullptr;
	return m_idToSession_map[inID];
}

IOCPSessionPtr IOCPSessionManager::CreateSession()
{
	IOCPSessionPtr newSession = fpCreateSession();
	newSession->SetID(m_newID);

	m_idToSession_map.insert({ m_newID ,newSession });

	++m_newID;

	return newSession;
}

void IOCPSessionManager::DestroySession(IOCPSessionPtr inpSession)
{
	m_idToSession_map.erase(inpSession->GetID());
}

bool IOCPSessionManager::Initialize()
{
	return true;
}

void IOCPSessionManager::Finalize()
{
}
