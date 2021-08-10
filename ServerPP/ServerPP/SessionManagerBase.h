#pragma once

template <typename T>
class SessionManagerBase : public Singleton<T>
{
public:
	using SessionCreationFunc = SessionBasePtr(*)();
protected:
	std::unordered_set<SessionBasePtr> m_session_set;
	SessionCreationFunc fpCreateSession;
	SessionManagerBase() :
		m_session_set(), fpCreateSession() {}
public:
	void RegistCreationFunction(SessionCreationFunc fp);

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
	auto delete_session = m_session_set.find(inpSession);
	if (m_session_set.end() != delete_session)
	{
		m_session_set.erase(delete_session);
	}
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
inline std::shared_ptr<Derived_Class> SessionManagerBase<T>::CreateSession()
{
	SessionBasePtr newSession = fpCreateSession();
	m_session_set.insert(newSession);
	return std::static_pointer_cast<Derived_Class>(newSession);
}
