#pragma once

template <typename T>
class NetworkManagerClientBase : public Singleton<T>
{
protected:
	SessionBasePtr pSessionBase;
	HandlePtr hRecvThread;
protected:
	NetworkManagerClientBase() {}
public:
	~NetworkManagerClientBase();
	bool Initialize(LPVOID) override;
	void Finalize() override;
	TCPSocketPtr GetSockPtr() const;

	virtual bool DoFrame() = 0;
	virtual bool Recv(RecvPacketPtr& outRecvPacket) = 0;
	virtual	bool Send(SendPacketPtr inpPacket) = 0;
};

template<typename T>
inline NetworkManagerClientBase<T>::~NetworkManagerClientBase()
{

}

template<typename T>
inline bool NetworkManagerClientBase<T>::Initialize(LPVOID arg)
{
	// wsa init
	if (false == SocketUtil::Init())
		return false;

	pSessionBase = *(SessionBasePtr*)arg;

	//家南 积己
	pSessionBase->m_pSock = SocketUtil::CreateTCPSocket();
	if (pSessionBase->m_pSock == nullptr)
	{
		return false;
	}

	sockaddr_in _addr;
	int retval = inet_pton(AF_INET, SERVERIP, &_addr.sin_addr);
	if (retval == SOCKET_ERROR)
		return false;

	// set address
	SocketAddress serverAddr(_addr.sin_addr.s_addr, htons(SERVERPORT));
	pSessionBase->m_addr = serverAddr;

	// connect
	if (false == pSessionBase->m_pSock->Connect(pSessionBase->m_addr))
		return false;

	// debug
	pSessionBase->m_addr.Print("Connet to Server [IP] : %s / [Port] : %d");

	return true;
}

template<typename T>
inline void NetworkManagerClientBase<T>::Finalize()
{
	SocketUtil::CleanUp();
}

template<typename T>
inline TCPSocketPtr NetworkManagerClientBase<T>::GetSockPtr() const
{
	return pSessionBase->GetSockPtr();
}
