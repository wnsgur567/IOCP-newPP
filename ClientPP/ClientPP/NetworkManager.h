#pragma once


class NetworkManagerClient : public Singleton<NetworkManagerClient>
{
	friend class Singleton;
protected:
	TCPSocketPtr m_sock;
	SocketAddress m_serveraddr;
	HandlePtr hRecvThread;
protected:
	NetworkManagerClient() {}	
	virtual bool Init(const char* inIP, u_short inPort);
public:
	NetworkManagerClient(const NetworkManagerClient&) = delete;
	NetworkManagerClient& operator=(const NetworkManagerClient&) = delete;
	virtual ~NetworkManagerClient();
public:
	bool Initialize() override;
	void Finalize() override;
	virtual bool DoFrame();
	TCPSocketPtr GetSockPtr() const;

	// �ϴ� ����� ����
	static bool Recv(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket);
	static bool Send(const TCPSocketPtr inpSock, SendPacketPtr inpPacket);
};