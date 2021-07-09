#pragma once

class NetworkManagerServer
{
protected:
	TCPSocketPtr m_pListenSock;
protected:
	NetworkManagerServer() {}
	virtual bool Init(u_short inPort, bool isInNonBlock);
public:
	NetworkManagerServer(const NetworkManagerServer&) = delete;
	NetworkManagerServer& operator=(const NetworkManagerServer&) = delete;
	virtual ~NetworkManagerServer();
public:
	virtual bool DoFrame() = 0;

	TCPSocketPtr GetListenSockPtr() const;
};

class IOCPNetworkManager : public NetworkManagerServer
{
public:
	static std::unique_ptr<IOCPNetworkManager> sInstance;
	using psize_t = PacketManager::psize_t;
protected:
	HandlePtr	 m_pHcp;
	std::vector<HandlePtr> m_hAcceptThreads;
	std::vector<HandlePtr> m_hWorkerThreads;
protected:
	IOCPNetworkManager() {}

	bool Init(u_short inPort, bool isInNonBlock) override;
public:
	static bool StaticInit(u_short inPort);
	IOCPNetworkManager(const IOCPNetworkManager&) = delete;
	IOCPNetworkManager& operator=(const IOCPNetworkManager&) = delete;
	~IOCPNetworkManager();
public:
	virtual bool DoFrame();
	HandlePtr GetHCPPtr() const;
public:

	static DWORD WINAPI AcceptThread(LPVOID arg);
	static bool RecvAsync(const TCPSocketPtr inpSock, RecvPacketPtr& outRecvPacket);	// 비동기 recv
	static bool SendAsync(const TCPSocketPtr inpSock, SendPacketPtr inSendPacket);	// 비동기 send	
	static E_PacketState CompleteRecv(TCPSocketPtr inpSock, RecvPacketPtr& outpPacket, const psize_t inCompletebyte);
	static E_PacketState CompleteSend(TCPSocketPtr inpSock, SendPacketPtr inpPacket, const psize_t inCompletebyte);
	static DWORD WINAPI WorkerThread(LPVOID arg);
};

class SelectNetworkManager : public NetworkManagerServer
{

};

