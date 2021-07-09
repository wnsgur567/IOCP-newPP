#pragma once


class NetworkManagerClient : public Singleton<NetworkManagerClient>
{
protected:
	TCPSocketPtr m_sock;
	SocketAddress m_serveraddr;
	HandlePtr hRecvThread;
protected:
	NetworkManagerClient() {}
	bool StaticInitialize() override;
	virtual bool Init(u_short inPort, bool isInNonBlock);
public:
	NetworkManagerClient(const NetworkManagerClient&) = delete;
	NetworkManagerClient& operator=(const NetworkManagerClient&) = delete;
	virtual ~NetworkManagerClient();
public:
	bool Initialize() override;
	void Finalize() override;
	virtual bool DoFrame();
	TCPSocketPtr GetSockPtr() const;
};