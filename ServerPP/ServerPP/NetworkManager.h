#pragma once

template <typename T>
class NetworkManagerServer : public Singleton<T>
{
protected:
	TCPSocketPtr m_pListenSock;
	u_short m_port;
protected:
	NetworkManagerServer() : m_port(SERVERPORT) {}	
public:
	NetworkManagerServer(const NetworkManagerServer&) = delete;
	NetworkManagerServer& operator=(const NetworkManagerServer&) = delete;
	virtual ~NetworkManagerServer();
public:
	virtual bool Initialize(LPVOID) override;
	virtual void Finalize() override;	

	TCPSocketPtr GetListenSockPtr() const;
};


template<typename T>
NetworkManagerServer<T>::~NetworkManagerServer()
{

}

template<typename T>
bool NetworkManagerServer<T>::Initialize(LPVOID)
{
	// wsa init
	if (false == SocketUtil::Init())
		return false;

	// listen 家南 积己
	m_pListenSock = SocketUtil::CreateTCPSocket();
	if (m_pListenSock == nullptr)
	{
		return false;
	}

	// binding
	SocketAddress myAddress(htonl(INADDR_ANY), htons(m_port));
	if (false == m_pListenSock->Bind(myAddress))
		return false;

	// debug
	printf("Initializing NetworkManager at port %d", m_port);

	// listening
	if (false == m_pListenSock->Listen(SOMAXCONN))
		return false;

	return true;
}

template<typename T>
void NetworkManagerServer<T>::Finalize()
{
	SocketUtil::CleanUp();
}

template<typename T>
TCPSocketPtr NetworkManagerServer<T>::GetListenSockPtr() const
{
	return m_pListenSock;
}