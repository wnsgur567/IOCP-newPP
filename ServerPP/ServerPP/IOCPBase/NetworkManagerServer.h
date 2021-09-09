#pragma once

namespace IOCP_Base
{
	template <typename T>
	class NetworkManagerServer : public MyBase::Singleton<T>
	{
	public:
		struct InitArgs
		{
			std::string ip;
			u_short port = 0;
		};
	protected:
		NetBase::TCPSocketPtr m_pListenSock;
		u_short m_port;
	protected:
		NetworkManagerServer() : m_port(0) {}
	public:
		NetworkManagerServer(const NetworkManagerServer&) = delete;
		NetworkManagerServer& operator=(const NetworkManagerServer&) = delete;
		virtual ~NetworkManagerServer();
	public:
		virtual bool Initialize(LPVOID) override;
		virtual void Finalize() override;

		NetBase::TCPSocketPtr GetListenSockPtr() const;
	};


	template<typename T>
	NetworkManagerServer<T>::~NetworkManagerServer()
	{

	}

	template<typename T>
	bool NetworkManagerServer<T>::Initialize(LPVOID args)
	{
		InitArgs* pArgs = (InitArgs*)args;
		// ip...
		m_port = pArgs->port;


		// wsa init
		if (false == NetBase::SocketUtil::Init())
			return false;

		// listen 家南 积己
		m_pListenSock = NetBase::SocketUtil::CreateTCPSocket();
		if (m_pListenSock == nullptr)
		{
			return false;
		}

		// binding
		NetBase::SocketAddress myAddress(INADDR_ANY, htons(m_port));
		if (false == m_pListenSock->Bind(myAddress))
			return false;

		// debug
		printf("Initializing NetworkManager at port %d\n", m_port);

		// listening
		if (false == m_pListenSock->Listen(SOMAXCONN))
			return false;

		return true;
	}

	template<typename T>
	void NetworkManagerServer<T>::Finalize()
	{
		NetBase::SocketUtil::CleanUp();
	}

	template<typename T>
	NetBase::TCPSocketPtr NetworkManagerServer<T>::GetListenSockPtr() const
	{
		return m_pListenSock;
	}
}