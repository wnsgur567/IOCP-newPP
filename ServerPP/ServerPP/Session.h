#pragma once



class Session
{
	friend class IOCPSessionManager;
public:
	using psize_t = PacketManager::psize_t;
	using id_t = IOCPSessionManager::clientid_t;
	using pid_t = ::id_t;
protected:
	id_t m_id;
	ESessionState m_state;
	TCPSocketPtr m_pSock;
	SocketAddress m_addr;

	pid_t m_newSendID;
	pid_t m_newRecvID;
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

class IOCPSessionBase : public Session
{
protected:
	std::queue<SendPacketPtr> m_sendQueue;
public:
	IOCPSessionBase();
	bool Recv() override;
	bool Send() override;

	virtual bool OnAccepted(TCPSocketPtr, SocketAddress) = 0;
	virtual bool OnDisconnected() = 0;
	virtual bool OnCompleteRecv(RecvPacketPtr) = 0;
	virtual bool OnCompleteSend(SendPacketPtr) = 0;
};

class IOCPSession : public IOCPSessionBase, public std::enable_shared_from_this<IOCPSession>
{
public:
	IOCPSession();

	static IOCPSessionPtr CreateSession();
public:
	bool OnAccepted(TCPSocketPtr, SocketAddress) override;
	bool OnDisconnected() override;
	bool OnCompleteRecv(RecvPacketPtr) override;
	bool OnCompleteSend(SendPacketPtr) override;
};