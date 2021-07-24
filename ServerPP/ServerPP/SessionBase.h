#pragma once

class SessionBase;
using SessionBasePtr = std::shared_ptr<SessionBase>;

class SessionBase
{	
private:
	template <typename T>
	friend class SessionManagerBase;
public:
	using packetSize_t = PacketBase::packetSize_t;
	using pid_t = PacketBase::packetId_t;
	using id_t = PacketBase::packetId_t;
protected:
	id_t m_id;
	ESessionState m_state;
	TCPSocketPtr m_pSock;
	SocketAddress m_addr;

	pid_t m_newSendID;
	pid_t m_newRecvID;
public:
	SessionBase();
	virtual ~SessionBase();

	SessionBase(const SessionBase&) = delete;
	SessionBase& operator=(const SessionBase&) = delete;
protected:
	TCPSocketPtr GetSockPtr();
	id_t GetID();
	void SetID(id_t inID);
	ESessionState GetState();
	void SetState(ESessionState inState);
protected:
	virtual void Initialze() = 0;
public:
	template <typename DerivedClass>
	static SessionBasePtr CreateSession();
};

template<typename DerivedClass>
inline SessionBasePtr  SessionBase::CreateSession()
{
	SessionBasePtr pSession = std::make_shared<DerivedClass>();
	pSession->Initialze();
	return pSession;
}
