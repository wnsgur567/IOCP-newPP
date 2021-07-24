#pragma once

template <typename T>
class SessionManagerBase : public Singleton<T>
{
public:
	using sessionid_t = ::gid_t;
	using SessionCreationFunc = SessionBasePtr(*)();
protected:
	sessionid_t m_newID;
	std::map<sessionid_t, SessionBasePtr> m_idToSession_map;
	SessionCreationFunc fpCreateSession;
	SessionManagerBase() :
		m_newID(0), m_idToSession_map(), fpCreateSession() {}
public:
	void RegistCreationFunction(SessionCreationFunc fp);

	template <typename Derived_Class>
	std::shared_ptr<Derived_Class> GetSession(const sessionid_t inID);

	template <typename Derived_Class>
	std::shared_ptr<Derived_Class> CreateSession();
	void DestroySession(SessionBasePtr inpSession);

public:
	bool Initialize(LPVOID) override;
	void Finalize() override;
};

template<typename T>
inline void SessionManagerBase<T>::RegistCreationFunction(SessionCreationFunc fp)
{
	fpCreateSession = fp;
}

template<typename T>
inline void SessionManagerBase<T>::DestroySession(SessionBasePtr inpSession)
{
}

template<typename T>
inline bool SessionManagerBase<T>::Initialize(LPVOID)
{
	return true;
}

template<typename T>
inline void SessionManagerBase<T>::Finalize()
{
}

template<typename T>
template<typename Derived_Class>
inline std::shared_ptr<Derived_Class> SessionManagerBase<T>::GetSession(const sessionid_t inID)
{
	if (m_idToSession_map.end() == m_idToSession_map.find(inID))
		return nullptr;
	return m_idToSession_map[inID];
}

template<typename T>
template<typename Derived_Class>
inline std::shared_ptr<Derived_Class> SessionManagerBase<T>::CreateSession()
{
	SessionBasePtr newSession = fpCreateSession();
	m_idToSession_map.insert({ m_newID++ ,newSession });

	newSession->SetID(m_newID);
	++m_newID;

	return std::static_pointer_cast<Derived_Class>(newSession);
}
