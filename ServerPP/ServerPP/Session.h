#pragma once

enum class ESessionState
{
	None,

	Disconnected
};

class Session
{
	friend class IOCPSessionManager;
	using psize_t = PacketManager::psize_t;
	using id_t = IOCPSessionManager::clientid_t;
protected:
	id_t m_id;
	ESessionState m_state;
	TCPSocketPtr m_pSock;
	SocketAddress m_addr;

	RecvPacketPtr m_pRecvPacket;
	SendPacketPtr m_pSendPacket;
private:
	void SetID(id_t inState);
public:
	Session();

	virtual ~Session();
	Session(const Session&) = delete;
	Session& operator=(const Session&) = delete;
	id_t GetID();
	ESessionState GetState();
	TCPSocketPtr GetSockPtr();
	void SetState(ESessionState inState);
public:
	virtual bool Send() = 0;
	virtual bool Recv() = 0;
};

class IOCPSession : public Session
{
public:
	using pid_t = ::id_t;
protected:
	pid_t m_newSendID;
	pid_t m_newRecvID;

	std::queue<SendPacketPtr> m_sendQueue;	
public:
	IOCPSession();
	
	bool Recv() override;
	bool Send() override;

	static IOCPSessionPtr CreateSession();
public:
	virtual bool OnAccepted(TCPSocketPtr,SocketAddress);
	virtual bool OnDisconnected();
	virtual bool OnCompleteRecv(RecvPacketPtr);
	virtual bool OnCompleteSend(SendPacketPtr);
};