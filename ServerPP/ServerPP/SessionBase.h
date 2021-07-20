#pragma once

class SessionBase;
using SessionBasePtr = std::shared_ptr<SessionBase>;

class SessionBase
{
	template<typename T>
	friend class SessionManagerBase;
	template<typename T>
	friend class IOCPNetworkManagerBase;
public:
	using psize_t = PacketManager::psize_t;
	using id_t = PacketManager::id_t;
	using pid_t = ::id_t;
protected:
	id_t m_id;
	ESessionState m_state;
	TCPSocketPtr m_pSock;
	SocketAddress m_addr;

	pid_t m_newSendID;
	pid_t m_newRecvID;
protected:
	void SetID(id_t inState);
public:
	SessionBase();

	virtual ~SessionBase();
	SessionBase(const SessionBase&) = delete;
	SessionBase& operator=(const SessionBase&) = delete;
	id_t GetID();
	ESessionState GetState();
	TCPSocketPtr GetSockPtr();
	void SetState(ESessionState inState);
	pid_t CountingRecvID();
	pid_t CountingSendID();
public:
	template <typename DerivedClass>
	static SessionBasePtr CreateSession();
};

template<typename DerivedClass>
inline SessionBasePtr  SessionBase::CreateSession()
{
	return std::make_shared<DerivedClass>();
}
