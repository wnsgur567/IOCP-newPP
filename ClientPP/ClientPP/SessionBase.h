#pragma once

class SessionBase;
using SessionBasePtr = std::shared_ptr<SessionBase>;

class SessionBase
{
public:
	using psize_t = PacketManager::psize_t;
	using pid_t = ::id_t;
	template <typename T>
	friend class NetworkManagerClientBase;
protected:
	TCPSocketPtr m_pSock;
	SocketAddress m_addr;
	pid_t m_newSendID;
	pid_t m_newRecvID;
public:
	SessionBase();
	virtual ~SessionBase();
	SessionBase(const Session&) = delete;
	SessionBase& operator=(const Session&) = delete;
public:
	TCPSocketPtr GetSockPtr();
	SocketAddress GetSockAddress();
	bool IsDuplicatedPacket(const pid_t inID);
	pid_t CountingRecvID();
	pid_t CountingSendID();

	virtual bool Recv() = 0;
	virtual bool Send() = 0;
	template <typename DerivedClass>
	static SessionBasePtr CreateSession();
};

template<typename DerivedClass>
inline SessionBasePtr  SessionBase::CreateSession()
{
	return std::make_shared<DerivedClass>();
}
