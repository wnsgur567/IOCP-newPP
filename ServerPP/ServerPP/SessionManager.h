#pragma once

class IOCPSessionManager : public Singleton<IOCPSessionManager>
{
	friend class Singleton;
public:
	using clientid_t = unsigned __int32;
	using SessionCreationFunc = IOCPSessionPtr(*)();
protected:
	clientid_t m_newID;
	std::map<clientid_t, IOCPSessionPtr> m_idToSession_map;
	SessionCreationFunc fpCreateSession;
public:
	IOCPSessionManager();
	void RegistCreationFunction(SessionCreationFunc fp);
	IOCPSessionPtr GetSession(const clientid_t inID);

	virtual IOCPSessionPtr CreateSession();
	virtual void DestroySession();
public:
	bool Initialize() override;
	void Finalize() override;
};
