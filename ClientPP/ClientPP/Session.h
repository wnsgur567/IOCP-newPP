#pragma once

enum class ESessionState
{
	None,

	Conneted,

	Test,

	Disconneted,
};

class ClientSession
{
protected:
	ESessionState m_state;
	TCPSocketPtr m_pSock;
	SocketAddress m_addr;

	RecvPacketPtr m_pRecvPacket;
	SendPacketPtr m_pSendPacket;
public:
	ClientSession();

	virtual ~ClientSession();
	ClientSession(const ClientSession&) = delete;
	ClientSession& operator=(const ClientSession&) = delete;

	ESessionState GetState();
	TCPSocketPtr GetSockPtr();
	void SetState(ESessionState inState);

	bool Recv();
	bool Send();
	static ClientSessionPtr CreateSession();

};